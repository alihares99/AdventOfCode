#pragma once
#include "useful.h"

struct Day17 : public SolutionPair {

    int n_row = 0;
    int n_col = 0;

    auto get_input(const string& filename) {
        auto lines = get_input_all_lines(filename);
        vector<vector<int>> inputs;
        n_row = int(lines.size());
        n_col = int(lines[0].size());
        inputs.resize(n_row, vector<int>(n_col));
        for (int i = 0; i < n_row; i++)
            for (int j = 0; j < n_col; j++)
                inputs[i][j] = lines[i][j] - '0';
        return inputs;
    }

    struct MinHeat {
        vector<vector<array<int, 4>>> data;
        MinHeat(int n_row, int n_col) : 
            data(n_row, 
                vector<array<int, 4>>(n_col, 
                make_array<int, 4>(numeric_limits<int>::max()))) 
        {}
        int& operator()(const DirectedPoint& p) { return data[p.i][p.j][p.dir]; }
        int& operator()(const Point& p, Direction dir) { return data[p.i][p.j][dir]; }
        int& operator()(int i, int j, Direction dir) { return data[i][j][dir]; }
    };

    int solve(vector<vector<int>>& inputs, int min_move, int max_move) {
        DirectedPoint p1 = {{0, 0}, dir_down};
        DirectedPoint p2 = {{0, 0}, dir_right};
        MinHeat min_heat(n_row, n_col);
        queue<DirectedPoint> to_visit;
        min_heat(p1) = 0;
        min_heat(p2) = 0;
        to_visit.push(p1);      
        to_visit.push(p2);
        while (!to_visit.empty()) {
            auto& p = to_visit.front();
            int added_heat = 0;
            int last_heat = min(min_heat(p, rotate_clockwise(p.dir)), min_heat(p, rotate_counter_clockwise(p.dir)));
            for (int i = 1; i <= max_move; i++) {
                p.move();
                if (p.i < 0 || p.j < 0 || p.i >= n_row || p.j >= n_col)
                    break;
                added_heat += inputs[p.i][p.j];
                if (i >= min_move) {
                    int new_val = added_heat + last_heat;
                    if (new_val < min_heat(p)) {
                        min_heat(p) = new_val;
                        to_visit.push(DirectedPoint(p).turn_left());
                        to_visit.push(DirectedPoint(p).turn_right());
                    }
                }
            }
            to_visit.pop();
        }
        Point last = {n_row - 1, n_col - 1};
        return min(min_heat(last, dir_right), min_heat(last, dir_down));
    }

    pair<string, string> solve(const string& filename) override {
        auto inputs = get_input(filename);

        int part1 = solve(inputs, 1, 3);
        int part2 = solve(inputs, 4, 10);
        
        return {to_string(part1), to_string(part2)};
    }
};
