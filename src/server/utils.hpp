#pragma once

#include <locale>

namespace socketchatserver {

class StringHelper {
    public:
        static void trim(std::string &);
        static void ltrim(std::string &);
        static void rtrim(std::string &);
        static std::string trim_copy(std::string);
        static std::string ltrim_copy(std::string);
        static std::string rtrim_copy(std::string);
};
}