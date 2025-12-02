#pragma once
#include "useful.h"

struct Day01 : public SolutionPair {

    static inline constexpr bool debug = false;

    auto get_input(const string& filename) {
        string f = filename;
        if (debug)
            f = "../test.txt";
        auto lines = get_input_all_lines(f);
        return lines;
    }

    pair<int, int> calc(int& current, char c, int count) {
        auto input = current;
        if (c == 'L') {
            current -= count;
        } else {
            current += count;
        }
        int p2 = (abs(current) / 100) + (input * current < 0 || current == 0 ? 1 : 0);
        current = current % 100;
        current = current < 0 ? current + 100 : current;
        int p1 = current == 0 ? 1 : 0;
        return make_pair(p1, p2);
    }

    pair<string, string> solve(const string& filename) override {
        auto inputs = get_input(filename);
        int part1 = 0;
        int part2 = 0;

        int current = 50;
        for (const string& line: inputs) {
            auto [p1, p2] = calc(current, line[0], stoi(line.substr(1, -1)));
            part1 += p1;
            part2 += p2;
        }
        
        return {to_string(part1), to_string(part2)};
    }
};
