#pragma once
#include "useful.h"

struct Day14 : public SolutionPair {

    using Grid = vector<string>;

    template<int direction>
    struct Move {
        void operator()(Point& p) {
            if constexpr (direction == 0) {p.i--;}
            if constexpr (direction == 1) {p.j--;}
            if constexpr (direction == 2) {p.i++;}
            if constexpr (direction == 3) {p.j++;}
        }
    };

    template<int direction>
    void roll(Grid& grid) {
        for (int i = 0; i < grid.size(); i++) {
            for (int j = 0; j < grid[0].size(); j++) {
                if (grid[i][j] == 'O') {
                    Point swap_me {i, j};
                    Point p = swap_me;
                    while (p.i >= 0 && p.j >= 0 && p.i < grid.size() && p.j < grid[0].size()) {
                        if (grid[p.i][p.j] == '#')
                            break;
                        if (grid[p.i][p.j] == '.')
                            swap_me = p;
                        Move<direction>{}(p);
                    }
                    if (swap_me.i != i || swap_me.j != j)
                        swap(grid[i][j], grid[swap_me.i][swap_me.j]);
                }
            }
        }
    }

    int calc(const Grid& grid) {
        int res = 0;
        const auto n_row = int(grid.size());
        const auto n_col = int(grid[0].size());
        for (int i = 0; i < n_row; i++) {
            for (int j = 0; j < n_col; j++) {
                if (grid[i][j] == 'O') 
                    res += n_row - i;
            }
        }
        return res;
    }

    pair<string, string> solve(const string& filename) override {
        Grid grid = get_input_all_lines(filename);
        int part1 = 0;
        int part2 = 0;

        roll<0>(grid);
        part1 = calc(grid);

        // part2:
        vector<int> seen;
        seen.reserve(1000);
        bool done = false;
        int first = -1;
        int cycle = -1;
        int success = 0;
        while (!done) {
            roll<0>(grid);
            roll<1>(grid);
            roll<2>(grid);
            roll<3>(grid);
            seen.push_back(calc(grid));
            if (first == -1) {
                for (int i = int(seen.size()) - 2; i >= 0; i--) {
                    if (seen[i] == seen.back()) {
                        first = i;
                        cycle = int(seen.size()) - 1 - i;
                        break;
                    }
                }
            }
            else {
                if (seen.back() == seen[seen.size() - 1 - cycle]) {
                    success++;
                    if (success == cycle) {
                        done = true;
                    }
                } 
                else {
                    first = -1;
                    success = 0;
                }
            }
        }
        part2 = seen[((1000000000 - 1 - first) % cycle) + first];
        
        return {to_string(part1), to_string(part2)};
    }
};
