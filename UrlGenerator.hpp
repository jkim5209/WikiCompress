#include "StringGenerator.hpp"

class UrlGenerator {
public:
    virtual std::string gen_str() override;
    virtual void store_str() override;
private:
    //enum class UrlSuffix {
    //    COM, NET, BIZ, ORG, ORG_UK
    //};
    //static std::vector<UrlSuffix> suffixes;

    static size_t idx = 0;
    static std::vector<bool> is_https;
    static std::vector<bool> has_www;
    static std::vector<std::string> url;
};