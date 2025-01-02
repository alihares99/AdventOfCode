#pragma once
#include "useful.h"

struct Day13 : public SolutionPair {

    struct Maze {
        vector<uint64_t> verticals;
        vector<uint64_t> horizotals;

        Maze(vector<string>::iterator begin, vector<string>::iterator end) {
            horizotals.reserve(end - begin);
            verticals.reserve(begin->size());
            for (auto it = begin; it != end; ++it) {
                uint64_t val = 0;
                for (auto c : *it) {
                    val = val << 1;
                    if (c == '#')
                        val = val | 1;
                }
                horizotals.push_back(val);
            }
            for (int i = int(begin->size()) - 1; i >= 0; i--) {
                uint64_t val = 0;
                uint64_t mask = uint64_t(1) << i;
                for (int j = 0; j < horizotals.size(); j++) {
                    val = val << 1;
                    if (horizotals[j] & mask)
                        val = val | 1;
                }
                verticals.push_back(val);
            }
        }
    };

    auto get_input(const string& filename) {
        vector<Maze> mazes;
        auto lines = get_input_all_lines(filename);
        auto begin = lines.begin();
        for (auto it = lines.begin(); it != lines.end(); ++it) {
            if (it->empty()) {
                mazes.emplace_back(begin, it);
                begin = next(it);
            }
        }
        if (begin != lines.end())
            mazes.emplace_back(begin, lines.end());
        return mazes;
    }

    struct EqMatcher {
        bool operator()(uint64_t a, uint64_t b) {
            return a == b;
        }
        bool operator()() {
            return true;
        }
    };

    struct SmudgeMatcher {
        bool found_smudge = false;
        bool operator()(uint64_t a, uint64_t b) {
            if (a == b)
                return true;
            if (!found_smudge) {
                found_smudge = std::popcount(a ^ b) == 1;
                return found_smudge;
            }
            return false;
        }
        bool operator()() {
            return found_smudge;
        }
    };

    template<class Matcher>
    int find_mirror(vector<uint64_t>& vec) {
        for (int i = 1; i < vec.size(); i++) {
            bool all_match = true;
            Matcher matcher;
            for (int j = 1; j < vec.size(); j++) {
                if (i - j < 0 || i + j - 1 >= vec.size())
                    break;
                if (!matcher(vec[i - j], vec[i + j - 1])) {
                    all_match = false;
                    break;
                }
            }
            if (all_match && matcher())
                return i;
        }
        return -1;
    }

    pair<string, string> solve(const string& filename) override {
        auto inputs = get_input(filename);
        uint64_t part1 = 0;
        uint64_t part2 = 0;

        for (auto& maze : inputs) {
            part1 += 100 * max(0, find_mirror<EqMatcher>(maze.horizotals)) + 
                    max(0, find_mirror<EqMatcher>(maze.verticals));
            part2 += 100 * max(0, find_mirror<SmudgeMatcher>(maze.horizotals)) + 
                    max(0, find_mirror<SmudgeMatcher>(maze.verticals));
        }
        
        return {to_string(part1), to_string(part2)};
    }
};
