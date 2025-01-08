#pragma once
#include "useful.h"

struct Day18 : public SolutionPair {

    struct Data {
        int n;
        Direction dir;
    };

    auto get_input(const string& filename) {
        auto lines = get_input_all_lines(filename);
        vector<Data> data;
        vector<Data> data2;
        data.reserve(lines.size());
        data2.reserve(lines.size());
        for (auto& line : lines) {
            Data d;
            Data d2;
            switch(line[0]) {
                case 'U': d.dir = dir_up; break;
                case 'D': d.dir = dir_down; break; 
                case 'L': d.dir = dir_left; break;
                case 'R': d.dir = dir_right; break;
            }
            sscanf_s(line.c_str() + 2, "%d (#%x)", &d.n, &d2.n);
            switch(d2.n & 0xF) {
                case 0: d2.dir = dir_right; break;
                case 1: d2.dir = dir_down; break;
                case 2: d2.dir = dir_left; break;
                case 3: d2.dir = dir_up; break;
            }
            d2.n = d2.n >> 4;
            data.push_back(d);
            data2.push_back(d2);
        }
        return make_pair(data, data2);
    }

    uint64_t get_lagoon_size(vector<Data>& data) {
        int n_clock_wise_rotations = 0;
        for (int i = 0; i < data.size(); i++) {
            int j = (i + 1) % data.size();
            n_clock_wise_rotations += data[j].dir == rotate_clockwise(data[i].dir) ? 1 : -1;
        }
        bool clockwise = n_clock_wise_rotations == 4;
        uint64_t res = 1;
        int64_t x = 1;
        for (auto& d : data) {
            switch (d.dir)
            {
            case dir_right: x += d.n; res += d.n; break;
            case dir_left: x -= d.n; break;
            case dir_down: 
            case dir_up:
                if (clockwise != (d.dir == dir_up)) 
                    res += x * d.n; 
                else 
                    res -= (x - 1) * d.n; 
                break;
            }
        }
        
        return res;
    }

    pair<string, string> solve(const string& filename) override {
        auto [data, data2] = get_input(filename);
        int64_t part1 = get_lagoon_size(data);
        int64_t part2 = get_lagoon_size(data2);
        return {to_string(part1), to_string(part2)};
    }
};
