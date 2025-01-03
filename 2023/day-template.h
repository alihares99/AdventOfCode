#pragma once
#include "useful.h"

struct DayXX : public SolutionPair {

    auto get_input(const string& filename) {
        auto lines = get_input_all_lines(filename);
        return lines;
    }

    pair<string, string> solve(const string& filename) override {
        auto inputs = get_input(filename);
        uint64_t part1 = 0;
        uint64_t part2 = 0;
        
        return {to_string(part1), to_string(part2)};
    }
};
