#include "ContributorGenerator.hpp"
//#include <cassert>
#include <fstream>
#include <iostream>
#include <streambuf>

using namespace std;

int main(int argc, char** argv) {
    //assert(argc == 2);
    char* filename = argv[1];
    ifstream fin(filename);
    string str((istreambuf_iterator<char>(fin)),
        istreambuf_iterator<char>());

    ContributorGenerator cg;
    auto p = cg.find_range(str);
    string left = str.substr(0, p.first);
    string right = str.substr(p.first + p.second, str.length() - p.first - p.second);
    cout << p.first << " " << p.second << endl;
    cout << str.substr(p.first, p.second) << endl;

    cg.store_str(str.substr(p.first, p.second));

    string reconstruction = left + cg.gen_str() + right;
    cout << (reconstruction == str) << endl;
}