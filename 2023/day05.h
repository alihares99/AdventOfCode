#pragma once
#include "useful.h"

struct Day05 : public SolutionPair {
    struct Destination {
        uint64_t dest;
        uint64_t n;
    };

    struct Range {
        uint64_t start;
        uint64_t len;

        bool operator<(const Range& other) {
            return start < other.start;
        }
    };

    struct Map {
        map<uint64_t, Destination> m;

        void add(uint64_t a, uint64_t b, uint64_t c) {
            m[b] = {a, c};
        }

        uint64_t apply(uint64_t val) {
            auto it = m.upper_bound(val);
            if (it != m.begin()) {
                advance(it, -1);
                auto& source_start = it->first;
                auto& destination = it->second;
                if (val < source_start + destination.n) {
                    return val + destination.dest - source_start;
                }
            }

            return val;
        }

        vector<Range> apply(Range in) {
            vector<Range> res;
            auto it = m.upper_bound(in.start);
            if (it != m.begin()) {
                it = prev(it);
            }

            while (in.len > 0 && it != m.end()) {
                auto& source_start = it->first;
                auto& destination = it->second;
                if (source_start <= in.start && in.start < source_start + destination.n) {
                    Range tmp;
                    tmp.start = in.start + destination.dest - source_start;
                    tmp.len = min(in.len, source_start + destination.n - in.start);
                    res.push_back(tmp);
                    in.start += tmp.len;
                    in.len -= tmp.len;
                    it = next(it);
                }
                else {
                    if (source_start <= in.start) {
                        it = next(it);
                        continue;
                    }

                    Range tmp;
                    tmp.start = in.start;
                    tmp.len = min(in.len, source_start - in.start);
                    res.push_back(tmp);
                    in.start += tmp.len;
                    in.len -= tmp.len;
                }
            }

            if (in.len > 0)
                res.push_back(in);
            
            return res;
        }

    };

    auto get_inputs(const string& filename) {
        ifstream file(filename);
        vector<uint64_t> seeds;
        string str;
        if (getline(file, str)) {
            istringstream ss(str);
            
            string tmp;
            ss >> tmp;
            uint64_t v;
            while(ss >> v) {
                seeds.push_back(v);
            }
        }

        vector<Map> maps;
        while (getline(file, str)) {
            if(str.size() == 0) {
                maps.push_back(Map());
                getline(file, str);
                continue;
            }

            istringstream ss(str);
            uint64_t a, b, c;
            if (ss >> a >> b >> c) {
                maps.back().add(a, b, c);
            }
        }

        return make_pair(seeds, maps);
    };

    pair<string, string> solve(const string& filename) override {
        auto [seeds, maps] = get_inputs(filename);

        // part2:
        vector<Range> input_ranges;
        for (int i = 0; i < seeds.size(); i += 2) {
            Range tmp;
            tmp.start = seeds[i];
            tmp.len = seeds[i + 1];
            input_ranges.push_back(tmp);
        }

        for (auto& map : maps) {
            vector<Range> res;
            for (auto range : input_ranges) {
                auto partial_res = map.apply(range);
                res.insert(res.end(), partial_res.begin(), partial_res.end());
            }
            sort(res.begin(), res.end());
            input_ranges = move(res);
        }
        auto part2 = input_ranges[0].start;

        // part1
        for (auto& map : maps) {
            for (auto& seed : seeds) {
                seed = map.apply(seed);
            }
        }
        auto part1 = *min_element(seeds.begin(), seeds.end());

        return {to_string(part1), to_string(part2)};
    }
};

