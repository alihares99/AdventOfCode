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

using namespace std;

void transpose(vector<string>& mat) {
    int n_row = mat.size();
    int n_col = mat[0].size();
    for (int i = 0; i < n_row; i++) {
        for (int j = 0; j < n_col; j++) {
            if (i == j) continue;
            swap(mat[i][j], mat[j][i]);
        }
    }
}

void shift_up(vector<string>& mat, int n) {
    int n_row = mat.size();
    int n_col = mat[0].size();
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

int part1() {
    ifstream file("../../input.txt", ios::in);

    int count = 0;

    vector<string> lines;
    lines.reserve(200);
    string str;
    while(getline(file, str)) {
        lines.push_back(str);
    }
    int n_row = lines.size();
    int n_col = lines[0].size();

    count += count_in_rows(lines); // horizontal
    transpose(lines);
    count += count_in_rows(lines); // vertical
    shift_up(lines, 1);
    count += count_in_rows(lines); // diagonal
    shift_up(lines, -2);
    count += count_in_rows(lines); // cross diagonal

    return count;
}

int part2() {
    ifstream file("../../input.txt", ios::in);

    int count = 0;

    vector<string> lines;
    lines.reserve(200);
    string str;
    while(getline(file, str)) {
        lines.push_back(str);
    }
    int n_row = lines.size();
    int n_col = lines[0].size();

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

    return count;
}

int main () {
    cout << "part 1: " << part1() << endl;
    cout << "part 2: " << part2() << endl;
}
