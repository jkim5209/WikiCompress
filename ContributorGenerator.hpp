#include "StringGenerator.hpp"
#include <queue>

class ContributorGenerator {
public:
    virtual std::string gen_str() override;
    virtual void store_str() override;
private:
    enum class ContributorType {
        ID_SPECIFIED, IP, CONVERSION_SCRIPT, TEMPLATE_NAMESPACE,
        MISC
    }
    struct IP {
        char nums[4];
    };
    static std::map<int, std::string> id_to_name;
    static std::queue<std::string> user_names;
    static std::queue<std::string> user_ids;
    static std::queue<ContributorType> types;
    static std::queue<IP> ips;
    static std::queue<std::string> misc;
}