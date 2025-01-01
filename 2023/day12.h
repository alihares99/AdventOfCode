#pragma once
#include "useful.h"

struct Day12 : public SolutionPair {

    struct Data {
        string str;
        vector<int> brokens;
    };

    auto get_input(const string& filename) {
        auto lines = get_input_all_lines(filename);
        vector<Data> inputs;
        inputs.reserve(lines.size());
        for (int i = 0; i < lines.size(); i++) {
            istringstream ss(lines[i]);
            inputs.emplace_back();
            getline(ss, inputs.back().str, ' ');
            int v;
            while (ss >> v) {
                inputs.back().brokens.push_back(v);
                ss.ignore();
            }
        }
        return inputs;
    }

    uint64_t calc(const string& str, vector<int> brokens) {
        vector<int> biggest;
        biggest.reserve(str.size());
        for (int i = 0; i < str.size(); i++) {
            if (i > 0 && biggest.back() > 0) {
                biggest.push_back(biggest.back() - 1);
                continue;
            }

            int count = 0;
            for (int j = i; j < str.size(); j++) {
                if (str[j] == '#' || str[j] == '?')
                    count++;
                else
                    break;
            }
            biggest.push_back(count);
        }

        map<int, uint64_t> placements;
        placements[0] = 1;
        for (auto b : brokens) {
            if (placements.empty())
                break;

            map<int, uint64_t> tmp;
            for (auto& item : placements) {
                for (int i = item.first; i < str.size(); i++) {
                    if (str[i] != '.' && biggest[i] >= b && 
                        (i + b >= str.size() || str[i + b] == '.' || str[i + b] == '?'))
                    {
                        tmp[i + b + 1] += item.second;
                    }

                    if (str[i] == '#')
                        break;
                }
            }

            placements = move(tmp);
        }

        int last_broken = int(str.find_last_of('#'));            
        last_broken += 2;
        uint64_t count = 0;
        for (auto& item : placements) {
            if (item.first >= last_broken)
                count += item.second;
        }
        return count;
    }

    pair<string, string> solve(const string& filename) override {
        auto inputs = get_input(filename);
        uint64_t part1 = 0;
        uint64_t part2 = 0;
        for (const auto& data : inputs) {
            part1 += calc(data.str, data.brokens);
            part2 += calc(repeat<char>(data.str, 5, '?'), repeat<int>(data.brokens, 5));
        }
        
        return {to_string(part1), to_string(part2)};
    }
};
