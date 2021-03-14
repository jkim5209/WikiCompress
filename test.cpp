#include "WikiArticleGenerator.hpp"
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
    WikiArticleGenerator wag;
    std::string magic_token = "WikiArticleGenerator";
    std::string reconstruction = "";
    std::string suffix_to_process = str;
    while (true)
    {
        auto p = wag.find_range(suffix_to_process);
        if (p.first == -1)
        {
            break;
        }
        wag.store_str(suffix_to_process.substr(p.first, p.second));
        reconstruction += suffix_to_process.substr(0, p.first);
        reconstruction += magic_token;
        suffix_to_process = suffix_to_process.substr(p.first + p.second, suffix_to_process.length() - p.first - p.second);
    }
    reconstruction += suffix_to_process;
    wag.store_to_file("contributor_generator.txt");
    auto compression_stop = chrono::high_resolution_clock::now();
    cout << "took " << chrono::duration_cast<chrono::seconds>(compression_stop - start).count() << " to compress" << endl;
    std::ofstream out_compressed("compressed.txt");
    out_compressed << reconstruction;
    out_compressed.close();
    while (true)
    {
        size_t replace_idx = reconstruction.find(magic_token);
        if (replace_idx == std::string::npos)
        {
            break;
        }
        reconstruction.replace(replace_idx, magic_token.length(), wag.gen_str());
    }
    auto decompression_stop = chrono::high_resolution_clock::now();
    cout << "took " << chrono::duration_cast<chrono::seconds>(decompression_stop - compression_stop).count() << " to decompress" << endl;
    std::ofstream out_decompressed("decompressed.txt");
    out_decompressed << reconstruction;
    out_decompressed.close();
    cout << (reconstruction == str) << endl;
}