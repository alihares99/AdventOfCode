#pragma once
#include "useful.h"

struct Day15 : public Solution {
    struct Matrix {
        vector<string> lines;
        Point robot;
        int n_row;
        int n_col;
        bool part2;

        Matrix(const vector<string>& lines, bool part2) : lines (lines), part2 (part2) {
            n_row = int(lines.size());
            n_col = int(lines[0].size());
            bool done = false;
            for (int i = 0; i < n_row && !done; i++) {
                for (int j = 0; j < n_col && !done; j++) {
                    if (lines[i][j] == '@') {
                        robot.i = i;
                        robot.j = j;
                        done = true;
                    }
                }
            }
        }

        char& get(const Point& p) {
            return lines[p.i][p.j];
        }

        void make_a_move(Direction dir) {
            if (part2) {
                if (dir == dir_left || dir == dir_right) {
                    part2 = false;
                    make_a_move(dir);
                    part2 = true;
                }
                else {
                    bool can = true;
                    vector<unordered_set<Point>> all_rows;
                    unordered_set<Point> row;
                    row.insert(robot);
                    while (can && !row.empty()) {
                        all_rows.push_back(move(row));
                        row = unordered_set<Point>{};
                        for (Point p : all_rows.back()) {
                            Point next = p;
                            next.move(dir);
                            char next_c = get(next);
                            if (next_c == '#') {
                                can = false; // cannot!
                                break;
                            }
                            else if (next_c == '[') {
                                row.insert(Point{next.i, next.j});
                                row.insert(Point{next.i, next.j + 1});
                            }
                            else if (next_c == ']') {
                                row.insert(Point{next.i, next.j});
                                row.insert(Point{next.i, next.j - 1});
                            }
                        }                
                    }
                    if (can) {
                        for (int i = (int) all_rows.size() - 1; i >= 0; i--) {
                            for (auto p : all_rows[i]) {
                                Point next = p;
                                next.move(dir);
                                swap(get(next), get(p));
                            }
                        }
                        robot.move(dir);
                    }
                }
            }
            else {
                Point end_of_line = robot;
                end_of_line.move(dir);
                while (get(end_of_line) == 'O' || get(end_of_line) == '[' || get(end_of_line) == ']') {
                    end_of_line.move(dir);    
                }
                char end_char = get(end_of_line);
                if (end_char == '.') {
                    Direction opp_dir = mirror(dir);
                    while (end_of_line != robot) {
                        Point next = end_of_line;
                        next.move(opp_dir);
                        swap(get(end_of_line), get(next));
                        end_of_line = next;
                    }
                    robot.move(dir);
                }
            }
        }

        uint64_t calculate_gps() {
            uint64_t res = 0;
            for (int i = 0; i < n_row; i++) {
                for (int j = 0; j < n_col; j++) {
                    if ((part2 && lines[i][j] == '[') || (!part2 && lines[i][j] == 'O')) {
                        res += i * 100 + j;
                    }
                }
            }
            return res;
        }

    };


    auto get_input(const string& filename, bool part2) {
        vector<Direction> moves;
        vector<string> lines;

        ifstream file(filename);
        string str;
        while(getline(file, str)) {
            if (str.empty())
                break;
            
            if (part2) {
                string tmp;
                tmp.reserve(str.size() * 2);
                for (auto c : str) {
                    switch (c)
                    {
                    case '#': tmp.append("##"); break;
                    case 'O': tmp.append("[]"); break;
                    case '.': tmp.append(".."); break;
                    case '@': tmp.append("@."); break;
                    }
                }
                lines.push_back(tmp);
            }
            else {
                lines.push_back(str);
            }
            
        }

        while (getline(file, str)) {
            for (auto c : str) {
                Direction dir;
                switch (c) {
                    case '<': dir = dir_left; break;
                    case '^': dir = dir_up; break;
                    case '>': dir = dir_right; break;
                    case 'v': dir = dir_down; break;
                }
                moves.push_back(dir);
            }
        }
        return make_pair(Matrix{lines, part2}, moves);
    }

    uint64_t solve(const string& filename, bool part2) {
        auto [mat, moves] = get_input(filename, part2);
        for (auto move : moves) {
            mat.make_a_move(move);
        }
        return mat.calculate_gps();
    }

    string part1(const string& filename) override {
        return to_string(solve(filename, false));
    }

    string part2(const string& filename) override {
        return to_string(solve(filename, true));
    }
};
