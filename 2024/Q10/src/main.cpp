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

auto get_input() {
    vector<string> mat;

    ifstream file("../../input.txt");
    string str;
    while (getline(file, str)) {
        mat.push_back(str);
    }
    return mat;
}

union Index {
    struct {int i, j; };
    uint64_t full;
};

void get_nines(vector<string>& mat, int i, int j, unordered_set<uint64_t>& nines) {
    if (mat[i][j] == '9') {
        Index tmp;
        tmp.i = i;
        tmp.j = j;
        nines.insert(tmp.full);
        return;
    }
    auto next = mat[i][j] + 1;
    if (i > 0 && mat[i - 1][j] == next) 
        get_nines(mat, i - 1, j, nines);
    if (i < mat.size() - 1 && mat[i + 1][j] == next) 
        get_nines(mat, i + 1, j, nines);
    if (j > 0 && mat[i][j - 1] == next) 
        get_nines(mat, i, j - 1, nines);
    if (j < mat[0].size() - 1 && mat[i][j + 1] == next) 
        get_nines(mat, i, j + 1, nines);
}

uint64_t part1() {
    uint64_t res = 0;

    auto mat = get_input();
    const auto n_row = mat.size();
    const auto n_col = mat[0].size();
    for (int i = 0; i < n_row; i++) {
        for (int j = 0; j < n_col; j++) {
            if (mat[i][j] == '0') {
                unordered_set<uint64_t> nines;
                get_nines(mat, i, j, nines);
                res += nines.size();
            }
        }
    }

    return res;
}

int count(vector<string>& mat, int i, int j) {
    if (mat[i][j] == '9') {
        return 1;
    }

    int my_count = 0;
    auto next = mat[i][j] + 1;
    if (i > 0 && mat[i - 1][j] == next) 
        my_count += count(mat, i - 1, j);
    if (i < mat.size() - 1 && mat[i + 1][j] == next) 
        my_count += count(mat, i + 1, j);
    if (j > 0 && mat[i][j - 1] == next) 
        my_count += count(mat, i, j - 1);
    if (j < mat[0].size() - 1 && mat[i][j + 1] == next) 
        my_count += count(mat, i, j + 1);
    
    return my_count;
}

uint64_t part2() {
    uint64_t res = 0;

    auto mat = get_input();
    const auto n_row = mat.size();
    const auto n_col = mat[0].size();
    for (int i = 0; i < n_row; i++) {
        for (int j = 0; j < n_col; j++) {
            if (mat[i][j] == '0') {
                res += count(mat, i, j);
            }
        }
    }

    return res;
}

int main () {
    cout << part1() << " " << part2() << endl;
}
