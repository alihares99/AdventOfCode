#pragma once
#include "useful.h"

struct Day06 : public SolutionPair {

    auto get_input(const string& filename) {
        vector<int> times, distances;

        ifstream file(filename);
        string str;
        getline(file, str);
        {
            istringstream ss(str);
            string tmp;
            ss >> tmp;
            int v;
            while (ss >> v) {
                times.push_back(v);
            }
        }

        getline(file, str);
        {
            istringstream ss(str);
            string tmp;
            ss >> tmp;
            int v;
            while (ss >> v) {
                distances.push_back(v);
            }
        }

        return make_pair(times, distances);
    }

    auto concat_values(vector<int> in) {
        uint64_t out = 0;
        for (auto item : in) {
            auto tmp = item;
            while(tmp /= 10) out *= 10;
            out *= 10;
            out += item;
        }
        return out;
    };

    pair<string, string> solve(const string& filename) override {
        auto [times, distances] = get_input(filename);
        uint64_t res = 1;
        for (int i = 0; i < times.size(); i++) {
            auto time = times[i];
            auto distance = distances[i];
            int n = 0;
            for (int j = 1; j < time; j++) {
                if (j * (time - j) > distance)
                    n++;
            }
            res *= n;
        }

        // part2
        auto time = concat_values(times);
        auto distance = concat_values(distances);
        uint64_t n = 0;
        for (int j = 1; j < time; j++) {
            if (j * (time - j) > distance)
                n++;
        }

        return {to_string(res), to_string(n)};
    }
};
