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
#include <regex>

using namespace std;

struct Data{ int i, j, vi, vj; };

const int n_i = 101;
const int n_j = 103;

auto get_input() {
    vector<Data> input;
    ifstream file("../../input.txt");
    regex reg("p=(\\d+),(\\d+) v=(-?\\d+),(-?\\d+)");
    string str;
    while(getline(file, str)) {
        Data data;
        smatch match;
        if (regex_match(str, match, reg)) {
            data.i = stoi(match[1]);
            data.j = stoi(match[2]);
            data.vi = stoi(match[3]);
            data.vj = stoi(match[4]);
        }
        input.push_back(data);
    }
    return input;
}

uint64_t solve() {
    auto input = get_input();
    uint64_t res = 0;
    int n_seconds = 100;

    for (auto& data : input) {
        data.i = (data.i + data.vi * n_seconds) % n_i;
        data.j = (data.j + data.vj * n_seconds) % n_j;
        if (data.i < 0) data.i += n_i;
        if (data.j < 0) data.j += n_j;
    }

    int q1 = 0;
    int q2 = 0;
    int q3 = 0;
    int q4 = 0;
    for (auto& data : input) {
        if (data.i < n_i / 2) {
            if (data.j < n_j / 2) 
                q1++;
            else if (data.j > n_j / 2)
                q2++;
        }
        else if (data.i > n_i / 2) {
            if (data.j < n_j / 2) 
                q3++;
            else if (data.j > n_j / 2)
                q4++;
        }
    }

    return q1 * q2 * q3 * q4;
}


bool advance(vector<Data>& input, vector<vector<int>>& grid, int n) {
    for (auto& data : input) {
        grid[data.i][data.j]--;

        data.i = (data.i + data.vi * n) % n_i;
        data.j = (data.j + data.vj * n) % n_j;
        if (data.i < 0) data.i += n_i;
        if (data.j < 0) data.j += n_j;

        grid[data.i][data.j]++;
    }

    bool no_overlap = true;
    for (int i = 0; i < n_i; i++) {
        for (int j = 0; j < n_j; j++) {
            if (grid[i][j] > 1)
                no_overlap = false;
        }
    }
    return no_overlap;
}

auto make_grid(vector<Data>& input) {
    vector<vector<int>> grid;
    grid.resize(n_i);
    for (auto& vec : grid) 
        vec.resize(n_j);
    for (auto& data : input) {
        grid[data.i][data.j]++;
    }
    return grid;
}

uint64_t solve2() {
    auto input = get_input();
    auto grid = make_grid(input);
    uint64_t n = 1;
    while(!advance(input, grid, 1)) 
        ++n;

    cout << endl;
    for (int j = 0; j < n_j; j++) {
        for (int i = 0; i < n_i; i++) {
            if (grid[i][j] == 0)
                cout << " ";
            else
                cout << "*";
        }
        cout << endl;
    }
    cout << endl << endl;
    
    return n;
}

int main () {
    cout << solve() << " " << solve2() << endl;
}
