#pragma once
#include "useful.h"

struct Day08 : public SolutionPair {

    auto get_input(const string& filename) {
        ifstream file(filename);
        string instructions;
        getline(file, instructions);

        unordered_map<string, pair<string, string>> mapping;
        string str;
        getline(file, str);
        while (getline(file, str)) {
            istringstream ss(str);
            string s1, s2, s3, tmp;
            ss >> s1;
            ss.ignore(4);
            ss >> s2;
            s2.pop_back();
            ss >> s3;
            s3.pop_back();
            mapping[s1] = make_pair(s2, s3);
        }

        int n_instruction = int(instructions.size());
        unordered_map<string, string> reduced;
        for (auto item : mapping) {
            string in = item.first;
            string out = in;
            for (char c : instructions) {
                if (c == 'L') 
                    out = mapping[out].first;
                else
                    out = mapping[out].second;
            }
            reduced[in] = out;
        }

        return make_pair(n_instruction, reduced);
    }

    struct Sequence {
        uint64_t cycle = -1;
        uint64_t pos_z = -1;

        void fill(const unordered_map<string, string>& mapping, string cur) {
            unordered_map<string, int> seen;
            int count = 0;
            while (!seen.contains(cur)) {
                seen[cur] = count;
                if (cur[2] == 'Z') {
                    if (pos_z == -1)
                        pos_z = count;
                    else
                        assert (false);
                }
                count++;
                cur = mapping.at(cur);
            }
            cycle = count - seen[cur];
        }

    };

    pair<string, string> solve(const string& filename) override {

        auto [jump, mapping] = get_input(filename);
        uint64_t part1 = 0;
        string cur = "AAA";
        while (cur != "ZZZ") {
            part1 += jump;
            cur = mapping[cur];
        }

        // part2:
        vector<Sequence> seqs;
        for (const auto& item : mapping) {
            if (item.first[2] == 'A') {
                seqs.emplace_back();
                seqs.back().fill(mapping, item.first);
            }
        }

        uint64_t lcm_so_far = 1;
        uint64_t part2 = seqs[0].pos_z;
        for (auto& seq : seqs) {
            while (part2 % seq.cycle != seq.pos_z % seq.cycle) {
                part2 += lcm_so_far;
            }
            lcm_so_far = lcm(lcm_so_far, seq.cycle);
        }
        part2 *= jump;

        return {to_string(part1), to_string(part2)};
    }
};
