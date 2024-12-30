#pragma once
#include "useful.h"

struct Day17 : public Solution {
    struct CPU {
        uint64_t regA;
        uint64_t regB;
        uint64_t regC;
        int ic = 0; // instruction_pointer
        vector<int> program;

        uint64_t& get_combo(uint64_t& operand) {
            if (operand < 4) return operand;
            else if (operand == 4) return regA;
            else if (operand == 5) return regB;
            else if (operand == 6) return regC;
            else assert(false);
            return operand;
        }

        void execute(int opcode, uint64_t operand, vector<int>& output) {
            switch (opcode)
            {   
            case 0: regA = regA >> get_combo(operand); break;
            case 1: regB = regB ^ operand; break;
            case 2: regB = get_combo(operand) % 8; break;
            case 3: if (regA != 0) ic = int(operand) - 2; break;
            case 4: regB = regB ^ regC; break;
            case 5: output.push_back(get_combo(operand) % 8); break;
            case 6: regB = regA >> get_combo(operand); break;
            case 7: regC = regA >> get_combo(operand); break;
            default: assert(false);
            }
        }

        vector<int> run_all(uint64_t reg_a, optional<int> max_output_count = nullopt) {
            vector<int> output;
            regA = reg_a;
            regB = 0;
            regC = 0;
            ic = 0;

            while(ic < int (program.size()) - 1) {
                execute(program[ic], program[ic + 1], output);
                if (max_output_count && output.size() >= *max_output_count)
                    return output;
                ic += 2;
            }
            return output;
        }
    };

    CPU get_input(const string& filename) {
        CPU cpu;
        auto lines = get_input_all_lines(filename);
        istringstream ss(lines[0]);
        ss.ignore(12);
        ss >> cpu.regA;
        ss = istringstream(lines[1]);
        ss.ignore(12);
        ss >> cpu.regB;
        ss = istringstream(lines[2]);
        ss.ignore(12);
        ss >> cpu.regC;

        ss = istringstream(lines[4]);
        ss.ignore(9);
        int v;
        while (ss >> v) {
            cpu.program.push_back(v);
            ss.ignore();
        }

        return cpu;
    }

    string part1(const string& filename) override {
        CPU cpu = get_input(filename);
        auto out = cpu.run_all(cpu.regA);
        vector<string> out_str(out.size());
        for (int i = 0; i < out.size(); i++) {
            out_str[i] = to_string(out[i]);
        }
        return join_strings(out_str, ',');
    }

    string part2(const string& filename) override {
        CPU cpu = get_input(filename);

        // extra assumption for part2:
        bool reg_a_shifted_3 = false;
        for(int i = 0; i < cpu.program.size(); i+=2) {
            if (cpu.program[i] == 0 && cpu.program[i + 1] == 3) {
                reg_a_shifted_3 = true;
            }
        }
        assert (reg_a_shifted_3);

        // part2:
        int n_match = 0;
        vector<uint64_t> prev_matches = { 0 };
        while (n_match < cpu.program.size()) {
            vector<uint64_t> matches;
            for (auto m : prev_matches) {
                for (int i = 0; i < 8; i++) {
                    uint64_t cur = (m << 3) + i;
                    auto out = cpu.run_all(cur, 1);
                    if (out[0] == cpu.program[cpu.program.size() - 1 - n_match]) {
                        matches.push_back(cur);
                    }
                }
            }
            prev_matches = move(matches);
            n_match++;
        }

        return to_string(*ranges::min_element(prev_matches));
    }
};
