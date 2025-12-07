#pragma once
#include "useful.h"

struct Day06 : public SolutionPair
{

    static inline constexpr bool debug = false;

    auto get_input(const string &filename)
    {
        string f = filename;
        if (debug)
            f = "../test.txt";
        auto lines = get_input_all_lines(f);


        vector<vector<int>> nums(lines.size() - 1);
        vector<char> ops;
        for (int i = 0; i < lines.size() - 1; i++) {
            istringstream ss(lines[i]);
            while (true) {
                int a;
                ss >> a;
                if (ss.fail())
                    break;
                nums[i].push_back(a);
            }
        }
        auto& ops_line = lines[lines.size() - 1];
        istringstream ss(ops_line);
        while(true) {
            char c;
            ss >> c;
            if (ss.fail())
                break;
            ops.push_back(c);
        }

        // part2:
        vector<vector<int>> nums2;
        vector<int> col_nums;
        for (int j = 0; j < lines[0].size(); j++) {
            bool empty = true;
            int v = 0;
            for (int i = 0; i < lines.size() - 1; i++) {
                int digit = int(lines[i][j]) - '0';
                if (0 <= digit && digit <= 9) {
                    empty = false;
                    v *= 10;
                    v += digit;
                }
            }

            if (!empty) {
                col_nums.push_back(v);
            }
            else {
                nums2.push_back(std::move(col_nums));
                col_nums = vector<int>();
            }
        }
        nums2.push_back(std::move(col_nums));
        return make_tuple(nums, ops, nums2);
    }

    pair<string, string> solve(const string &filename) override
    {
        auto [nums, ops, nums2] = get_input(filename);
        uint64_t part1 = 0;
        uint64_t part2 = 0;

        for (int j = 0; j < nums[0].size(); j++) {
            char op = ops[j];
            uint64_t res = (op == '+' ? 0 : 1);
            for (int i = 0; i < nums.size(); i++) {
                if (op == '+') {
                    res += nums[i][j];
                } else {
                    res *= nums[i][j];
                }
            }
            part1 += res;
        }

        for (int i = 0; i < ops.size(); i++) {
            uint64_t res = ops[i] == '+' ? 0 : 1;
            for (int j = 0; j < nums2[i].size(); j++) {
                if (ops[i] == '+') {
                    res += nums2[i][j];
                }
                else {
                    res *= nums2[i][j];
                }
            }
            part2 += res;
        }

        return {to_string(part1), to_string(part2)};
    }
};
