#include "WikiArticleGenerator.hpp"
#include <regex>
#include <string>
#include <iostream>

TextBodyGenerator WikiArticleGenerator::text_body_generator;
std::deque<std::string> WikiArticleGenerator::misc;
const std::string WikiArticleGenerator::start_str = "  <page>\n";
const std::string WikiArticleGenerator::end_str = "  </page>\n";

std::string WikiArticleGenerator::gen_str()
{
    std::string output = start_str;
    std::string before_text = misc.front();
    misc.pop_front();
    output += before_text;
    output += text_body_generator.gen_str();
    std::string after_text = misc.front();
    misc.pop_front();
    output += after_text;
    output += end_str;
    return output;
}

void WikiArticleGenerator::store_str(const std::string& str)
{
    assert(str.find(start_str) == 0);
    std::regex page_re("  <page>\n((.|\n)*)  </page>\n");
    std::smatch m;
    if (std::regex_search(str, m, page_re))
    {
        std::string page_contents = m[1];
        auto p = text_body_generator.find_range(page_contents);
        if (p.first == -1)
        {
            std::cout << "No Text contained within page but expected text to be found" << std::endl;
            throw "FIND RANGE FAILED";
        }
        text_body_generator.store_str(page_contents.substr(p.first, p.second));
        std::string before_text = page_contents.substr(0, p.first);
        std::string after_text = page_contents.substr(p.first + p.second, page_contents.length() - p.first - p.second);
        misc.push_back(before_text);
        misc.push_back(after_text);
        return;
    }

    std::cout << "NO regex match for WikiArticle" << std::endl;
    throw "NO REGEX MATCHES!";
}

std::pair<int, int> WikiArticleGenerator::find_range(const std::string& str)
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

void WikiArticleGenerator::store_to_file(const std::string& filename)
{
    text_body_generator.store_to_file("text_body_generator_" + filename);
    return;
}