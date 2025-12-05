#pragma once
#include "useful.h"

struct Day05 : public SolutionPair {

    static inline constexpr bool debug = false;

    auto get_input(const string& filename) {
        string f = filename;
        if (debug)
            f = "../test.txt";
        auto lines = get_input_all_lines(f);

        vector<pair<uint64_t, uint64_t>> pairs;
        vector<uint64_t> nums;
        int i = 0;
        for (; i < lines.size(); i++) {
            auto& line = lines[i];
            if (line.size() == 0)
                break;
            int p = line.find('-');
            pairs.push_back(make_pair(stoll(line.substr(0, p)), stoll(line.substr(p + 1, -1))));
        }
        for (i++; i < lines.size(); i++) {
            nums.push_back(stoll(lines[i]));
        }
        return make_tuple(pairs, nums);
    }

    pair<string, string> solve(const string& filename) override {
        auto [pairs, nums] = get_input(filename);
        uint64_t part1 = 0;
        uint64_t part2 = 0;

        ranges::sort(pairs, [](const auto& a, const auto& b) { return a.first < b.first; });

        map<uint64_t, uint64_t> ranges;
        for (auto [p1, p2] : pairs) {
            auto it = ranges.end();
            if (it == ranges.begin()) {
                ranges.insert(ranges.end(), make_pair(p1, p2));
            }
            else {
                it = prev(it);
                if (it->second < p1 - 1) {
                    ranges.insert(ranges.end(), make_pair(p1, p2));
                } else {
                    it->second = max(p2, it->second);
                }
            }
        }

        for (auto num : nums) {
            auto it = ranges.upper_bound(num);
            if (it != ranges.begin()) {
                it = prev(it);
                if (it->second >= num)
                    part1++;
            }
        }

        // part2:
        for (auto [p1, p2] : ranges) {
            part2 += p2 - p1 + 1;
        }
        
        return {to_string(part1), to_string(part2)};
    }
};
