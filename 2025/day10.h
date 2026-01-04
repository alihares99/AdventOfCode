#pragma once
#include "useful.h"

struct Day10 : public SolutionPair {

    static inline constexpr bool debug = true;


    struct Item {
        vector<bool> lights;
        vector<vector<int>> buttons;
        vector<int> joltage_req;
    };

    auto get_input(const string& filename) {
        string f = filename;
        if (debug)
            f = "../test.txt";
        auto lines = get_input_all_lines(f);
        vector<Item> items;
        for (const auto& line : lines) {
            items.emplace_back();
            auto& item = items.back();
            istringstream ss(line);
            char c;
            int i;
            ss >> c;
            while (ss >> c, c != ']') {
                item.lights.push_back(c == '#');
            }
            while (ss >> c && c == '(') {
                item.buttons.emplace_back();
                auto& buttons = item.buttons.back();
                while (ss >> i) {
                    buttons.push_back(i);
                    ss >> c;
                }
                ss.clear();
            }
            while (ss >> i) {
                item.joltage_req.push_back(i);
                ss >> c;
            }
        }
        return items;
    }

    pair<string, string> solve(const string& filename) override {
        auto items = get_input(filename);
        uint64_t part1 = 0;
        uint64_t part2 = 0;
        
        return {to_string(part1), to_string(part2)};
    }
};
