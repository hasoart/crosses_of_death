#include <boost/asio.hpp>
#include <boost/algorithm/string.hpp>
#include <iostream>
#include <chrono>
#include <ctime>
#include <thread>
#include <future>

const auto port = 65001u;
const std::string ip = "127.0.0.1";

bool is_running = false;

bool read(boost::asio::ip::tcp::socket& s, std::string& message) {
	boost::asio::streambuf buffer;

    try {
        boost::asio::read_until(s, buffer, '\n');
    } catch (const boost::system::system_error& ex) {
        return false;
    }

	std::istream stream(&buffer);

	std::getline(stream, message);
	return true;
}

int listen_messages(boost::asio::ip::tcp::socket& socket) {
    std::string message;

    while (read(socket, message)) {
        const auto now = std::chrono::system_clock::now();
        const std::time_t t_c = std::chrono::system_clock::to_time_t(now);
        std::string timestamp(std::ctime(&t_c));
        boost::trim_right(timestamp);
        timestamp = timestamp.substr(4);
        std::cout << "[" << timestamp << "]\t" << message << std::endl;
    }

    std::cout << "Connection closed" << std::endl;
    is_running = false;
    return 0;
}

void send_messages(boost::asio::ip::tcp::socket& socket) {
	std::string message; 
    while (std::getline(std::cin, message) && is_running) {
        try {
            boost::asio::write(socket, boost::asio::buffer(message + "\n"));
        } catch (const boost::system::system_error& ex) {
            std::cout << "Connection lost" << std::endl;
            return;
        }

        if (message == "/EXIT") {
            break;
        }
    }
}

int main() {
    boost::asio::ip::tcp::endpoint endpoint(
            boost::asio::ip::address::from_string(ip), port);
    boost::asio::io_service serv;
    boost::asio::ip::tcp::acceptor acceptor(serv, endpoint.protocol());
    acceptor.bind(endpoint);
    acceptor.listen();

    boost::asio::ip::tcp::socket socket(serv);

    std::cout << "Waiting for connection..." << std::endl;
    acceptor.accept(socket);
    is_running = true;
    std::cout << "Connected!" << std::endl;


    std::future<int> listen_client = std::async(
            std::launch::async,
            [&](){return listen_messages(socket);});

    send_messages(socket);
}
