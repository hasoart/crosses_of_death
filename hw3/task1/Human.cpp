#include <iostream>
#include <string>

#include "json.hpp"

using json = nlohmann::json;

enum Sex {
    female = 0,
    male = 1,
    unknown = 2,
};

std::istream& operator>>(std::istream& is, Sex& sex) {
    std::string sex_str;
    is >> sex_str;
    if (sex_str == "Male" or sex_str == "male" or sex_str == "1")
        sex = Sex::male;
    else if (sex_str == "Female" or sex_str == "female" or sex_str == "0")
        sex = Sex::female;
    else
        sex = Sex::unknown;

    return is;
}

struct Human {
    std::string first_name;
    std::string last_name;
    
    Sex sex;

    double height;
    double weight;

    json toJson() {
        json human;
        human["first_name"] = first_name;
        human["last_name"] = last_name;
        human["sex"] = sex;
        human["height"] = height;
        human["weight"] = weight;

        return human;
    }
};

std::istream& operator>>(std::istream& is, Human& hum) {
    is >> hum.first_name >> hum.last_name >> hum.sex >> hum.height >> hum.weight;
    
    return is;
}

std::ostream& operator<<(std::ostream& os, Human& hum) {
    os << "First name: " << hum.first_name << "\n" <<
          "Last name: " << hum.last_name << "\n" <<
          "Sex: " << hum.sex << "\n" <<
          "Height: " << hum.height << "\n" <<
          "Weight:" << hum.weight << std::endl;

    return os;
}
