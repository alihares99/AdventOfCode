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

int solve(bool part2) {

    auto get_lines = [&](){
        ifstream file("../../input.txt", ios::in);
        vector<string> lines;
        string str;
        while(getline(file, str)) {
            lines.push_back(str);
        }
        return lines;
    };
    auto mat = get_lines();

    union Index {
        struct {int i, j;};
        uint64_t full;
    };
    unordered_map<char, vector<Index>> frequencies;

    const int n_row = mat.size();
    const int n_col = mat[0].size();
    auto is_in_bound = [&](int i, int j) {
        return i >= 0 && j >= 0 && i < n_row && j < n_col;
    };
    for (int i = 0; i < n_row; i++) {
        for (int j = 0; j < n_col; j++) {
            if (mat[i][j] != '.') {
                Index tmp;
                tmp.i = i;
                tmp.j = j;
                frequencies[mat[i][j]].push_back(tmp);
            }
        }
    }

    unordered_set<uint64_t> antinodes;
    for (auto& item : frequencies) {
        auto& antennas = item.second;
        for (int i = 0; i < antennas.size() - 1; i++) {
            for (int j = i + 1; j < antennas.size(); j++) {
                auto ant_1 = antennas[i];
                auto ant_2 = antennas[j];
                int diff_i = ant_2.i - ant_1.i;
                int diff_j = ant_2.j - ant_1.j;

                if (part2) {
                    antinodes.insert(ant_1.full);
                    antinodes.insert(ant_2.full);
                }

                while(true) {
                    ant_2.i += diff_i;
                    ant_2.j += diff_j;
                    if (is_in_bound(ant_2.i, ant_2.j)) {
                        antinodes.insert(ant_2.full);
                        if (!part2)
                            break;
                    }
                    else
                        break;
                }

                while(true) {
                    ant_1.i -= diff_i;
                    ant_1.j -= diff_j;
                    if (is_in_bound(ant_1.i, ant_1.j)){
                        antinodes.insert(ant_1.full);
                        if (!part2)
                            break;
                    }
                    else
                        break;
                }
            }
        }
    }

    return antinodes.size();
}

int main () {
    cout << solve(false) << " " << solve(true) << endl;
}
