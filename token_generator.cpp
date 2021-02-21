#include <vector>
#include <unordered_map>
#include <string>
#include <iostream>
#include <fstream>
#include <cassert>
#include <math.h>

using namespace std;

using token_data = pair<int, int>;

struct hash_pair {
    template <typename T1, typename T2>
    inline size_t operator() (const pair<T1, T2>&p) const noexcept {
        auto hash1 = hash<T1>{}(p.first);
        auto hash2 = hash<T2>{}(p.first);
        return hash1 ^ hash2;
    }
};

void generate_tokens(const string& filename) {
    cout.precision(15);
    vector<token_data> tok_list;
    unordered_map<string, int> token_map;
    unordered_map<int, string> id_map;
    int token_id = 0;
    string total_string;
    {
        ifstream fin(filename);
        char c;
        while (fin >> c) {
            total_string += c;
            string c_in_string = string(1,c);
            auto it = token_map.find(c_in_string);
            if (it == token_map.end()) {
                token_map[c_in_string] = token_id;
                id_map[token_id] = c_in_string;
                tok_list.emplace_back(token_id, 1);
                ++token_id;
            } else {
                tok_list.emplace_back(it->second, 1);
            }
        }
    }

    double prev_objs_fun = 1e20;

    while (true) {
        unordered_map<pair<int, int>, int, hash_pair> pair_map;
        int i = 0;
        while (true) {
            int next_idx = tok_list[i].second + i;
            if (next_idx >= tok_list.size()) break;

            ++pair_map[make_pair(tok_list[i].first, tok_list[next_idx].first)];
            i = next_idx;
        }

        int max_count = 0;
        pair<int, int> max_pair;
        for (const auto& p : pair_map) {
            if (p.second > max_count) {
                max_count = p.second;
                max_pair = p.first;
            }
        }
        string token_str = id_map[max_pair.first] + id_map[max_pair.second];
        token_map[token_str] = token_id;
        id_map[token_id] = token_str;

        double num_toks = 1;
        i = 0;
        while (true) {
            int next_idx = tok_list[i].second + i;
            if (next_idx >= tok_list.size())
                break;

            if (tok_list[i].first == max_pair.first && tok_list[next_idx].first == max_pair.second) {
                tok_list[i].first = token_id;
                tok_list[i].second += tok_list[next_idx].second;
            }
            ++num_toks;

            i = next_idx;
        }
        ++token_id;

        double obj_fun = 0;
        for (const auto& p : token_map) {
            obj_fun += double(p.first.length());
        }
        obj_fun += token_map.size();
        obj_fun += num_toks * log2(token_map.size());

        cout << "obj func " << obj_fun << "\n";
        if (prev_objs_fun < obj_fun) break;
        prev_objs_fun = obj_fun;
    }

    vector<int> clean_tok_list;
    int i = 0;
    while (true) {
        clean_tok_list.push_back(tok_list[i].first);
        i += tok_list[i].second;
        if (i >= tok_list.size())
            break;
    }

    unordered_map<int, int> tok_counter;
    for (int id : clean_tok_list) {
        ++tok_counter[id];
    }
    vector<pair<int, string>> count_string;
    for (auto p : tok_counter) {
        count_string.emplace_back(p.second, id_map[p.first]);
    }
    sort(count_string.begin(), count_string.end());
    for (auto it = count_string.rbegin(); it != count_string.rend(); ++it) {
        cout << it->first << " " << it->second << "\n";
    }

    string test_string;
    for (int token : clean_tok_list) {
        test_string += id_map[token];
    }

    assert(total_string == test_string);
}

int main() {
    generate_tokens("enwik_1000");
    return 0;
}