#pragma once
#include "useful.h"

struct Day01 : public Solution {
    vector<int> l, r;

    void get_input(const string& filename) {
        if (l.size() != 0)
            return;

        auto lines = get_input_all_lines(filename);
        l.resize(lines.size());
        r.resize(lines.size());
        for (size_t i = 0; i < lines.size(); i++) {
            istringstream ss(lines[i]);
            ss >> l[i] >> r[i];
        }
    }

    string part1(const string& filename) override {
        get_input(filename);
        sort(l.begin(), l.end());
        sort(r.begin(), r.end());

        int diff = 0;
        for (int i = 0; i < l.size(); i++) {
            diff += abs(l[i] - r[i]);
        }

        return to_string(diff);
    }

    string part2(const string& filename) override {
        get_input(filename);
        uint64_t count_2 = 0;
        for (int item : l) {
            auto [it1, it2] = equal_range(r.begin(), r.end(), item);
            count_2 += item * (it2 - it1);
        }

        return to_string(count_2);
    }
};
