#pragma once
#include "useful.h"

struct Day04 : public Solution {
    vector<string> lines;
    int n_row;
    int n_col;

   void get_input(const string& filename) {
        lines = get_input_all_lines(filename);
        n_row = int(lines.size());
        n_col = int(lines[0].size());
    }

    void transpose(vector<string>& mat) {
        int n_row = int(mat.size());
        int n_col = int(mat[0].size());
        for (int i = 0; i < n_row; i++) {
            for (int j = 0; j < n_col; j++) {
                if (i == j) continue;
                swap(mat[i][j], mat[j][i]);
            }
        }
    }

    void shift_up(vector<string>& mat, int n) {
        int n_row = int(mat.size());
        int n_col = int(mat[0].size());
        for (int j = 1; j < n_col; j++) {
            string col;
            col.reserve(n_row);
            for (int i = 0; i < n_row; i++) {
                col.push_back(mat[i][j]);
            }
            for (int i = 0; i < n_row; i++) {
                int index = (i + n * j) % n_row;
                if (index < 0) index += n_row;
                mat[i][j] = col[index];
            }
        }
    }

    int count_in_rows(const vector<string>& lines) {
        int count = 0;
        for (const auto& line : lines) {
            auto found = line.find("XMAS");
            while (found != string::npos) {
                count++;
                found = line.find("XMAS", found + 4);
            }

            found = line.find("SAMX");
            while (found != string::npos) {
                count++;
                found = line.find("SAMX", found + 4);
            }
        }
        return count;
    }

    string part1(const string& filename) override {
        get_input(filename);

        int count = 0;
        count += count_in_rows(lines); // horizontal
        transpose(lines);
        count += count_in_rows(lines); // vertical
        shift_up(lines, 1);
        count += count_in_rows(lines); // diagonal
        shift_up(lines, -2);
        count += count_in_rows(lines); // cross diagonal

        return to_string(count);
    }

    string part2(const string& filename) override {
        get_input(filename);

        int count = 0;
        for (int i = 1; i < n_row - 1; i++) {
            for (int j = 1; j < n_col - 1; j++) {
                if (lines[i][j] == 'A') {
                    string s = {lines[i-1][j-1], lines[i+1][j+1]};
                    string s2 = {lines[i-1][j+1], lines[i+1][j-1]};
                    if ((s == "MS" || s == "SM") && (s2 == "MS" || s2 == "SM")) {
                        count ++;
                    }
                }
            }
        }
        return to_string(count);
    }
};
