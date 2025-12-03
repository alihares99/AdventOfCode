#pragma once
#include "useful.h"

struct Day03 : public SolutionPair {

    static inline constexpr bool debug = false;

    auto get_input(const string& filename) {
        string f = filename;
        if (debug)
            f = "../test.txt";
        auto lines = get_input_all_lines(f);
        return lines;
    }

    template<int n>
    uint64_t get_largest_number(const string& line) {
        array<int, n> digits;
        int last = 0;
        for (int i = 0; i < n; i++) {
            digits[i] = max_element(line.begin() + last, line.end() - n + 1 + i) - line.begin();
            last = digits[i] + 1;
        }
        uint64_t num = 0;
        for (int i = 0; i < n; i++) {
            num *= 10;
            num += line[digits[i]] - '0';
        }
        return num;
    }

    pair<string, string> solve(const string& filename) override {
        auto inputs = get_input(filename);
        uint64_t part1 = 0;
        uint64_t part2 = 0;

        for (const auto& line : inputs) {
            part1 += get_largest_number<2>(line);
            part2 += get_largest_number<12>(line);
        }
        
        return {to_string(part1), to_string(part2)};
    }
};
