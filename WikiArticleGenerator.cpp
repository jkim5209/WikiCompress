#include "WikiArticleGenerator.hpp"
#include <regex>
#include <string>
#include <iostream>

ContributorGenerator WikiArticleGenerator::contributor_generator;
TextBodyGenerator WikiArticleGenerator::text_body_generator;
std::deque<std::string> WikiArticleGenerator::titles;
std::deque<int> WikiArticleGenerator::article_ids;
std::deque<bool> WikiArticleGenerator::has_restriction;
std::deque<std::string> WikiArticleGenerator::article_restrictions;
std::deque<int> WikiArticleGenerator::revision_ids;
std::deque<int> WikiArticleGenerator::timestamp_years;
std::deque<int> WikiArticleGenerator::timestamp_months;
std::deque<int> WikiArticleGenerator::timestamp_days;
std::deque<int> WikiArticleGenerator::timestamp_hours;
std::deque<int> WikiArticleGenerator::timestamp_minutes;
std::deque<int> WikiArticleGenerator::timestamp_seconds;
std::deque<bool> WikiArticleGenerator::has_minor;
std::deque<bool> WikiArticleGenerator::has_comment;
std::deque<std::string> WikiArticleGenerator::comments;
const std::string WikiArticleGenerator::start_str = "  <page>\n";
const std::string WikiArticleGenerator::end_str = "  </page>\n";

std::string WikiArticleGenerator::gen_str()
{
    std::string output = start_str;
    output += "    <title>";
    output += titles.front();
    titles.pop_front();
    output += "</title>\n    <id>";
    output += std::to_string(article_ids.front());
    article_ids.pop_front();
    output += "</id>\n";
    if (has_restriction.front())
    {
        output += "    <restrictions>";
        output += article_restrictions.front();
        output += "</restrictions>\n";
        article_restrictions.pop_front();
    }
    has_restriction.pop_front();
    output += "    <revision>\n      <id>";
    output += std::to_string(revision_ids.front());
    revision_ids.pop_front();
    output += "</id>\n      <timestamp>";
    output += std::to_string(timestamp_years.front());
    timestamp_years.pop_front();
    output += "-";
    // getting lazy here because strings and sprintf are annoying to do together, and this is a pretty specific use case
    // clean this up eventually
    int month = timestamp_months.front();
    timestamp_months.pop_front();
    if (month < 10)
    {
        output += "0";
    }
    output += std::to_string(month);
    output += "-";
    int day = timestamp_days.front();
    timestamp_days.pop_front();
    if (day < 10)
    {
        output += "0";
    }
    output += std::to_string(day);
    output += "T";
    int hour = timestamp_hours.front();
    timestamp_hours.pop_front();
    if (hour < 10)
    {
        output += "0";
    }
    output += std::to_string(hour);
    output += ":";
    int minute = timestamp_minutes.front();
    timestamp_minutes.pop_front();
    if (minute < 10)
    {
        output += "0";
    }
    output += std::to_string(minute);
    output += ":";
    int seconds = timestamp_seconds.front();
    timestamp_seconds.pop_front();
    if (seconds < 10)
    {
        output += "0";
    }
    output += std::to_string(seconds);
    output += "Z</timestamp>\n";
    output += contributor_generator.gen_str();
    if (has_minor.front())
    {
        output += "      <minor />\n";
    }
    has_minor.pop_front();
    if (has_comment.front())
    {
        output += "      <comment>";
        output += comments.front();
        comments.pop_front();
        output += "</comment>\n";
    }
    has_comment.pop_front();
    output += "      ";
    output += text_body_generator.gen_str();
    output += "\n    </revision>\n";
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
        std::regex title_re("    <title>(.*)</title>\n    <id>(\\d*)</id>\n");
        std::smatch title_m;
        if (!std::regex_search(before_text, title_m, title_re))
        {
            std::cout << "Title regex couldn't match properly for " << before_text << std::endl;
        }
        titles.push_back(title_m[1]);
        article_ids.push_back(stoi(title_m[2], nullptr, 10));
        std::regex restrictions_re("    <restrictions>(.*)</restrictions>\n");
        std::smatch restrictions_m;
        if (std::regex_search(before_text, restrictions_m, restrictions_re))
        {
            has_restriction.push_back(true);
            article_restrictions.push_back(restrictions_m[1]);
        }
        else
        {
            has_restriction.push_back(false);
        }
        std::regex revision_re("    <revision>\n      <id>(\\d*)</id>\n      <timestamp>(\\d\\d\\d\\d)-(\\d\\d)-(\\d\\d)T(\\d\\d):(\\d\\d):(\\d\\d)Z</timestamp>");
        std::smatch revision_m;
        if (!std::regex_search(before_text, revision_m, revision_re))
        {
            std::cout << "Revisions regex couldn't match properly for " << before_text << std::endl;
        }
        revision_ids.push_back(stoi(revision_m[1], nullptr, 10));
        timestamp_years.push_back(stoi(revision_m[2], nullptr, 10));
        timestamp_months.push_back(stoi(revision_m[3], nullptr, 10));
        timestamp_days.push_back(stoi(revision_m[4], nullptr, 10));
        timestamp_hours.push_back(stoi(revision_m[5], nullptr, 10));
        timestamp_minutes.push_back(stoi(revision_m[6], nullptr, 10));
        timestamp_seconds.push_back(stoi(revision_m[7], nullptr, 10));

        p = contributor_generator.find_range(before_text);
        if (p.first == -1)
        {
            std::cout << "No Contributor contained within page but expected contributor to be found" << std::endl;
        }
        contributor_generator.store_str(before_text.substr(p.first, p.second));
        std::string after_contributor = before_text.substr(p.first + p.second, before_text.length() - p.first - p.second);
        if (after_contributor.find("      <minor />") != std::string::npos)
        {
            has_minor.push_back(true);
        }
        else
        {
            has_minor.push_back(false);
        }
        std::regex comment_re("      <comment>(.*)</comment>\n");
        std::smatch comment_m;
        if (std::regex_search(after_contributor, comment_m, comment_re))
        {
            has_comment.push_back(true);
            comments.push_back(comment_m[1]);
        }
        else
        {
            has_comment.push_back(false);
        }
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