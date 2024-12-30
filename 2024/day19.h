#pragma once
#include "useful.h"

struct Day19 : public Solution {
    set<string> towels;
    vector<string> designs;
    optional<pair<int, uint64_t>> result;

    void get_input(const string& filename) {
        if (!towels.empty())
            return;

        auto lines = get_input_all_lines(filename);
        
        istringstream ss(lines[0]);
        string towel;
        while (getline(ss, towel, ',')) {
            towels.insert(towel);
            ss.ignore();
        }
        
        for (int i = 2; i < lines.size(); i++) {
            designs.push_back(lines[i]);
        }
    }

    uint64_t is_possible(const set<string>& towels, string& design) {
        vector<uint64_t> dp(design.size(), 0);
        for (int i = 0; i < design.size(); i++) {
            if (i > 0 && dp[i - 1] == 0) 
                continue;
            
            uint64_t n_ways_so_far = i > 0 ? dp[i - 1] : 1;
            char c = design[i];
            for (auto towel_it = towels.lower_bound(string({c})); 
                towel_it != towels.end() && towel_it->at(0) == c; ++towel_it
                ) {
                int n = i + int(towel_it->size());
                if (n > design.size()) {
                    continue;
                }
                string_view sub_design(design.begin() + i, design.begin() + n);
                if (sub_design == *towel_it) {
                    dp[n - 1] += n_ways_so_far;
                }
            }
        }
        
        return dp.back();
    }

    void solve(const string& filename) {
        if (result.has_value())
            return;

        get_input(filename);
        int count_possible = 0;
        uint64_t n_possibility = 0;
        for (auto& design : designs) {
            auto res = is_possible(towels, design);
            if (res > 0)
                count_possible++;
            n_possibility += res;
        }
        result = {count_possible, n_possibility};
    }

    string part1(const string& filename) override {
        solve(filename);
        return to_string(result->first);
    }

    string part2(const string& filename) override {
        solve(filename);
        return to_string(result->second);
    }
};
