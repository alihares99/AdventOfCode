#pragma once
#include "useful.h"

struct Day18 : public Solution {
    vector<string> lines;
    const int grid_size = 71;

    struct ScorePoint : public DirectedPoint {
        int score = 0;
    };

    auto get_input(int how_many) {
        vector<vector<bool>> bytes;
        bytes.resize(grid_size, vector<bool>(grid_size, false));
        for (int i = 0; i < min(how_many, int(lines.size() + 1)); i++) {
            istringstream ss(lines[i + 1]);
            int v1, v2;
            ss >> v1;
            ss.ignore();
            ss >> v2;
            bytes[v2][v1] = true;
        }

        return bytes;
    }

    int solve(int how_many) {
        vector<vector<bool>> bytes = get_input(how_many);
        vector<vector<int>> min_dist(grid_size, vector<int>(grid_size, numeric_limits<int>::max()));
        vector<ScorePoint> to_visit;
        ScorePoint first;
        first.i = 0;
        first.j = 0;
        first.score = 0;
        first.dir = dir_right;
        to_visit.push_back(first);
        while (!to_visit.empty()) {
            vector<ScorePoint> next_points;
            for (auto& p : to_visit) {
                if (p.i < 0 || p.j < 0 || p.i >= grid_size || p.j >= grid_size)
                    continue;
                if (bytes[p.i][p.j]) 
                    continue;
                if (min_dist[p.i][p.j] > p.score)
                    min_dist[p.i][p.j] = p.score;
                else
                    continue;

                if (p.i == grid_size - 1 && p.j == grid_size - 1)
                    return min_dist[p.i][p.j];

                
                auto p1 = p; p1.move(); p1.score = p.score + 1;
                auto p2 = p; p2.turn_right(); p2.move(); p2.score = p.score + 1;
                auto p3 = p; p3.turn_left(); p3.move(); p3.score = p.score + 1;
                next_points.push_back(p1);
                next_points.push_back(p2);
                next_points.push_back(p3);
            }
            to_visit = move(next_points);
        }

        return -1;
    }

    string part1(const string& filename) override {
        if (lines.empty())
            lines = get_input_all_lines(filename);
        
        return to_string(solve(1024));
    }

    string part2(const string& filename) override {
        if (lines.empty())
            lines = get_input_all_lines(filename);
        int start = 0;
        int end = int(lines.size()) - 1;
        while (start != end - 1) {
            int n = (start + end) / 2;
            bool has_path = (solve(n) != -1);
            if (has_path)
                start = n;
            else
                end = n;
        }
        return lines[end];
    }
};
