#include "StringGenerator.hpp"
#include <queue>

class UrlGenerator : public StringGenerator {
public:
    virtual std::string gen_str() override;
    virtual void store_str(const std::string& str) override;
    virtual std::pair<int, int> find_range(const std::string& str) override;
    virtual void store_to_file(const std::string& filename) override {}; // TODO implement
private:
    //enum class UrlSuffix {
    //    COM, NET, BIZ, ORG, ORG_UK
    //};
    //static std::vector<UrlSuffix> suffixes;

    static std::queue<bool> is_https;
    static std::queue<bool> has_www;
    static std::queue<std::string> url;
};