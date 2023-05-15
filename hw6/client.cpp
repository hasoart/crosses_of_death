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
	boost::asio::ip::tcp::socket socket(serv, endpoint.protocol());

    const auto MAX_RECONNECT_ATTEMPTS = 3;
    auto connect_attempts = 0;
    auto reconnect_retry_time = std::chrono::milliseconds(1000);

    bool connected = false;
    while (!connected && connect_attempts < MAX_RECONNECT_ATTEMPTS) {
        try {
            socket.connect(endpoint);
            connected = true;
        } catch (const boost::system::system_error& ex) {
            std::cout << "Reconnect attempt " << ++connect_attempts << std::endl;
            std::this_thread::sleep_for(reconnect_retry_time);
        }
    }
    
    if (!connected) {
        std::cout << "Failed to connect to " << ip << ":" << port << std::endl;
        return 0;
    }
    
    is_running = true;
    std::cout << "Connected!" << std::endl;

    std::future<void> send_client = std::async(
            std::launch::async,
            [&](){return send_messages(socket);});

    listen_messages(socket);
}
