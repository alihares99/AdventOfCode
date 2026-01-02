#pragma once
#include "useful.h"

struct Day08 : public SolutionPair {

    static inline constexpr bool debug = false;
    static inline constexpr int N_CLOSEST = debug ? 10 : 1000;

    using Point = array<int64_t, 3>;

    auto get_input(const string& filename) {
        string f = filename;
        if (debug)
            f = "../test.txt";
        auto lines = get_input_all_lines(f);
        vector<Point> points;
        for (const auto& line : lines) {
            istringstream ss(line);
            points.emplace_back();
            char c;
            ss >> points.back()[0] >> c >> points.back()[1] >> c >> points.back()[2];
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
        Point *p1, *p2;
        uint64_t dist;
    };

    pair<string, string> solve(const string& filename) override {
        auto points = get_input(filename);
        uint64_t part1 = 1;
        uint64_t part2 = 0;

        vector<DistancePoint> distances;
        int n = points.size();
        for (int i = 0; i < n; i++) {
            for (int j = i + 1; j < n; j++) {
                distances.emplace_back(&points[i], &points[j], distance(points[i], points[j]));
            }
        }

        ranges::sort(distances, [](const auto& a, const auto& b) { return a.dist < b.dist; });

        int last_tag = 0;
        unordered_map<Point*, int> tags;
        unordered_map<int, vector<Point*>> groups;
        for (auto& p : points) {
            last_tag++;
            tags[&p] = last_tag;
            groups[last_tag].push_back(&p);
        }
        for (int i = 0; i < distances.size(); i++) {
            auto& p1 = distances[i].p1;
            auto& p2 = distances[i].p2;
            if (tags[p1] > points.size() && tags[p2] > points.size()) {
                if (tags[p1] != tags[p2]) {
                    int merged_tag = groups[tags[p1]].size() > groups[tags[p2]].size() ? tags[p1] : tags[p2];
                    int other_tag = groups[tags[p1]].size() > groups[tags[p2]].size() ? tags[p2] : tags[p1];
                    for (auto p : groups[other_tag]) {
                        assert(tags[p] == other_tag);
                        tags[p] = merged_tag;
                        groups[merged_tag].push_back(p);
                    }
                    groups.erase(other_tag);
                }
            } else if (tags[p2] > points.size()) {
                groups.erase(tags[p1]);
                tags[p1] = tags[p2];
                groups[tags[p2]].push_back(p1);
            } else if (tags[p1] > points.size()) {
                groups.erase(tags[p2]);
                tags[p2] = tags[p1];
                groups[tags[p1]].push_back(p2);
            } else {
                last_tag++;
                groups.erase(tags[p1]);
                groups.erase(tags[p2]);
                tags[p1] = tags[p2] = last_tag;
                groups[last_tag].push_back(p1);
                groups[last_tag].push_back(p2);
            }

            if (i == N_CLOSEST - 1) {
                vector<int> group_sizes;
                for (auto [tag, group] : groups) {
                    group_sizes.push_back(group.size());
                }
                partial_sort(group_sizes.begin(), group_sizes.begin() + 3, group_sizes.end(), std::greater{});
                for (int i = 0; i < 3; i++) {
                    part1 *= group_sizes[i];
                }
            }

            if (groups.size() == 1) {
                part2 = (*p1)[0] * (*p2)[0];
                break;
            }
        }
        
        return {to_string(part1), to_string(part2)};
    }
};
