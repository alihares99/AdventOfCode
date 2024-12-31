#pragma once
#include "useful.h"

struct Day11 : public SolutionPair {
    vector<Point> galaxies;
    vector<int> row_no_galaxy;
    vector<int> col_no_galaxy;

    void get_input(const string& filename) {
        auto mat = get_input_all_lines(filename);

        for (int i = 0; i < mat.size(); i++) {
            for (int j = 0; j < mat[i].size(); j++) {
                if (mat[i][j] == '#')
                    galaxies.emplace_back(i, j);
            }
        }

        vector<bool> row_has_galaxy(mat.size(), false);
        vector<bool> col_has_galaxy(mat[0].size(), false);
        for (auto& p : galaxies) {
            row_has_galaxy[p.i] = true;
            col_has_galaxy[p.j] = true;
        }

        for (int j = 0; j < col_has_galaxy.size(); j++) {
            if (!col_has_galaxy[j])
                col_no_galaxy.push_back(j);
        }

        for (int i = 0; i < row_has_galaxy.size(); i++) {
            if (!row_has_galaxy[i])
                row_no_galaxy.push_back(i);
        }
    }

    uint64_t solve(int n_expantion) {
        uint64_t res = 0;
        for (int i = 0; i < galaxies.size() - 1; i++) {
            for (int j = i; j < galaxies.size(); j++) {
                auto& g1 = galaxies[i];
                auto& g2 = galaxies[j];
                res += abs(g1.i - g2.i) + abs(g1.j - g2.j) +
                    (n_expantion - 1) * abs(ranges::upper_bound(row_no_galaxy, g1.i) - ranges::upper_bound(row_no_galaxy, g2.i)) +
                    (n_expantion - 1) * abs(ranges::upper_bound(col_no_galaxy, g1.j) - ranges::upper_bound(col_no_galaxy, g2.j));
            }
        }
        return res;
    }

    pair<string, string> solve(const string& filename) override {
        get_input(filename);
        return {to_string(solve(2)), to_string(solve(1000000))};
    }
};
