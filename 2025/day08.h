#pragma once
#include "useful.h"

struct Day08 : public SolutionPair {

    static inline constexpr bool debug = true;
    static inline constexpr int N_CLOSEST = debug ? 10 : 1000;

    auto get_input(const string& filename) {
        string f = filename;
        if (debug)
            f = "../test.txt";
        auto lines = get_input_all_lines(f);
        vector<array<int, 3>> points;
        for (const auto& line : lines) {
            istringstream ss(line);
            points.emplace_back();
            char c;
            ss >> points.back()[0] >> c >> points.back()[1] >> c >> points.back()[0];
        }
        return points;
    }

    uint64_t distance(const auto& a, const auto& b) {
        uint64_t res = 0;
        for (int i = 0; i < a.size(); i++) {
            res += (a[i] - b[i]) * (a[i] - b[i]);
        }
        return res;
    }

    struct DistancePoint {
        int p1, p2;
        uint64_t dist;
    };

    pair<string, string> solve(const string& filename) override {
        auto points = get_input(filename);
        uint64_t part1 = 0;
        uint64_t part2 = 0;

        vector<DistancePoint> distances;
        int n = points.size();
        for (int i = 0; i < n; i++) {
            for (int j = i + 1; j < n; j++) {
                distances.emplace_back(i, j, distance(points[i], points[j]));
            }
        }

        ranges::sort(distances, [](const auto& a, const auto& b) { return a.dist < b.dist; });

        int last_tag = 0;
        unordered_map<int, int> tags;
        for (int i = 0; i < N_CLOSEST; i++) {
            auto& p1 = distances[i].p1;
            auto& p2 = distances[i].p2;
            if (tags.contains(p1)) {
                tags[p2] = tags[p1];
            } else if (tags.contains(p2)) {
                tags[p1] = ;
            }
        }

        
        return {to_string(part1), to_string(part2)};
    }
};
