#include <iostream>
#include <fstream>
#include <cassert>
#include <algorithm>
#include <vector>
#include <array>
#include <string>
#include <sstream>
#include <set>
#include <cstdio>
#include <optional>
#include <unordered_map>
#include <unordered_set>
#include <chrono>

using namespace std;

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


    auto concat_2_numbers = [&](){
        int n_digits = 1;
        auto v = nums[index];
        while (v /= 10) {
            n_digits++;
        }
        uint64_t tens = 1;
        for (int i = 0; i < n_digits; i++) 
            tens *= 10;
        return so_far * tens + nums[index];
    };
    // cute but super slow alternative:
    /*
    auto concat_2_numbers = [&](){
        stringstream ss;
        ss << so_far << nums[index];
        uint64_t res;
        ss >> res;
        return res;
    };
    */
    uint64_t concat = concat_2_numbers();
    
    return check2(res, nums, index + 1, concat) ||
            check2(res, nums, index + 1, so_far + nums[index]) ||
            (so_far == 0 ? false : check2(res, nums, index + 1, so_far * nums[index]))
            ;
}

pair<uint64_t, uint64_t> solve() {
    ifstream file("../../input.txt", ios::in);

    uint64_t count = 0;
    uint64_t count2 = 0;

    auto get_lines = [&](){
        vector<string> lines;
        string str;
        while(getline(file, str)) {
            lines.push_back(str);
        }
        return lines;
    };
    auto lines = get_lines();

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
        if (check2(res, nums, 0, 0)) {
            count2 += res;
        }
    }

    return {count, count2};
}

int main () {
    using namespace chrono;
    auto tick = high_resolution_clock::now();
    auto res = solve();
    auto tock = high_resolution_clock::now();

    cout << res.first << " " << res.second << endl;
    cout << duration_cast<milliseconds>(tock - tick) << endl;
}
