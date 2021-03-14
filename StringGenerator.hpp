#pragma once
#include <string>

class ContextObject;

class StringGenerator {
public:
    virtual std::string gen_str() = 0;
    virtual void store_str(const std::string& str) = 0;
    // returns start and len of first substring it can process
    // pair returns -1, 0 if no match
    virtual std::pair<int, int> find_range(const std::string& str) = 0;
    //virtual void store_to_file(const std::string& filename) = 0;
    //virtual void load_from_file(const std::string& filename) = 0;
}; 
