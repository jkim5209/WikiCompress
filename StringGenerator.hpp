#pragma once
#include <string>

class ContextObject;

class StringGenerator {
public:
    virtual std::string gen_str(const ContextObject& ctx) = 0;
    virtual void store_string(std::string str, const ContextObject& ctx) = 0;
}; 
