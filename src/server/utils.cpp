#include <algorithm>
#include <cctype>
#include <locale>
#include "utils.hpp"

namespace socketchatserver {

void StringHelper::trim(std::string &string) {
    ltrim(string);
    rtrim(string);
}

void StringHelper::ltrim(std::string &string) {
    string.erase(string.begin(), std::find_if(string.begin(), string.end(), [](int ch) {
        return !std::isspace(ch);
    }));
}

void StringHelper::rtrim(std::string &string) {
    string.erase(std::find_if(string.rbegin(), string.rend(), [](int ch) {
        return !std::isspace(ch);
    }).base(), string.end());
}

std::string StringHelper::trim_copy(std::string string) {
    StringHelper::trim(string);
    return string;
}

std::string StringHelper::ltrim_copy(std::string string) {
    StringHelper::ltrim(string);
    return string;
}

std::string StringHelper::rtrim_copy(std::string string) {
    StringHelper::rtrim(string);
    return string;
}
}
