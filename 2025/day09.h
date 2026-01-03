#pragma once
#include "useful.h"

struct Day09 : public SolutionPair {

    static inline constexpr bool debug = false;

    auto get_input(const string& filename) {
        string f = filename;
        if (debug)
            f = "../test.txt";
        auto lines = get_input_all_lines(f);
        vector<Point> points;
        for (auto& line : lines) {
            istringstream ss(line);
            points.emplace_back();
            char c;
            ss >> points.back().i >> c >> points.back().j;
        }
        return points;
    }

    bool is_inside(const Point& p, const map<int, vector<pair<int, int>>>& col_lines) {
        int lines_passed = 0;
        int points_passed = 0;
        for (auto& [col, row_ranges] : col_lines) {
            if (col > p.i) break;
            for (auto& [row_start, row_end] : row_ranges) {
                if (p.j == row_start || p.j == row_end) {
                    points_passed++;
                }
                if (row_start <= p.j && p.j < row_end) {
                    lines_passed++;
                }
            }
        }
        return points_passed % 2 == 1 || lines_passed % 2 == 1;
    }

    pair<string, string> solve(const string& filename) override {
        auto points = get_input(filename);
        uint64_t part1 = 0;
        uint64_t part2 = 0;

        // part1:
        for (int i = 0; i < points.size(); i++) {
            for (int j = i + 1; j < points.size(); j++) {
                auto& a = points[i];
                auto& b = points[j];
                part1 = max(part1, uint64_t(abs(a.i - b.i) + 1) * uint64_t(abs(a.j - b.j) + 1));
            }
        }

        // part2:
        map<int, vector<pair<int, int>>> row_lines;
        map<int, vector<pair<int, int>>> col_lines;
        int prev_i = int(points.size()) - 1;
        for (int i = 0; i < points.size(); i++) {
            if (points[prev_i].i == points[i].i) {
                auto min_ = min(points[prev_i].j, points[i].j);
                auto max_ = max(points[prev_i].j, points[i].j);
                col_lines[points[prev_i].i].push_back(make_pair(min_, max_));
            } else {
                auto min_ = min(points[prev_i].i, points[i].i);
                auto max_ = max(points[prev_i].i, points[i].i);
                row_lines[points[prev_i].j].push_back(make_pair(min_, max_));
            }
            prev_i = i;
        }

        for (int i = 0; i < points.size(); i++) {
            for (int j = i + 1; j < points.size(); j++) {
                auto& a = points[i];
                auto& b = points[j];
                auto current_size = uint64_t(abs(a.i - b.i) + 1) * uint64_t(abs(a.j - b.j) + 1);
                if (current_size > part2) {
                    Point center {(a.i + b.i) / 2, (a.j + b.j) / 2};
                    if (!is_inside(center, col_lines)) {
                        continue;
                    }
                    
                    auto up = min(a.j, b.j);
                    auto down = max(a.j, b.j);
                    auto left = min(a.i, b.i);
                    auto right = max(a.i, b.i);

                    bool give_up = false;
                    for (auto it = col_lines.upper_bound(left); 
                        !give_up && it != col_lines.end() && it->first < right; ++it) {
                        for (auto& [row_start, row_end] : it->second) {
                            if (!((row_start <= up && row_end <= up) || (row_start >= down && row_end >= down))) {
                                give_up = true;
                                break;
                            }
                        }
                    }

                    if (give_up) continue;
                    
                    for (auto it = row_lines.upper_bound(up); 
                        !give_up && it != row_lines.end() && it->first < down; ++it) {
                        for (auto& [col_start, col_end] : it->second) {
                            if (!((col_start <= left && col_end <= left) || (col_start >= right && col_end >= right))) {
                                give_up = true;
                                break;
                            }
                        }
                    }

                    if (give_up) continue;
                    
                    part2 = current_size;
                }
            }
        }
        
        return {to_string(part1), to_string(part2)};
    }
};
