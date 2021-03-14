#include "StringGenerator.hpp"
#include <queue>
#include <map>

class ContributorGenerator : public StringGenerator {
public:
    virtual std::string gen_str() override;
    virtual void store_str(const std::string& str) override;
    virtual std::pair<int, int> find_range(const std::string& str) override;
private:
    enum class ContributorType {
        ID_SPECIFIED, IP, CONVERSION_SCRIPT, TEMPLATE_NAMESPACE,
        MISC
    };

    struct IP {
        char nums[4];
    };
    static std::map<int, std::string> id_to_name;
    static std::queue<std::string> user_names;
    static std::queue<int> user_ids;
    static std::queue<ContributorType> types;
    static std::queue<IP> ips;
    static std::queue<std::string> misc;
    static const std::string start_str;
    static const std::string end_str;
};