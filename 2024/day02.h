#pragma once
#include "useful.h"

struct Day02 : public Solution {

    vector<vector<int>> input;
    
    template <class Condition>
    bool check(const vector<int>& vec, Condition cond, bool check_skip) {
        set<int> skip_candidate;
        for (int i = 0; i < vec.size() - 1; i++) {
            if (!cond(vec[i], vec[i + 1])) {
                if (!check_skip) 
                    return false;

                bool can_skip_i = (i == 0) || cond(vec[i - 1], vec[i + 1]);
                bool can_skip_i_1 = (i == vec.size() - 2) || cond(vec[i], vec[i + 2]);

                if (!can_skip_i && !can_skip_i_1)
                    return false;
                if (skip_candidate.size() == 0) {
                    if (can_skip_i) skip_candidate.insert(i);
                    if (can_skip_i_1) skip_candidate.insert(i + 1);
                } else if (can_skip_i && skip_candidate.contains(i)) {
                    // do nothing
                } else {
                    return false;
                }
            }
        }
        
        return true;
    }

    struct CondAscend {
        bool operator() (int l, int r) {
            return (l < r && r <= l + 3);
        }
    };

    struct CondDescend {
        bool operator() (int r, int l) {
            return (l < r && r <= l + 3);
        }
    };

    bool check_ascending(const vector<int>& vec, bool check_skip) {
        return check<CondAscend>(vec, CondAscend(), check_skip);
    }

    bool check_descending(const vector<int>& vec, bool check_skip) {
        return check<CondDescend>(vec, CondDescend(), check_skip);
    }

    void get_input(const string& filename) {
        if (input.size() != 0) 
            return;
        
        auto lines = get_input_all_lines(filename);
        for (auto& line : lines) {
            vector<int> vec;
            istringstream ss(line);
            int val;
            while (ss >> val) {
                vec.push_back(val);
            }
            input.push_back(vec);
        }
    }

    string part1(const string& filename) override {
        get_input(filename);
        int count = 0;
        for (auto& vec : input) {
            if (check_ascending(vec, false) || check_descending(vec, false)) {
                count++;
            }
        }
        return to_string(count);
    }

    string part2(const string& filename) override {
        get_input(filename);
        int count = 0;
        for (auto& vec : input) {
            if (check_ascending(vec, true) || check_descending(vec, true)) {
                count++;
            }
        }
        return to_string(count);
    }
};
