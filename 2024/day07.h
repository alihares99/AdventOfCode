#pragma once
#include "useful.h"

struct Day07 : public Solution {
    optional<pair<uint64_t, uint64_t>> result;

    uint64_t concat_2_numbers(uint64_t a, uint64_t b) {
        int n_digits = get_n_digits(b);
        uint64_t tens = 1;
        for (int i = 0; i < n_digits; i++) 
            tens *= 10;
        return a * tens + b;
    }

    bool check(uint64_t res, vector<uint64_t>& nums, int index, uint64_t so_far) {
        if (index == nums.size()) {
            if (so_far == res)
                return true;
            else
                return false;
        }

        return check(res, nums, index + 1, so_far + nums[index]) ||
                check(res, nums, index + 1, so_far * nums[index]);
    }

    bool check2(uint64_t res, vector<uint64_t>& nums, int index, uint64_t so_far) {
        if (index == nums.size()) {
            if (so_far == res)
                return true;
            else
                return false;   
        }

        if (so_far > res)
            return false;
        
        return check2(res, nums, index + 1, so_far + nums[index]) ||
            check2(res, nums, index + 1, so_far * nums[index]) ||
            check2(res, nums, index + 1, concat_2_numbers(so_far, nums[index]));
    }

    void solve(const string& filename) {
        if (result.has_value())
            return;
        
        auto lines = get_input_all_lines(filename);

        uint64_t count = 0;
        uint64_t count2 = 0;

        for (const auto& line : lines) {
            istringstream ss (line);
            uint64_t tmp;
            ss >> tmp;
            uint64_t res = tmp;
            ss.ignore();
            vector<uint64_t> nums;
            while (ss >> tmp) {
                nums.push_back(tmp);
            }

            if (check(res, nums, 1, nums[0])) {
                count += res;
            }
            if (check2(res, nums, 1, nums[0])) {
                count2 += res;
            }
        }

        result = {count, count2};
    }

    string part1(const string& filename) override {
        solve(filename);
        return to_string(result->first);
    }

    string part2(const string& filename) override {
        solve(filename);
        return to_string(result->second);
    }
};
