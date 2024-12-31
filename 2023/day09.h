#pragma once
#include "useful.h"

struct Day09 : public SolutionPair {
    auto get_input(const string& filename) {
        ifstream file(filename);
        vector<vector<int64_t>> nums_vec;

        string str;
        while (getline(file, str)) {
            nums_vec.emplace_back();
            istringstream ss(str);
            int64_t val;
            while (ss >> val) {
                nums_vec.back().push_back(val);
            }
        }

        return nums_vec;
    }


    pair<string, string> solve(const string& filename) override {
        auto nums_vec = get_input(filename);
        int64_t res = 0;
        int64_t res2 = 0; // part2
        for (auto& nums : nums_vec) {
            stack<int64_t> st;
            stack<int64_t> st2; // part2
            bool has_non_zero = true;
            while (has_non_zero) {
                st.push(nums.back());
                st2.push(nums.front());
                has_non_zero = false;
                for (int i = 0; i < nums.size() - 1; i++) {
                    nums[i] = nums[i + 1] - nums[i];
                    if (nums[i] != 0)
                        has_non_zero = true;
                }
                nums.resize(nums.size() - 1);
            }

            while (!st.empty()) {
                res += st.top();
                st.pop();
            }
            
            // part2:
            int64_t partial2 = st2.top();
            st2.pop();
            while (!st2.empty()) {
                partial2 = st2.top() - partial2;
                st2.pop();
            }
            res2 += partial2;
        }
        return {to_string(res), to_string(res2)};
    }
};
