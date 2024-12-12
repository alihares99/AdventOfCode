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

auto get_input() {
    vector<string> mat;

    ifstream file("../../input.txt");
    string str;
    while(getline(file, str)) {
        mat.push_back(str);
    }
    return mat;
}

// area, perimeter
pair<uint64_t, uint64_t> recursive_part1(vector<string>& mat, char c, int i, int j) {
    const auto n_row = mat.size();
    const auto n_col = mat[0].size();
    if (!(i >= 0 && i < n_row && j >= 0 && j < n_col) || (mat[i][j] != c && mat[i][j] != c + 32)) 
        return make_pair(0, 1);
    if (mat[i][j] != c)
        return make_pair(0, 0);

    mat[i][j] += 32;
    
    auto up = recursive_part1(mat, c, i - 1, j);
    auto down = recursive_part1(mat, c, i + 1, j);
    auto left = recursive_part1(mat, c, i, j - 1);
    auto right = recursive_part1(mat, c, i, j + 1);

    return make_pair(1 + up.first + down.first + left.first + right.first, 
                up.second + down.second + left.second + right.second);
}

uint64_t solve1() {
    auto mat = get_input();
    vector<pair<uint64_t, uint64_t>> nums;
    const auto n_row = mat.size();
    const auto n_col = mat[0].size();
    for (int i = 0; i < n_row; i++) {
        for (int j = 0; j < n_col; j++) {
            if (mat[i][j] <= 'Z') {
                nums.push_back(recursive_part1(mat, mat[i][j], i, j));
            }
        }
    }

    uint64_t res = 0;
    for (auto& item : nums) {
        res += item.first * item.second;
    }

    return res;
}

enum Direction : uint8_t {
    dir_up, dir_down, dir_left, dir_right,
};

union Side {
    struct {uint16_t i, j; Direction dir;};
    uint64_t full;
};  

uint64_t recursive_part2(vector<string>& mat, char c, int i, int j, Direction dir, unordered_set<uint64_t>& sides) {
    const auto n_row = mat.size();
    const auto n_col = mat[0].size();
    if (!(i >= 0 && i < n_row && j >= 0 && j < n_col) || (mat[i][j] != c && mat[i][j] != c + 32)) {
        Side tmp;
        tmp.i = i;
        tmp.j = j;
        tmp.dir = dir;
        sides.insert(tmp.full);
        return 0;
    }
    if (mat[i][j] != c)
        return 0;

    mat[i][j] += 32;
    
    auto up = recursive_part2(mat, c, i - 1, j, dir_down, sides);
    auto down = recursive_part2(mat, c, i + 1, j, dir_up, sides);
    auto left = recursive_part2(mat, c, i, j - 1, dir_right, sides);
    auto right = recursive_part2(mat, c, i, j + 1, dir_left, sides);

    return 1 + up + down + left + right;
}

uint64_t find_n_sides(unordered_set<uint64_t>& sides) {
    uint64_t out = 0;
    while (!sides.empty()) {
        out++;
        Side tmp;
        tmp.full = *sides.begin();
        sides.erase(tmp.full);

        auto get_next = [](Side tmp, int n) {
            if (tmp.dir == dir_down || tmp.dir == dir_up) {
                tmp.j += n;
                return tmp;
            }
            else {
                tmp.i += n;
                return tmp;
            }
        };
        auto next1 = get_next(tmp, 1);
        auto next2 = get_next(tmp, -1);
        while (sides.contains(next1.full)) {
            sides.erase(next1.full);
            next1 = get_next(next1, 1);
        }
        while (sides.contains(next2.full)) {
            sides.erase(next2.full);
            next2 = get_next(next2, -1);
        }
    }
    return out;
}

uint64_t solve2() {
    auto mat = get_input();
    vector<pair<uint64_t, uint64_t>> nums;
    const auto n_row = mat.size();
    const auto n_col = mat[0].size();
    for (int i = 0; i < n_row; i++) {
        for (int j = 0; j < n_col; j++) {
            if (mat[i][j] <= 'Z') {
                unordered_set<uint64_t> sides;
                auto area = recursive_part2(mat, mat[i][j], i, j, dir_up, sides);
                auto n_sides = find_n_sides(sides);
                nums.emplace_back(area, n_sides);
            }
        }
    }

    uint64_t res = 0;
    for (auto& item : nums) {
        res += item.first * item.second;
    }

    return res;
}


int main () {
    cout << solve1() << " " << solve2() << endl;
}
