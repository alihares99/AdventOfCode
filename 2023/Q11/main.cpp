#include <iostream>
#include <fstream>
#include <cassert>
#include <algorithm>
#include <numeric>
#include <vector>
#include <array>
#include <string>
#include <regex>
#include <sstream>
#include <optional>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <stack>
#include <queue>

using namespace std;

struct Point {
    int i, j;
};

auto get_input() {
    ifstream file("../../input.txt");
    vector<string> mat;
    vector<Point> galaxies;
    vector<int> row_no_galaxy;
    vector<int> col_no_galaxy;
    
    string str;
    while (getline(file, str)) {
        mat.push_back(str);
    }

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

    return make_tuple(galaxies, row_no_galaxy, col_no_galaxy);
}

uint64_t solve(int n_expantion) {
    auto [galaxies, row_no_galaxy, col_no_galaxy] = get_input();

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

int main() {
    cout << solve(2) << " " << solve(1000000) << endl;
    return 0;
}