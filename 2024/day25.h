#pragma once
#include "useful.h"

struct Day25 : public Solution {
    auto get_input(const string& filename) {
        auto lines = get_input_all_lines(filename);
        vector<array<int, 5>> locks;
        vector<array<int, 5>> keys;
        
        
        for (int i = 0; i < lines.size(); i += 8) {
            bool is_lock = lines[i][0] == '#';
            array<int, 5> heights {0};
            for (int j = 1; j < 6; j++) {
                for (int k = 0; k < 5; k++) {
                    if (lines[i + j][k] == '#')
                        heights[k]++;
                }
            }
            if (is_lock)
                locks.push_back(heights);
            else
                keys.push_back(heights);
        }
        return make_pair(locks, keys);
    }

    string part1(const string& filename) override {
        auto [locks, keys] = get_input(filename);
        int count = 0;
        for (int i = 0; i < locks.size(); i++) {
            for (int j = 0 ; j < keys.size(); j++) {
                bool match = true;
                for (int k = 0; k < 5; k++) {
                    if (locks[i][k] + keys[j][k] > 5) {
                        match = false;
                        break;
                    }
                }

                if (match) {
                    count++;
                } 
            }
        }

        return to_string(count);
    }

    string part2(const string& filename) override {
        return "Challenge Completed!";
    }
};
