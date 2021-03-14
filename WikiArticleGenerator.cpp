#include "WikiArticleGenerator.hpp"
#include <regex>
#include <string>
#include <iostream>

std::deque<std::string> WikiArticleGenerator::misc;
const std::string WikiArticleGenerator::start_str = "  <page>\n";
const std::string WikiArticleGenerator::end_str = "  </page>\n";

std::string WikiArticleGenerator::gen_str()
{
    std::string output = start_str;
    std::string body = misc.front();
    output += body;
    misc.pop_front();
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
        misc.push_back(page_contents);
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
    return;
}