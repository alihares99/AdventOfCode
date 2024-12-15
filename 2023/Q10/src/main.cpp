#include <iostream>
#include <fstream>
#include <cassert>
#include <algorithm>
#include <numeric>
#include <vector>
#include <array>
#include <string>
#include <regex>
#include <sstream>
#include <optional>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <stack>
#include <queue>

using namespace std;

enum Direction : int {dir_up = 1, dir_right = 2, dir_down = 4, dir_left = 8};

Direction mirror(Direction dir) {
    return Direction(0xF & (dir >> 2 | dir << 2));
}

struct Pipe {
    int directions = 0;
    char c;

    Pipe(char c) {
        this->c = c;
        switch (c)
        {
        case '|': directions = dir_down | dir_up; break;
        case '-': directions = dir_left | dir_right; break;
        case '7': directions = dir_left | dir_down; break;
        case 'F': directions = dir_right | dir_down; break;
        case 'L': directions = dir_up| dir_right; break;
        case 'J': directions = dir_up | dir_left; break;
        }
    }

    bool do_you_have(Direction dir) {
        return (directions & dir) > 0;
    }

    Direction get_other_direction(Direction dir) {
        return Direction(directions ^ int(dir));
    }
};

struct Point {
    int i = 0, j = 0;
    bool operator==(const Point&) const = default;
};

template <>
struct std::hash<Point> {
    std::size_t operator()(const Point& s) const noexcept {
        std::size_t h1 = std::hash<int>{}(s.i);
        std::size_t h2 = std::hash<int>{}(s.j);
        return h1 ^ (h2 << 1); // or use boost::hash_combine
    }
};

struct Matrix {
    vector<vector<Pipe>> pipes;
    Point start;

    struct iterator {
        int i, j;
        Direction to;
        Direction from;
        Matrix* parent;

        iterator& operator++() {
            switch (to)
            {
            case dir_up: i -= 1; break;
            case dir_down: i += 1; break;
            case dir_left: j -= 1; break;
            case dir_right: j += 1; break;
            }

            // mirror the direction:
            from = mirror(to);
            if (i == parent->start.i && j == parent->start.j) {
                i = -1;
                j = -1;
            }
            else if (is_valid() && parent->pipes[i][j].do_you_have(from)) {
                to = parent->pipes[i][j].get_other_direction(from);
            }
            else {
                i = -1;
                j = -1;
            }

            return *this;
        }

        bool operator==(iterator& other) {
            return i == other.i && j == other.j;
        }

        bool is_valid() {
            return i >= 0 && j >= 0 && i < parent->pipes.size() && j < parent->pipes[0].size();
        }

        void swap_direction() {
            swap(from, to);
        }
    };

    iterator begin() {
        iterator it;
        it.parent = this;
        it.i = start.i;
        it.j = start.j;
        int dirs = pipes[it.i][it.j].directions;
        for (Direction d : {dir_up, dir_right, dir_down, dir_left}) {
            if (d & dirs) {
                it.to = d;
                break;
            }
        }
        it.from = Direction(dirs ^ it.to);
        return it;
    }

    iterator end() {
        iterator it;
        it.i = -1;
        it.j = -1;
        return it;
    }

    void decide_S_directions() {
        iterator it;
        it.parent = this;
        it.i = start.i;
        it.j = start.j;
        vector<Direction> dirs;
        for (auto dir : {dir_left, dir_down, dir_right, dir_up}) {
            it.to = dir;
            auto tmp = it;
            ++tmp;
            if (tmp.is_valid()) {
                dirs.push_back(dir);
            }
        }
        if (dirs.size() == 2) {
            Direction start_dir = Direction(dirs[0] | dirs[1]);
            pipes[start.i][start.j].directions = start_dir;
        }
    }
};

auto get_input() {
    ifstream file("../../input.txt");
    Matrix grid;
    grid.pipes.reserve(200);

    string str;
    while (getline(file, str)) {
        vector<Pipe> line_of_pipes;
        line_of_pipes.reserve(200);
        for (char c : str) {
            if (c == 'S') {
                grid.start.i = int(grid.pipes.size());
                grid.start.j = int(line_of_pipes.size());
            }
            line_of_pipes.emplace_back(c);
        }
        grid.pipes.push_back(line_of_pipes);
    }
    grid.decide_S_directions();

    return grid;
}

int solve() {
    auto grid = get_input();
    auto it1 = grid.begin();
    auto it2 = it1; it2.swap_direction();
    int furthest = 0;
    do {
        ++it1;
        if (it1 != it2) {
            ++it2;
            ++furthest;
        }
    } while (it1 != it2);
    return furthest;
}

int solve2() {
    auto grid = get_input();
    unordered_set<Point> path;
    for (auto it = grid.begin(); it.is_valid(); ++it) {
        path.insert(Point{it.i, it.j});
    }

    int n_enclosed = 0;
    for (int i = 0; i < grid.pipes.size(); i++) {
        int junction = 0;
        int jj = 0;
        for (int j = 0; j < grid.pipes[0].size(); j++) {
            Point p;
            p.i = i;
            p.j = j;
            if (path.contains(p)) {
                int dir = grid.pipes[i][j].directions;
                if (dir == (dir_down | dir_up) || 
                    dir == (dir_down | dir_right) || 
                    dir == (dir_down | dir_left))
                    junction++;
            }
            else if (junction % 2 == 1) {
                n_enclosed++;
            }
        }
    }

    return n_enclosed;
}

int main() {
    cout << solve() << " " << solve2() << endl;
    return 0;

}