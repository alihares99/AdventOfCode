#pragma once
#include "useful.h"

struct Day16 : public SolutionPair {

    struct iterator;

    struct Grid {
        vector<string> lines;
        Grid(vector<string>&& m) : lines(move(m)) {}

        struct iterator {
            Grid& g;
            DirectedPoint p;
            iterator(Grid& grid, const DirectedPoint& point) : g(grid), p(point) {}
            bool is_valid() { return p.i >= 0 && p.j >= 0 && p.i < g.lines.size() && p.j < g.lines[0].size(); }
            optional<iterator> move() {
                optional<iterator> res;
                switch (g.lines[p.i][p.j])
                {
                case '/':
                    if (p.dir == dir_right || p.dir == dir_left)
                        p.turn_left();
                    else
                        p.turn_right();
                    break;
                case '\\':
                    if (p.dir == dir_up || p.dir == dir_down)
                        p.turn_left();
                    else
                        p.turn_right();
                    break;
                case '-':
                    if (p.dir == dir_up || p.dir == dir_down) {
                        auto pp = p;
                        p.turn_left();
                        pp.turn_right();
                        res.emplace(g, pp);
                    }
                    break;
                case '|':
                    if (p.dir == dir_left || p.dir == dir_right) {
                        auto pp = p;
                        p.turn_left();
                        pp.turn_right();
                        res.emplace(g, pp);
                    }
                default:
                    break;
                }
                p.move();
                if (res.has_value())
                    res->move();
                return res;
            }
        };
    };

    auto get_input(const string& filename) {
        auto lines = get_input_all_lines(filename);
        return Grid(move(lines));
    }

    int count_energized(Grid& grid, Grid::iterator start) {
        unordered_set<DirectedPoint> seen;
        queue<Grid::iterator> its;
        its.push(start);
        while (!its.empty()) {
            auto it = its.front();
            its.pop();
            while (it.is_valid()) {
                if (seen.contains(it.p))
                    break;
                seen.insert(it.p);
                auto next = it.move();
                if (next.has_value())
                    its.push(move(*next));
            }
        }

        unordered_set<Point> ss;
        ss.reserve(seen.size());
        for (auto& item : seen) {
            ss.insert(item);
        }
        return int(ss.size());
    }

    pair<string, string> solve(const string& filename) override {
        auto grid = get_input(filename);

        vector<int> results;
        results.reserve((grid.lines.size() + grid.lines[0].size()) * 2);
        for (int i = 0; i < grid.lines.size(); i++) {
            results.push_back(count_energized(grid, {grid, {{i, 0}, dir_right}}));
            results.push_back(count_energized(grid, {grid, {{i, int(grid.lines[0].size()) - 1}, dir_right}}));
        }
        for (int i = 0; i < grid.lines[0].size(); i++) {
            results.push_back(count_energized(grid, {grid, {{0, i}, dir_right}}));
            results.push_back(count_energized(grid, {grid, {{int(grid.lines.size()) - 1, i}, dir_right}}));
        }
        
        int part1 = results[0];
        int part2 = *ranges::max_element(results);

        return {to_string(part1), to_string(part2)};
    }
};
