#include "TextBodyGenerator.hpp"
#include <regex>
#include <string>
#include <iostream>

std::deque<std::string> TextBodyGenerator::misc;
const std::string TextBodyGenerator::start_str = "<text xml:space=\"preserve\">";
const std::string TextBodyGenerator::end_str = "</text>";

std::string TextBodyGenerator::gen_str()
{
    std::string output = start_str;
    std::string body = misc.front();
    output += body;
    misc.pop_front();
    output += end_str;
    return output;
}

void TextBodyGenerator::store_str(const std::string& str)
{
    assert(str.find(start_str) == 0);
    std::regex text_re("<text xml:space=\"preserve\">((.|\n)*)</text>");
    std::smatch m;
    if (std::regex_search(str, m, text_re))
    {
        std::string text_contents = m[1];
        misc.push_back(text_contents);
        return;
    }

    std::cout << "NO regex match for TextBody" << std::endl;
    throw "NO REGEX MATCHES!";
}

std::pair<int, int> TextBodyGenerator::find_range(const std::string& str)
{
    size_t start = str.find(start_str);
    size_t end = str.find(end_str);
    if (start == std::string::npos)
    {
        return {-1,0};
    }
    if (end == std::string::npos) 
    {
        return {-1,0};
    }
    if (end < start) 
    {
        return {-1,0};
    }
    return {start, end + end_str.size() - start};
}

void TextBodyGenerator::store_to_file(const std::string& filename)
{
    return;
}