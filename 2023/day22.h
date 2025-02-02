#pragma once
#include "useful.h"

struct Day22 : public SolutionPair {

    struct Brick {
        array<int, 3> start;
        int dim = -1;
        int len = -1;
    };

    struct CompZAxis {
        bool operator()(const Brick& a, const Brick& b) {
            return a.start[2] < b.start[2];
        }
    };

    auto get_input(const string& filename) {
        auto lines = get_input_all_lines(filename);
        vector<Brick> bricks;
        bricks.reserve(lines.size());
        for (auto& line : lines) {
            istringstream ss(line);
            array<int, 6> nums;
            for (int i = 0; i < 6; i++) {
                ss >> nums[i];
                ss.ignore();
            }
            Brick b;
            for (int i = 0; i < 3; i++) {
                b.start[i] = min(nums[i], nums[i + 3]);
                if (nums[i] != nums[i + 3]) {
                    b.dim = i;
                    b.len = abs(nums[i] - nums[i + 3]);
                }
            }
            bricks.push_back(b);
        }

        return bricks;
    }

    template <class T>
    bool is_subset(const unordered_set<T>& small, const unordered_set<T>& large) {
        for (auto& item : small) {
            if (!large.contains(item))
                return false;
        }
        return true;
    }

    pair<string, string> solve(const string& filename) override {
        auto bricks = get_input(filename);
        
        ranges::sort(bricks, CompZAxis{});
        unordered_map<Point, pair<Brick*, int>> heights;
        unordered_map<Brick*, unordered_set<Brick*>> tops;
        unordered_map<Brick*, unordered_set<Brick*>> bottoms;
        for (auto& b : bricks) {
            vector<Point> points {{b.start[0], b.start[1]}};
            if (b.dim != 2) {
                for (int j = 0; j < b.len; j++) {
                    Point p = points[0];
                    if (b.dim == 0)
                        p.i += j + 1;
                    else
                        p.j += j + 1;
                    points.push_back(p);
                }
            }
            int height_under_me = heights[points[0]].second;
            for (int i = 1; i < points.size(); i++)
                height_under_me = max(height_under_me, heights[points[i]].second);
            
            for (auto& p : points) {
                if (heights[p].second == height_under_me) {
                    tops[heights[p].first].insert(&b);
                    bottoms[&b].insert(heights[p].first);
                }
                heights[p].first = &b;
                heights[p].second = height_under_me + 1 + (b.dim == 2 ? b.len : 0);
            }
        }

        uint64_t part1 = 0;
        uint64_t part2 = 0;

        for (auto& b : bricks) {
            unordered_set<Brick*> dropped {&b};
            bool new_item_dropped = true;
            while (new_item_dropped) {
                new_item_dropped = false;
                unordered_set<Brick*> next_row;
                for (auto& item : dropped) {
                    for (auto item2 : tops[item]) {
                        if (!dropped.contains(item2))
                            next_row.insert(item2);
                    }
                }
                for (auto& item : next_row) {
                    if (is_subset(bottoms[item], dropped)) {
                        bool success = dropped.insert(item).second;
                        new_item_dropped = new_item_dropped || success;
                    }
                }
            }
            if (dropped.size() == 1)
                part1++;
            else
                part2 += dropped.size() - 1;
        }
        
        return {to_string(part1), to_string(part2)};
    }
};
