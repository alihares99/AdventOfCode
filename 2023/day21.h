#pragma once
#include "useful.h"

struct Day21 : public SolutionPair {

    static inline constexpr int n_step_part1 = 64;
    static inline constexpr int n_step_part2 = 26501365;

    struct Grid {
        int n;
        Point start;
        vector<string> lines;
        bool is_in_bound(const Point& p) {
            return p.i >= 0 && p.j >= 0 && p.i < n && p.j < n;
        }
        char get(const Point& p) {
            int i = p.i % n;
            int j = p.j % n;
            if (i < 0) i += n;
            if (j < 0) j += n;
            return lines[i][j]; 
        }
    } g;

    void get_input(const string& filename) {
        auto lines = get_input_all_lines(filename);
        assert(lines.size() == lines[0].size());
        g.n = int(lines.size());
        for (int i = 0; i < g.n; i++) {
            for (int j = 0; j < g.n; j++) {
                if (lines[i][j] == 'S') {
                    lines[i][j] = '.';
                    g.start = {i, j};
                }
            }
        }
        g.lines = move(lines);
    }

    struct ReachablePointCounter {
        Grid& g;
        unordered_set<Point> seen;
        unordered_set<Point> border;
        int prev = 1;
        int prev2 = 0;

        ReachablePointCounter(Grid& _g) : g(_g), border({_g.start}) {}

        int get_next() {
            unordered_set<Point> next;
            for (Point p : border) {
                if (Point pp = p; g.get(pp.move(dir_up)) == '.' && !seen.contains(pp))
                    next.insert(pp);
                if (Point pp = p; g.get(pp.move(dir_down)) == '.' && !seen.contains(pp))
                    next.insert(pp);
                if (Point pp = p; g.get(pp.move(dir_right)) == '.' && !seen.contains(pp))
                    next.insert(pp);
                if (Point pp = p; g.get(pp.move(dir_left)) == '.' && !seen.contains(pp))
                    next.insert(pp);
                seen.insert(p);
            }
            border = move(next);
            int res = prev2 + int(border.size());
            prev2 = prev;
            prev = res;
            return res;
        }

    };

    pair<string, string> solve(const string& filename) override {
        get_input(filename);
        uint64_t part1 = 0;
        uint64_t part2 = 0;
        ReachablePointCounter counter(g);
        bool found = false;
        int mod = n_step_part2 % g.n;
        int index = mod;
        int quad_coef = 0;
        int c1 = 0, c2 = 0, c3 = 0, c4 = 0;
        int remaining = 4;
        for (int i = 1; !found; i++) {
            int c = counter.get_next();
            if (i == n_step_part1) {
                part1 = c;
            }
            if (i % g.n == mod) {
                c4 = c3;
                c3 = c2;
                c2 = c1;
                c1 = c;
                remaining--;
                if (remaining == 0) {
                    quad_coef = c4 + c2 - 2 * c3;
                    if (quad_coef == c3 + c1 - 2 * c2) {
                        found = true;
                    }
                    else {
                        index += g.n;
                        remaining ++;
                    }
                }
            }
        }

        assert(n_step_part2 > index);
        uint64_t n = (n_step_part2 - index) / g.n;
        part2 = c4 + n * (c3 - c4) + ((n * (n - 1)) / 2) * quad_coef;
        
        return {to_string(part1), to_string(part2)};
    }
};
