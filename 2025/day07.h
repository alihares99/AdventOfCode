#pragma once
#include "useful.h"

struct Day07 : public SolutionPair {

    static inline constexpr bool debug = false;

    auto get_input(const string& filename) {
        string f = filename;
        if (debug)
            f = "../test.txt";
        auto lines = get_input_all_lines(f);
        return lines;
    }

    pair<string, string> solve(const string& filename) override {
        auto grid = get_input(filename);
        uint64_t part1 = 0;
        uint64_t part2 = 0;

        int rows = grid.size();
        int cols = grid[0].size();
        vector<uint64_t> beams(cols);
        for (int i = 0; i < rows; i += 2) {
            vector<uint64_t> next = beams;
            for (int j = 0; j < cols; j++) {
                switch (grid[i][j])
                {
                case 'S':
                    next[j] = 1;
                    break;
                case '^':
                    if (beams[j]) {
                        part1++;
                        next[j - 1] += next[j];
                        next[j + 1] += next[j];
                        next[j] = 0;
                    }
                    break;
                }
            }
            beams = std::move(next);
        }
        part2 = ranges::fold_left(beams, 0, plus<>()); // accumulate

        
        return {to_string(part1), to_string(part2)};
    }
};
