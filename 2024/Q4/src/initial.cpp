#include <fstream>
#include <vector>
#include <string>
#include <array>

using namespace std;

int part1_first_try() {
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

    array<char, 4> w1 = {'X', 'M', 'A', 'S'};
    array<char, 4> w2 = {'S', 'A', 'M', 'X'};
    int p1 = 0;
    int p2 = 0;
    for (int i = 0; i < n_row; i++) {
        p1 = 0;
        p2 = 0;
        for (int j = 0; j < n_col; j++) {
            if (lines[i][j] == w1[p1]) p1++; else if (lines[i][j] == w1[0]) p1 = 1; else p1 = 0;
            if (lines[i][j] == w2[p2]) p2++; else if (lines[i][j] == w2[0]) p2 = 1; else p2 = 0;

            if (p1 == 4) { 
                count ++; p1 = 0; }
            if (p2 == 4) { 
                count ++; p2 = 0; }
        }
    }

    for (int i = 0; i < n_col; i++) {
        p1 = 0;
        p2 = 0;
        for (int j = 0; j < n_row; j++) {
            if (lines[j][i] == w1[p1]) p1++; else if (lines[j][i] == w1[0]) p1 = 1; else p1 = 0;
            if (lines[j][i] == w2[p2]) p2++; else if (lines[j][i] == w2[0]) p2 = 1; else p2 = 0;

            if (p1 == 4) { count ++; p1 = 0; }
            if (p2 == 4) { count ++; p2 = 0; }
        }
        p1 = 0;
        p2 = 0;
    }

    int i_start = n_row - 4;
    int j_start = 0;
    while (j_start <= n_col - 4) {
        int i = i_start;
        int j = j_start;
        p1 = 0;
        p2 = 0;
        while (i < n_row && j < n_col) {
            if (lines[i][j] == w1[p1]) p1++; else if (lines[i][j] == w1[0]) p1 = 1; else p1 = 0;
            if (lines[i][j] == w2[p2]) p2++; else if (lines[i][j] == w2[0]) p2 = 1; else p2 = 0;

            if (p1 == 4) { count ++; p1 = 0; }
            if (p2 == 4) { count ++; p2 = 0; }

            i++;
            j++;
        }
        if (i_start > 0) {
            i_start--;
        } else {
            j_start++;
        }
    }

    i_start = 0;
    j_start = 3;
    while (i_start <= n_row - 4) {
        int i = i_start;
        int j = j_start;
        p1 = 0;
        p2 = 0;
        while (i < n_row && j >= 0) {
            if (lines[i][j] == w1[p1]) p1++; else if (lines[i][j] == w1[0]) p1 = 1; else p1 = 0;
            if (lines[i][j] == w2[p2]) p2++; else if (lines[i][j] == w2[0]) p2 = 1; else p2 = 0;

            if (p1 == 4) { count ++; p1 = 0; }
            if (p2 == 4) { count ++; p2 = 0; }

            i++;
            j--;
        }
        if (j_start < n_col) {
            j_start++;
        } else {
            i_start++;
        }
    }

    return count;
}