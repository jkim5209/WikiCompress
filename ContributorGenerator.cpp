#include "ContributorGenerator.hpp"
#include <regex>
#include <iostream>
#include <string>
//#include <cassert>

std::map<int, std::string> ContributorGenerator::id_to_name;
std::deque<std::string> ContributorGenerator::user_names;
std::deque<int> ContributorGenerator::user_ids;
std::deque<ContributorGenerator::ContributorType> ContributorGenerator::types;
std::deque<ContributorGenerator::IP> ContributorGenerator::ips;
std::deque<std::string> ContributorGenerator::misc;
const std::string ContributorGenerator::start_str = "      <contributor>\n        ";
const std::string ContributorGenerator::end_str = "      </contributor>\n";

std::string ContributorGenerator::gen_str() {
    std::string output = start_str;
    ContributorType t = types.front();
    types.pop_front();
    switch (t) 
    {
        case ContributorType::ID_SPECIFIED: {
            int id = user_ids.front();
            user_ids.pop_front();
            auto it = id_to_name.find(id);
            output += "<username>";
            if (it == id_to_name.end()) 
            {
                output += user_names.front();
                user_names.pop_front();
            } 
            else 
            {
                output += it->second;
            }
            output += "</username>\n        <id>" +
                std::to_string(id) + "</id>\n";
            break;
        }
        case ContributorType::IP:
            output += "<ip>";
            for (int i = 0; i < 3; ++i) 
            {
                output += std::to_string(int(ips.front().nums[i])) + ".";
            }
            output += std::to_string(int(ips.front().nums[3])) + "</ip>\n";
            ips.pop_front();
            break;
        case ContributorType::CONVERSION_SCRIPT:
            output += "<ip>Conversion script</ip>\n";
            break;
        case ContributorType::TEMPLATE_NAMESPACE:
            output += "<ip>Template namespace initialisation script</ip>\n";
            break;
        case ContributorType::MISC:
            output += "<ip>" + misc.front() + "</ip>\n";
            misc.pop_front();
            break;
    }
    output += "      </contributor>\n";
    return output;
}

void ContributorGenerator::store_str(const std::string& str) {
    assert(str.find(start_str) == 0);
    std::regex user_re("<username>(.*)</username>\n        <id>(\\d+)</id>\n");
    std::smatch m;
    if (std::regex_search(str, m, user_re)) 
    {
        int user_id = stoi(m[2], nullptr, 0);
        std::string user_name = m[1];
        user_ids.push_back(user_id);
        user_names.push_back(user_name);
        types.push_back(ContributorType::ID_SPECIFIED);
        return;
    }

    std::regex ip_re("<ip>(\\d).(\\d).(\\d).(\\d)</ip>\n");
    if (std::regex_search(str, m, ip_re)) 
    {
        IP new_ip;
        for (int i = 0; i < 4; ++i) 
        {
            new_ip.nums[i] = stoi(m[i + 1], nullptr, 0);
        }
        ips.push_back(new_ip);
        types.push_back(ContributorType::IP);
        return;
    }
    
    std::regex ip_conversion("<ip>Conversion Script</ip>\n");
    if (std::regex_search(str, m, ip_conversion)) 
    {
        types.push_back(ContributorType::CONVERSION_SCRIPT);
        return;
    }

    std::regex ip_namespace("<ip>Template namespace initialisation script</ip>\n");
    if (std::regex_search(str, m, ip_namespace)) 
    {
        types.push_back(ContributorType::TEMPLATE_NAMESPACE);
        return;
    }

    std::regex ip_misc("<ip>(.*)</ip>\n");
    if (std::regex_search(str, m, ip_misc)) 
    {
        misc.push_back(m[1]);
        types.push_back(ContributorType::MISC);
        return;
    }

    std::cout << "NO regex match for Contributor" << std::endl;
    throw "NO REGEX MATCHES!";

}

std::pair<int, int> ContributorGenerator::find_range(const std::string& str)
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

void ContributorGenerator::store_to_file(const std::string& filename)
{
    int USER_ID_MAP_THRESHOLD = 1;
    // Iterate through and remove usernames from the list that occur multiple times
    std::map<int, int> id_to_count_map;
    for (auto it = user_ids.cbegin(); it != user_ids.cend(); ++it)
    {
        id_to_count_map[*it] += 1;
    }
    std::map<std::string, std::string> usernames_to_remove;
    for (int i = 0; i < user_ids.size(); i++)
    {
        int user_id = user_ids[i];
        int user_id_count = id_to_count_map[user_id];
        // 216 is ignored because the id has multiple associated usernames, Larry Sanger, Larry_Sanger, so ambiguous which one to use when
        if (user_id_count > USER_ID_MAP_THRESHOLD && user_id != 216)
        {
            id_to_name[user_id] = user_names[i];
            usernames_to_remove[user_names[i]] = user_names[i];
        }
    }

    auto it = user_names.cbegin();
    while(it != user_names.cend())
    {
        if (usernames_to_remove.find(*it) != usernames_to_remove.cend())
        {
            it = user_names.erase(it);
        }
        else
        {
            ++it;
        }
    }

}