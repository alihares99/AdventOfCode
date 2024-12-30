#pragma once
#include "useful.h"

struct Day08 : public Solution {
    vector<string> mat;
    const int n_row = 0;
    const int n_col = 0;

    void get_input(const string& filename) {
        if (!mat.empty())
            return;
        mat = get_input_all_lines(filename);
        const_cast<int&>(n_row) = int(mat.size());
        const_cast<int&>(n_col) = int(mat[0].size());
    }

    bool is_in_bound(const Point& p) {
        return p.i >= 0 && p.j >= 0 && p.i < n_row && p.j < n_col;
    };

    int solve(bool part2) {
        unordered_map<char, vector<Point>> frequencies;
        for (int i = 0; i < n_row; i++) {
            for (int j = 0; j < n_col; j++) {
                if (mat[i][j] != '.') {
                    frequencies[mat[i][j]].emplace_back(i, j);
                }
            }
        }

        unordered_set<Point> antinodes;
        for (auto& item : frequencies) {
            auto& antennas = item.second;
            for (int i = 0; i < antennas.size() - 1; i++) {
                for (int j = i + 1; j < antennas.size(); j++) {
                    auto ant_1 = antennas[i];
                    auto ant_2 = antennas[j];
                    int diff_i = ant_2.i - ant_1.i;
                    int diff_j = ant_2.j - ant_1.j;

                    if (part2) {
                        antinodes.insert(ant_1);
                        antinodes.insert(ant_2);
                    }

                    while(true) {
                        ant_2.i += diff_i;
                        ant_2.j += diff_j;
                        if (is_in_bound(ant_2)) {
                            antinodes.insert(ant_2);
                            if (!part2)
                                break;
                        }
                        else
                            break;
                    }

                    while(true) {
                        ant_1.i -= diff_i;
                        ant_1.j -= diff_j;
                        if (is_in_bound(ant_1)){
                            antinodes.insert(ant_1);
                            if (!part2)
                                break;
                        }
                        else
                            break;
                    }
                }
            }
        }

        return int(antinodes.size());
    }

    string part1(const string& filename) override {
        get_input(filename);
        return to_string(solve(false));
    }

    string part2(const string& filename) override {
        get_input(filename);
        return to_string(solve(true));
    }
};
