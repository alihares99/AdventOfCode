#pragma once
#include "useful.h"

struct Day21 : public Solution {

    using Table_t = unordered_map<pair<char, char>, int64_t>;

    struct KeyPad {
        int n_row;
        int n_col;
        vector<string> chars;
        unordered_map<char, pair<int, int>> indexes;
        string all;

        KeyPad(vector<string>&& chars_) : 
            n_row(int(chars_.size())), 
            n_col(int(chars_[0].size())), 
            chars(move(chars_)) 
        {
            all.reserve(n_row * n_col);
            indexes.reserve(n_row * n_col);
            for (int i = 0; i < n_row; i++)
                for (int j = 0; j < n_col; j++) 
                    if (chars[i][j] != ' ') {
                        indexes[chars[i][j]] = make_pair(i, j);
                        all.push_back(chars[i][j]);
                    }
        }

        vector<string> get_path(char a, char b) {
            if (a == b)
                return {"A"};
            
            auto [i1, j1] = indexes[a];
            auto [i2, j2] = indexes[b];
            char c_horiz = j2 > j1 ? '>' : '<';
            char c_vertic = i2 > i1 ? 'v' : '^';
            string horiz (abs(j2 - j1), c_horiz);
            string vertic (abs(i2 - i1), c_vertic);
            if (i1 == i2)
                return {horiz + "A"};
            if (j1 == j2) 
                return {vertic + "A"};

            vector<string> res;
            if (chars[i1][j2] != ' ')
                res.push_back(horiz + vertic + "A");
            if (chars[i2][j1] != ' ')
                res.push_back(vertic + horiz + "A");
            return res;
        }

        Table_t calc_table(Table_t& table) {
            Table_t table2;
            for (auto a : all) {
                for (auto b : all) {
                    char prev = 'A';
                    auto paths = get_path(a, b);
                    int64_t minimum = numeric_limits<int64_t>::max();
                    for (auto& path : paths) {
                        int64_t cur = 0;
                        for (auto c : path) {
                            cur += table[{prev, c}];
                            prev = c;
                        }

                        minimum = min(minimum, cur);
                    }

                    table2[{a, b}] = minimum;
                }
            }

            return table2;
        }
    };

    KeyPad numbers = KeyPad({"789", "456", "123", " 0A"});
    KeyPad arrows = KeyPad({" ^A", "<v>"});

    Table_t calc_table(int n_arrows_table) {
        Table_t table;
        for (auto a : arrows.all) {
            for (auto b : arrows.all) {
                table[{a, b}] = 1;
            }
        }

        for (int i = 0; i < n_arrows_table; i++) 
            table = arrows.calc_table(table);
        table = numbers.calc_table(table);

        return table;
    }

    int64_t calc_complexity(const string& str, Table_t& table) {
        int64_t res = 0;
        char prev = 'A';
        for (char c : str) {
            res += table[{prev, c}];
            prev = c;
        }

        int64_t v;
        istringstream ss(str);
        ss >> v;
        
        return res * v;
    }

    int64_t sum_complexities(const string& filename, int n_arrows_table) {
        Table_t table = calc_table(n_arrows_table);

        int64_t res = 0;
        auto input = get_input_all_lines(filename);
        for (auto& str : input) {
            res += calc_complexity(str, table);
        }

        return res;
    }

    string part1(const string& filename) override {
        return to_string(sum_complexities(filename, 2));
    }

    string part2(const string& filename) override {
        return to_string(sum_complexities(filename, 25));
    }
};
