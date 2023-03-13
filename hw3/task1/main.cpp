#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <filesystem>

#include "Human.cpp"

namespace fs = std::filesystem;

const std::string save_dir_name = "people";

int main() {
    fs::path save_dir(save_dir_name);

    // check for existance and availability
    if (not fs::exists(save_dir))
        fs::create_directory(save_dir);
    else if (not fs::is_directory(save_dir))
            throw "Cannot save to specified directory";


    unsigned N;
    std::vector<json> people;

    std::cout << "How many people to enter?\n";
    std::cin >> N;
    std::cout << "[first_name] [last_name] [sex] [height] [weight]" << std::endl;

    for (auto i = 0; i < N; ++i) {
        Human human;
        std::cin >> human;

        people.push_back(human.toJson());
    }

    for (auto& human: people) {
        std::string filename = static_cast<std::string>(human["first_name"]) + "_" +
                               static_cast<std::string>(human["last_name"]) + ".json";
        fs::path path = save_dir / (filename);

        std::fstream fs(path, std::ios::out);
        fs << human.dump(4) << std::endl;
    }

    return 0;
}
