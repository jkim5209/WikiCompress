#include <vector>
#include <unordered_map>
#include <map>
#include <string>
#include <iostream>
#include <fstream>
#include <cassert>
#include <math.h>
#include <ctype.h>

//#define NDEBUG

using namespace std;

using token_data = pair<int, int>;

void generate_tokens_with_jumps(const vector<int> token_list, 
        map<string, int>& token_map, map<int, string>& id_map, 
        int token_id, int num_jumps);

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
    map<string, int> token_map;
    map<int, string> id_map;
    int token_id = 0;
    #ifndef NDEBUG
    string total_string;
    #endif
    double init_bytes = 0;
    {
        ifstream fin(filename);
        char c;
        while (fin >> c) {
            c = tolower(c);
            #ifndef NDEBUG
            total_string += c;
            #endif

            init_bytes += 1;
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
        //map<pair<int, int>, int, hash_pair> pair_map;
        map<pair<int, int>, int> pair_map;
        int i = 0;
        map<int, int> token_count;
        while (true) {
            ++token_count[tok_list[i].first];
            int next_idx = tok_list[i].second + i;
            if (next_idx >= tok_list.size()) break;

            ++pair_map[make_pair(tok_list[i].first, tok_list[next_idx].first)];
            i = next_idx;
        }

        double max_score = 0;
        pair<int, int> max_pair;
        for (const auto& p : pair_map) {
            //if (token_count[p.first.first] == p.second && token_count[p.first.second] == p.second) {
            //    max_count = p.second;
            //    max_pair = p.first;
            //    cout << "found token to remove!\n";
            //    break;
            //}
            double score = double(p.second);
            if (score > max_score) {
                max_score = score;
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

            i += tok_list[i].second;
        }
        ++token_id;

        double obj_fun = 0;
        for (const auto& p : token_map) {
            obj_fun += double(p.first.length());
        }
        obj_fun += token_map.size();
        obj_fun += num_toks * log2(token_map.size()) / 8;

        //cout << "obj func " << obj_fun << "\n";
        cout << "compression " << obj_fun / init_bytes << " max score " << max_score << "\n";
        //if (prev_objs_fun < obj_fun) break;
        if (max_score < 16) break;
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
    //ofstream fout("dictionary.txt");
    //for (auto it = count_string.rbegin(); it != count_string.rend(); ++it) {
    //    fout << it->first << " " << it->second << "\n";
    //}

    

    string test_string;
    for (int token : clean_tok_list) {
        test_string += id_map[token];
    }

    #ifndef NDEBUG
    if (total_string == test_string) {
        cout << "assert passed!" << endl;
    } else {
        cout << "assert failed!" << endl;

    }
    #endif

    generate_tokens_with_jumps(clean_tok_list, token_map, id_map, token_id, 1);
}

void generate_tokens_with_jumps(const vector<int> token_list, 
        map<string, int>& token_map, map<int, string>& id_map, 
        int token_id, int num_jumps) {
    cout.precision(15);
    vector<token_data> tok_list;
    for (int token : token_list) {
        tok_list.emplace_back(token, 1);
    }

    #ifndef NDEBUG
    string total_string;
    #endif

    double prev_objs_fun = 1e20;

    while (true) {
        //map<pair<int, int>, int, hash_pair> pair_map;
        map<pair<int, int>, int> pair_map;
        int i = 0;
        int jumped_idx = 0;
        for (int j = 0; j < num_jumps + 1; ++j) {
            jumped_idx += tok_list[jumped_idx].second;
        }
        while (true) {
            if (jumped_idx >= tok_list.size()) break;

            ++pair_map[make_pair(tok_list[i].first, tok_list[jumped_idx].first)];
            i = i + tok_list[i].second;
            jumped_idx += tok_list[jumped_idx].second;
        }

        double max_score = 0;
        pair<int, int> max_pair;
        for (const auto& p : pair_map) {
            //if (token_count[p.first.first] == p.second && token_count[p.first.second] == p.second) {
            //    max_count = p.second;
            //    max_pair = p.first;
            //    cout << "found token to remove!\n";
            //    break;
            //}
            double score = double(p.second);
            if (score > max_score) {
                max_score = score;
                max_pair = p.first;
            }
        }

        string token_str = id_map[max_pair.first];
        for (int i = 0; i < num_jumps; ++i) {
            token_str += '*';
        }
        token_str += id_map[max_pair.second];
        token_map[token_str] = token_id;
        id_map[token_id] = token_str;

        double num_toks = 1;
        i = 0;
        int before_jumped_idx = 0;
        for (int j = 0; j < num_jumps; ++j) {
            before_jumped_idx += tok_list[before_jumped_idx].second;
        }
        jumped_idx = before_jumped_idx + tok_list[before_jumped_idx].second;
        while (true) {
            if (jumped_idx >= tok_list.size())
                break;


            if (tok_list[i].first == max_pair.first && tok_list[jumped_idx].first == max_pair.second) {
                tok_list[i].first = token_id;
                tok_list[before_jumped_idx].second += tok_list[jumped_idx].second;
                jumped_idx += tok_list[jumped_idx].second;
            }
            ++num_toks;

            i += tok_list[i].second;
            before_jumped_idx += tok_list[before_jumped_idx].second;
            jumped_idx += tok_list[jumped_idx].second;
        }

        ++token_id;

        double obj_fun = 0;
        for (const auto& p : token_map) {
            obj_fun += double(p.first.length());
        }
        obj_fun += token_map.size();
        obj_fun += num_toks * log2(token_map.size()) / 8;

        //cout << "obj func " << obj_fun << "\n";
        cout << "1jump compression " << obj_fun << " max score " << max_score << " " << token_str << "\n";
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
    ofstream fout("dictionary.txt");
    for (auto it = count_string.rbegin(); it != count_string.rend(); ++it) {
        fout << it->first << " " << it->second << "\n";
    }

    string test_string;
    for (int token : clean_tok_list) {
        test_string += id_map[token];
    }

    #ifndef NDEBUG
    if (total_string == test_string) {
        cout << "assert passed!" << endl;
    } else {
        cout << "assert failed!" << endl;

    }
    #endif
}

int main() {
    generate_tokens("enwik_10000");
    return 0;
}
