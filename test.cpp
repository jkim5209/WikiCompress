#include "ContributorGenerator.hpp"
//#include <cassert>
#include <fstream>
#include <iostream>
#include <streambuf>
#include <chrono>
using namespace std;

int main(int argc, char** argv) {
    //assert(argc == 2);
    char* filename = argv[1];
    ifstream fin(filename);
    string str((istreambuf_iterator<char>(fin)),
        istreambuf_iterator<char>());
    auto start = chrono::high_resolution_clock::now();
    ContributorGenerator cg;
    std::string magic_token = "ContributorGenerator";
    std::string reconstruction = str;
    while (true)
    {
        auto p = cg.find_range(reconstruction);
        if (p.first == -1)
        {
            break;
        }
        cg.store_str(reconstruction.substr(p.first, p.second));
        reconstruction.replace(p.first, p.second, magic_token);
    }
    auto compression_stop = chrono::high_resolution_clock::now();
    cout << "took " << chrono::duration_cast<chrono::seconds>(compression_stop - start).count() << " to compress" << endl;

    while (true)
    {
        size_t replace_idx = reconstruction.find(magic_token);
        if (replace_idx == std::string::npos)
        {
            break;
        }
        reconstruction.replace(replace_idx, magic_token.length(), cg.gen_str());
    }
    auto decompression_stop = chrono::high_resolution_clock::now();
    cout << "took " << chrono::duration_cast<chrono::seconds>(decompression_stop - compression_stop).count() << " to decompress" << endl;

    cout << (reconstruction == str) << endl;
}