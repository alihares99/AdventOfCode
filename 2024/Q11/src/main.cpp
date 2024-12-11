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
#include <forward_list>
#include <chrono>

using namespace std;

int get_n_digits(auto val) {
    int n_digit = 1;
    while (val /= 10) n_digit++;
    return n_digit;
}

uint64_t get_tens(auto val) {
    uint64_t res = 1;
    for (auto i = 0; i < val; i++) res *= 10;
    return res;
}

auto get_input() {
    unordered_map<uint64_t, uint64_t> nums;

    ifstream file("../../input.txt");
    string str;
    getline(file, str);
    istringstream ss(str);
    uint64_t v;
    while (ss >> v) {
        nums[v]++;
    }
    return nums;
}

uint64_t solve(int n) {
    auto nums = get_input();
    
    for (int i = 0; i < n; i++) {
        decltype(nums) tmp;
        for (auto it : nums) {
            if (it.first == 0)
                tmp[1] += it.second;
            else {
                int n_digits = get_n_digits(it.first);
                if (n_digits % 2 == 0) {
                    auto tens = get_tens(n_digits / 2);
                    auto v1 = it.first / tens;
                    auto v2 = it.first % tens;
                    tmp[v1] += it.second;
                    tmp[v2] += it.second;
                }
                else {
                    tmp[it.first * 2024] += it.second;
                }
            }
        }
        nums = move(tmp);
    }

    uint64_t res = 0;
    for (auto it : nums) {
        res += it.second;
    }
    return res;
}

int main () {
    cout << solve(25) << " " << solve(75) << endl;
}
