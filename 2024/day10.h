#pragma once
#include "useful.h"

struct Day10 : public Solution {

    vector<string> mat;
    const size_t n_row = 0;
    const size_t n_col = 0;

    void get_input(const string& filename) {
        if (!mat.empty())
            return;
        
        mat = get_input_all_lines(filename);
        const_cast<size_t&>(n_row) = mat.size();
        const_cast<size_t&>(n_col) = mat[0].size();
    }

    void get_nines(vector<string>& mat, int i, int j, unordered_set<Point>& nines) {
        if (mat[i][j] == '9') {
            nines.insert({i, j});
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

    string part1(const string& filename) override {
        get_input(filename);
        uint64_t res = 0;
        for (int i = 0; i < n_row; i++) {
            for (int j = 0; j < n_col; j++) {
                if (mat[i][j] == '0') {
                    unordered_set<Point> nines;
                    get_nines(mat, i, j, nines);
                    res += nines.size();
                }
            }
        }
        return to_string(res);
    }

    string part2(const string& filename) override {
        get_input(filename);
        uint64_t res = 0;
        for (int i = 0; i < n_row; i++) {
            for (int j = 0; j < n_col; j++) {
                if (mat[i][j] == '0') {
                    res += count(mat, i, j);
                }
            }
        }

        return to_string(res);
    }
};
