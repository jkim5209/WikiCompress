#include "ContributorGenerator.hpp"
#include <regex>
#include <iostream>
#include <string>
//#include <cassert>

std::map<int, std::string> ContributorGenerator::id_to_name;
std::queue<std::string> ContributorGenerator::user_names;
std::queue<int> ContributorGenerator::user_ids;
std::queue<ContributorGenerator::ContributorType> ContributorGenerator::types;
std::queue<ContributorGenerator::IP> ContributorGenerator::ips;
std::queue<std::string> ContributorGenerator::misc;
const std::string ContributorGenerator::start_str = "      <contributor>\n        ";
const std::string ContributorGenerator::end_str = "      </contributor>\n";

std::string ContributorGenerator::gen_str() {
    std::string output = start_str;
    ContributorType t = types.front();
    types.pop();
    switch (t) {
        case ContributorType::ID_SPECIFIED: {
            int id = user_ids.front();
            user_ids.pop();
            auto it = id_to_name.find(id);
            output += "<username>";
            if (it == id_to_name.end()) {
                output += user_names.front();
                user_names.pop();
            } else {
                output += it->second;
            }
            output += "</username>\n        <id>" +
                std::to_string(id) + "</id>\n";
            break;
        }
        case ContributorType::IP:
            output += "<ip>";
            for (int i = 0; i < 3; ++i) {
                output += std::to_string(int(ips.front().nums[i])) + ".";
            }
            output += std::to_string(int(ips.front().nums[3])) + "</ip>\n";
            ips.pop();
            break;
        case ContributorType::CONVERSION_SCRIPT:
            output += "<ip>Conversion script</ip>\n";
            break;
        case ContributorType::TEMPLATE_NAMESPACE:
            output += "<ip>Template namespace initialisation script</ip>\n";
            break;
        case ContributorType::MISC:
            output += "<ip>" + misc.front() + "</ip>\n";
            misc.pop();
            break;
    }
    output += "      </contributor>\n";
    return output;
}

void ContributorGenerator::store_str(const std::string& str) {
    std::cout << str << std::endl;
    assert(str.find(start_str) == 0);
    std::regex user_re("<username>(.*)</username>\n        <id>(\\d+)</id>\n");
    std::smatch m;
    if (std::regex_search(str, m, user_re)) {
        std::cout << "this match" << std::endl;
        std::cout << m[2] << std::endl;
        int user_id = stoi(m[2], nullptr, 0);
        id_to_name[user_id] = m[1];
        user_ids.push(user_id);
        types.push(ContributorType::ID_SPECIFIED);
        return;
    }

    std::regex ip_re("<ip>(\\d).(\\d).(\\d).(\\d)</ip>\n");
    if (std::regex_search(str, m, ip_re)) {
        IP new_ip;
        for (int i = 0; i < 4; ++i) {
            new_ip.nums[i] = stoi(m[i + 1], nullptr, 0);
        }
        ips.push(new_ip);
        types.push(ContributorType::IP);
        return;
    }
    
    std::regex ip_conversion("<ip>Conversion Script</ip>\n");
    if (std::regex_search(str, m, ip_conversion)) {
        types.push(ContributorType::CONVERSION_SCRIPT);
        return;
    }

    std::regex ip_namespace("<ip>Template namespace initialisation script</ip>\n");
    if (std::regex_search(str, m, ip_namespace)) {
        types.push(ContributorType::TEMPLATE_NAMESPACE);
        return;
    }

    std::regex ip_misc("<ip>(.*)</ip>\n");
    if (std::regex_search(str, m, ip_misc)) {
        misc.push(m[1]);
        types.push(ContributorType::MISC);
        return;
    }

    std::cout << "NO regex match" << std::endl;
    throw "NO REGEX MATCHES!";

}

std::pair<int, int> ContributorGenerator::find_range(const std::string& str) {
    size_t start = str.find(start_str);
    size_t end = str.find(end_str);
    if (start == std::string::npos) {
        return {-1,0};
    }
    if (end == std::string::npos) {
        return {-1,0};
    }
    if (end < start) {
        return {-1,0};
    }
    return {start, end + end_str.size() - start};
}