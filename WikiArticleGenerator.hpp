#include "StringGenerator.hpp"
#include "TextBodyGenerator.hpp"
#include "ContributorGenerator.hpp"
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
    static ContributorGenerator contributor_generator;
    static TextBodyGenerator text_body_generator;
    static std::deque<std::string> titles;
    static std::deque<int> article_ids;
    static std::deque<bool> has_restriction;
    static std::deque<std::string> article_restrictions;
    static std::deque<int> revision_ids;
    static std::deque<int> timestamp_years;
    static std::deque<int> timestamp_months;
    static std::deque<int> timestamp_days;
    static std::deque<int> timestamp_hours;
    static std::deque<int> timestamp_minutes;
    static std::deque<int> timestamp_seconds;
    static std::deque<bool> has_minor;
    static std::deque<bool> has_comment;
    static std::deque<std::string> comments;
    static const std::string start_str;
    static const std::string end_str;
};