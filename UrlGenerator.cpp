#include "UrlGenerator.hpp"
#include <ctype.h>

std::queue<bool> UrlGenerator::is_https;
std::queue<bool> UrlGenerator::has_www;
std::queue<std::string> UrlGenerator::url;

const std::string https_str = "https://";
const std::string http_str = "http://";
const std::string www_str = "www.";

std::string UrlGenerator::gen_str() {
    std::string output;
    if (is_https.front()) {
        output += https_str;
    } else {
        output += http_str;
    }
    is_https.pop();

    if (has_www.front()) {
        output += www_str;
    }
    has_www.pop();

    output += url.front();
    url.pop();

    return output;
}

void UrlGenerator::store_str(const std::string& str) {
    size_t used_len = 0;
    if (str.find(https_str) == 0) {
        is_https.push(true);
        used_len += https_str.size();
    } else if (str.find(http_str) == 0) {
        is_https.push(false);
        used_len += http_str.size();
    } else {
        throw "UNEXPECTED URL PREFIX";
    }

    if (str.find(www_str, used_len) == used_len) {
        used_len += www_str.size();
        has_www.push(true);
    } else {
        has_www.push(false);
    }

    url.push(str.substr(used_len, str.size()));
}


std::pair<int, int> UrlGenerator::find_range(
        const std::string& str) {
    // starts from any mention of http
    size_t start = str.find("http");
    size_t len = 0;
    while (len < str.size() && !(isspace(str[len]) || str[len] == ']')) {
        ++len;
    }
    return {start, len};
}