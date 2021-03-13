#include "ContributorGenerator.hpp"

std::string ContributorGenerator::gen_str() {
    std::string output = "      <contributor>\n        "
    ContributorType t = types.front();
    types.pop();
    switch (t) {
        case ID_SPECIFIED:
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
        case IP:
            output += "<ip>";
            IP ip_num = ips.front();
            ips.pop();
            for (int i = 0; i < 3; ++i) {
                output += std::to_string(int(ip_num[i])) + ".";
            }
            output += std::to_string(int(ip_num[3])) + "</ip>\n";
            break;
        case CONVERSION_SCRIPT:
            output += "<ip>Conversion script</ip>\n"
            break;
        case TEMPLATE_NAMESPACE:
            output += "<ip>Template namespace initialisation script</ip>\n"
            break;
        case MISC:
            output += "<ip>" + misc.front() + "</ip>\n";
            misc.pop();
            break;
    }
    output += "      </contributor>\n";
    return output;
}