#pragma once
#include "useful.h"

struct Day02 : public SolutionPair {

    static inline constexpr bool debug = false;

    auto get_input(const string& filename) {
        string f = filename;
        if (debug)
            f = "../test.txt";
        auto lines = get_input_all_lines(f);

        vector<pair<uint64_t, uint64_t>> nums;
        istringstream ss(lines[0]);
        uint64_t v1, v2;
        char c;
        while (!ss.fail()) {
            ss >> v1 >> c >> v2;
            nums.push_back(make_pair(v1, v2));
            ss >> c;
        }
        return nums;
    }

    uint64_t get_multiplier(auto pattern, int repeat) {
        uint64_t out = 1;
        int n_digits = get_n_digits(pattern);
        uint64_t power = pow(10, n_digits); 
        for (int i = 1; i < repeat; i++) {
            out *= power;
            out += 1;
        }
        return out;
    }

    uint64_t get_pattern(auto num, int repeat) {
        int n_digit = get_n_digits(num);
        int n = n_digit / repeat;
        if (n_digit % repeat == 0) {
            return num / uint64_t(pow(10, n_digit - n));
        } else {
            return uint64_t(pow(10, n));
        }
    }

    pair<string, string> solve(const string& filename) override {
        auto inputs = get_input(filename);
        uint64_t part1 = 0;
        uint64_t part2 = 0;

        for (auto [a, b] : inputs) {
            unordered_set<uint64_t> s;
            for (int i = 2; i <= get_n_digits(b); i++) {
                auto lo = a;
                uint64_t pattern = get_pattern(lo, i);
                lo = pattern * get_multiplier(pattern, i);
                while (lo <= b) {
                    if (a <= lo && !s.contains(lo)) {
                        if (i == 2)
                            part1 += lo;
                        part2 += lo;
                        s.insert(lo);
                    }
                    pattern++;
                    lo = pattern * get_multiplier(pattern, i);
                }
            }
        }
        
        return {to_string(part1), to_string(part2)};
    }

    // 28846518423
};
