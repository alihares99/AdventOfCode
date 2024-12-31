#pragma once
#include "useful.h"

struct Day10 : public SolutionPair {
    struct Pipe {
        int directions = 0;
        char c;

        Pipe(char c) {
            this->c = c;
            switch (c)
            {
            case '|': directions = dirflag_down | dirflag_up; break;
            case '-': directions = dirflag_left | dirflag_right; break;
            case '7': directions = dirflag_left | dirflag_down; break;
            case 'F': directions = dirflag_right | dirflag_down; break;
            case 'L': directions = dirflag_up| dirflag_right; break;
            case 'J': directions = dirflag_up | dirflag_left; break;
            }
        }

        bool do_you_have(DirectionFlag dir) {
            return (directions & dir) > 0;
        }

        DirectionFlag get_other_direction(DirectionFlag dir) {
            return DirectionFlag(directions ^ int(dir));
        }
    };

    struct Matrix {
        vector<vector<Pipe>> pipes;
        Point start;

        struct iterator {
            int i, j;
            DirectionFlag to;
            DirectionFlag from;
            Matrix* parent;

            iterator& operator++() {
                switch (to)
                {
                case dirflag_up: i -= 1; break;
                case dirflag_down: i += 1; break;
                case dirflag_left: j -= 1; break;
                case dirflag_right: j += 1; break;
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
            for (DirectionFlag d : {dirflag_up, dirflag_right, dirflag_down, dirflag_left}) {
                if (d & dirs) {
                    it.to = d;
                    break;
                }
            }
            it.from = DirectionFlag(dirs ^ it.to);
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
            vector<DirectionFlag> dirs;
            for (auto dir : {dirflag_left, dirflag_down, dirflag_right, dirflag_up}) {
                it.to = dir;
                auto tmp = it;
                ++tmp;
                if (tmp.is_valid()) {
                    dirs.push_back(dir);
                }
            }
            if (dirs.size() == 2) {
                DirectionFlag start_dir = DirectionFlag(dirs[0] | dirs[1]);
                pipes[start.i][start.j].directions = start_dir;
            }
        }
    };

    auto get_input(const string& filename) {
        ifstream file(filename);
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

    pair<string, string> solve(const string& filename) override {
        auto grid = get_input(filename);
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

        // part2:
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
                    if (dir == (dirflag_down | dirflag_up) || 
                        dir == (dirflag_down | dirflag_right) || 
                        dir == (dirflag_down | dirflag_left))
                        junction++;
                }
                else if (junction % 2 == 1) {
                    n_enclosed++;
                }
            }
        }

        return {to_string(furthest), to_string(n_enclosed)};
    }
};
