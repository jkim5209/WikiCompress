#include "UrlGenerator.hpp"

std::string UrlGenerator::gen_str() {
    std::string output = "";
    if (is_https[idx]) {
        output += "https://";
    } else {
        output += "http://";
    }

    if (has_www[idx]) {
        output += "www.";
    }

    output += url[idx];
    ++idx;

    return output;
}