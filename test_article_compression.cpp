#include "WikiArticleGenerator.hpp"
#include "ContributorGenerator.hpp"
#include "UrlGenerator.hpp"
//#include <cassert>
#include <fstream>
#include <iostream>
#include <streambuf>
#include <chrono>
using namespace std;

string compress(StringGenerator& gen, const string& str, const string& magic_token) {
    std::string reconstruction = "";
    std::string suffix_to_process = str;
    while (true)
    {
        auto p = gen.find_range(suffix_to_process);
        if (p.first == -1)
        {
            break;
        }
        gen.store_str(suffix_to_process.substr(p.first, p.second));
        reconstruction += suffix_to_process.substr(0, p.first);
        reconstruction += magic_token;
        suffix_to_process = suffix_to_process.substr(p.first + p.second, suffix_to_process.length() - p.first - p.second);
    }
    reconstruction += suffix_to_process;
    return reconstruction;
}

string decompress(StringGenerator& gen, string& reconstruction, const string& magic_token) {
    size_t last_idx = 0;
    vector<pair<size_t, size_t>> segments;
    string decompressed_str;
    while (true)
    {
        size_t replace_idx = reconstruction.find(magic_token, last_idx);
        if (replace_idx == std::string::npos)
        {
            break;
        }
        segments.push_back({last_idx, replace_idx - last_idx});
        last_idx = replace_idx + magic_token.size();
    }

    for (size_t i = 0; i < segments.size(); ++i) {
        decompressed_str += reconstruction.substr(segments[i].first, segments[i].second) +
                gen.gen_str();
    }

    decompressed_str += reconstruction.substr(last_idx,
        reconstruction.size() - last_idx);
    
    return decompressed_str;
}

int main(int argc, char** argv) {
    //assert(argc == 2);
    char* filename = argv[1];
    ifstream fin(filename);
    string str((istreambuf_iterator<char>(fin)),
        istreambuf_iterator<char>());

    vector<StringGenerator*> generators;
    vector<string> magic_tokens;
    generators.push_back(new WikiArticleGenerator());
    magic_tokens.push_back("WikiArticleGenerator");

    auto start = chrono::high_resolution_clock::now();
    string compressed_str = str;
    for (size_t i = 0; i < generators.size(); ++i) {
        compressed_str = compress(*generators[i], compressed_str, magic_tokens[i]);
        generators[i]->store_to_file(magic_tokens[i] + ".txt");
    }
    auto compression_stop = chrono::high_resolution_clock::now();
    cout << "took " << chrono::duration_cast<chrono::seconds>(compression_stop - start).count() << " to compress" << endl;

    std::ofstream out_compressed("compressed.txt");
    out_compressed << compressed_str;
    out_compressed.close();

    string decompressed_str = compressed_str;
    for (size_t i = 0; i < generators.size(); ++i) {
        decompressed_str = decompress(*generators[i], decompressed_str, magic_tokens[i]);
    }
    auto decompression_stop = chrono::high_resolution_clock::now();

    cout << "took " << chrono::duration_cast<chrono::seconds>(decompression_stop - compression_stop).count() << " to decompress" << endl;
    std::ofstream out_decompressed("decompressed.txt");
    out_decompressed << decompressed_str;
    out_decompressed.close();

    if (decompressed_str == str) {
        cout << "Test passed" << endl;
    } else {
        cout << "Test failed" << endl;
    }
}