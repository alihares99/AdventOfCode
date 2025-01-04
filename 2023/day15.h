#pragma once
#include "useful.h"

struct Day15 : public SolutionPair {

    uint8_t compute_hash(const string& in) {
        uint8_t res = 0;
        for (auto c : in) {
            res += c;
            res *= 17;
        }
        return res;
    }

    struct Instruction {
        string name;
        optional<int> focal_len;
    };

    Instruction parse(const string& in) {
        Instruction res;
        if (in.back() == '-') {
            res.name = in.substr(0, in.size() - 1);
        }
        else {
            int eq = int(in.find_last_of('='));
            res.name = in.substr(0, eq);
            res.focal_len = stoi(in.substr(eq + 1, in.size() - eq - 1));
        }
        return res;
    }

    auto get_input(const string& filename) {
        auto lines = get_input_all_lines(filename);
        auto line = join_strings(lines);
        vector<string> strs;
        strs.reserve(10000);
        istringstream ss (line);
        string str;
        while (getline(ss, str, ','))
            strs.push_back(str);
        return strs;
    }

    pair<string, string> solve(const string& filename) override {
        auto inputs = get_input(filename);
        uint64_t part1 = 0;
        uint64_t part2 = 0;

        for (auto str : inputs) {
            part1 += compute_hash(str);
        }

        // part2:
        vector<Instruction> instructions;
        instructions.reserve(inputs.size());
        for (auto& str : inputs) {
            instructions.push_back(parse(str));
        }

        array<forward_list<pair<string, int>>, 256> hash_map;
        for (auto& instr : instructions) {
            auto& box = hash_map[compute_hash(instr.name)];
            bool done = false;
            auto it_prev = box.before_begin();
            for (auto it = box.begin(); it != box.end(); it_prev = it++) {
                if (it->first == instr.name) {
                    if (instr.focal_len.has_value()) 
                        it->second = *instr.focal_len;
                    else 
                        box.erase_after(it_prev);
                    done = true;
                    break;
                }
            }
            if (!done && instr.focal_len.has_value())
                box.insert_after(it_prev, make_pair(instr.name, *instr.focal_len));
        }

        for (int i = 0; i < 256; i++) {
            int j = 0;
            for (auto& lenz : hash_map[i]) {
                part2 += (i + 1) * (++j) * lenz.second;
            }
        }
        
        return {to_string(part1), to_string(part2)};
    }
};
