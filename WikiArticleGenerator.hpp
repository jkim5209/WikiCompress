#include "StringGenerator.hpp"
#include "TextBodyGenerator.hpp"
#include <queue>
#include <map>

class WikiArticleGenerator : public StringGenerator {
public:
    virtual std::string gen_str() override;
    virtual void store_str(const std::string& str) override;
    virtual std::pair<int, int> find_range(const std::string& str) override;
    virtual void store_to_file(const std::string& filename) override;
private:
    // enum class ContributorType {
    //     ID_SPECIFIED, IP, CONVERSION_SCRIPT, TEMPLATE_NAMESPACE,
    //     MISC
    // };

    // struct IP {
    //     char nums[4];
    // };
    static TextBodyGenerator text_body_generator;
    static std::deque<std::string> misc;
    static const std::string start_str;
    static const std::string end_str;
};