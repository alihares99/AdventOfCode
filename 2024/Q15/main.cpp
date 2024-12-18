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

enum Direction : int {dir_up = 0, dir_right, dir_down, dir_left};

Direction mirror(Direction dir) {
    return Direction((dir + 2) % 4);
}

struct Point {
    int i, j;

    Point move(Direction dir) {
        Point p = *this;
        switch (dir)
        {
        case dir_up: p.i--; break;
        case dir_down: p.i++; break;
        case dir_right: p.j++; break;
        case dir_left: p.j--; break;
        }
        return p;
    }

    bool operator==(const Point& p) const = default;
};

template <>
struct hash<Point> {
    size_t operator()(const Point& p) const noexcept {
        std::size_t h1 = std::hash<int>{}(p.i);
        std::size_t h2 = std::hash<int>{}(p.j);
        return h1 ^ (h2 << 1); // or use boost::hash_combine
    }
};

struct Matrix {
    vector<string> lines;
    Point robot;
    int n_row;
    int n_col;
    bool part2;

    Matrix(const vector<string>& lines, bool part2 = false) : lines (lines), part2 (part2) {
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
                        Point next = p.move(dir);
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
                            Point next = p.move(dir);
                            swap(get(next), get(p));
                        }
                    }
                    robot = robot.move(dir);
                }
            }
        }
        else {
            Point end_of_line = robot.move(dir);
            while (get(end_of_line) == 'O' || get(end_of_line) == '[' || get(end_of_line) == ']') {
                end_of_line = end_of_line.move(dir);    
            }
            char end_char = get(end_of_line);
            if (end_char == '.') {
                Direction opp_dir = mirror(dir);
                while (end_of_line != robot) {
                    Point next = end_of_line.move(opp_dir);
                    swap(get(end_of_line), get(next));
                    end_of_line = next;
                }
                robot = robot.move(dir);
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


auto get_input(bool part2 = false) {
    vector<Direction> moves;
    vector<string> lines;

    ifstream file("../../input.txt");
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

uint64_t solve(bool part2 = false) {
    auto [mat, moves] = get_input(part2);
    for (auto move : moves) {
        mat.make_a_move(move);
    }
    return mat.calculate_gps();
}

int main () {
    cout << solve() << " " << solve(true) << endl;
}
