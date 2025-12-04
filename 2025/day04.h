#pragma once
#include "useful.h"

struct Day04 : public SolutionPair {

    static inline constexpr bool debug = false;

    auto get_input(const string& filename) {
        string f = filename;
        if (debug)
            f = "../test.txt";
        auto lines = get_input_all_lines(f);
        return lines;
    }

    int is_paper_roll(const vector<string>& grid, int i, int j) {
        if (i < 0 || i >= grid.size() || j < 0 || j >= grid[0].size())
            return 0;
        return grid[i][j] == '@' ? 1 : 0;
    }

    bool is_removable(const vector<string>& grid, int i, int j) {
        int c = is_paper_roll(grid, i - 1, j - 1) 
              + is_paper_roll(grid, i - 1, j    ) 
              + is_paper_roll(grid, i - 1, j + 1) 
              + is_paper_roll(grid, i    , j - 1) 
              + is_paper_roll(grid, i    , j + 1) 
              + is_paper_roll(grid, i + 1, j - 1) 
              + is_paper_roll(grid, i + 1, j    ) 
              + is_paper_roll(grid, i + 1, j + 1);
        return c < 4;
    }

    pair<string, string> solve(const string& filename) override {
        auto grid = get_input(filename);
        uint64_t part1 = 0;
        uint64_t part2 = 0;
        
        int rows = grid.size();
        int cols = grid[0].size();
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                if (grid[i][j] != '@') 
                    continue;
                
                if (is_removable(grid, i, j))
                    part1++;
            }
        }

        bool removed_one = true;
        while (removed_one) {
            removed_one = false;
            for (int i = 0; i < rows; i++) {
                for (int j = 0; j < cols; j++) {
                    if (grid[i][j] != '@') 
                        continue;
                    
                    if (is_removable(grid, i, j)) {
                        part2++;
                        grid[i][j] = '.';
                        removed_one = true;
                    }
                }
            }
        }

        return {to_string(part1), to_string(part2)};
    }
};
