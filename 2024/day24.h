#pragma once
#include "useful.h"

struct Day24 : public Solution {
    enum LogicOperator {
        logic_none, logic_and, logic_or, logic_xor
    };

    struct LogicGate {
        LogicGate(string op1, string op2, string output, LogicOperator logic) 
            : op1(op1), op2(op2), output(output), logic(logic) {}

        string op1, op2, output;
        LogicOperator logic;

        bool contains_input(const string& input) {
            return op1 == input || op2 == input;
        }
    };

    unordered_map<string, bool> wires;
    vector<LogicGate> gates;
    const int n_bits = 0;

    string get_wire_name(char c, int rank) {
        string str(3, c);
        str[1] = '0' + rank / 10;
        str[2] = '0' + rank % 10;
        return str;
    }

    void get_input(const string& filename) {
        if (gates.size() != 0)
            return;
        
        auto lines = get_input_all_lines(filename);
        auto it = lines.begin();
        for (; !it->empty(); ++it) {
            string name;
            int value;
            istringstream ss(*it);
            getline(ss, name, ':');
            ss.ignore();
            ss >> value;

            wires[name] = (value == 1);
        }
        
        ++it;

        for (;it != lines.end(); ++it) {
            string op1, logic_str, op2, output; 

            istringstream ss(*it);
            ss >> op1 >> logic_str >> op2;
            ss.ignore(4);
            ss >> output;

            LogicOperator logic = logic_none;
            if (logic_str == "AND") logic = logic_and;
            else if (logic_str == "OR") logic = logic_or;
            else logic = logic_xor;

            gates.emplace_back(op1, op2, output, logic);
        }

        const_cast<int&>(n_bits) = int(wires.size() / 2);
    }

    string part1(const string& filename) override {
        get_input(filename);
        int n_z = n_bits + 1;
        vector<string> z_bits(n_z);
        stack<string> to_compute;
        for (int i = 0; i < n_z; i++) {
            z_bits[i] = get_wire_name('z', i);
            to_compute.push(z_bits[i]);
        }

        unordered_map<string, const LogicGate*> gate_outputs;
        for (auto& gate : gates) {
            gate_outputs[gate.output] = &gate;
        }
        
        while (!to_compute.empty()) {
            string& output_str = to_compute.top();
            if (wires.contains(output_str)) {
                to_compute.pop();
                continue;
            }
            
            bool can_compute = true;
            const auto& gate = gate_outputs.at(output_str);
            if (!wires.contains(gate->op1)) {
                to_compute.push(gate->op1);
                can_compute = false;
            }
            if (!wires.contains(gate->op2)) {
                to_compute.push(gate->op2);
                can_compute = false;
            }
            if (can_compute) {
                bool output_val;
                bool v1 = wires.at(gate->op1);
                bool v2 = wires.at(gate->op2);
                switch (gate->logic)
                {
                case logic_and: output_val = v1 && v2; break;
                case logic_or: output_val = v1 || v2; break;
                case logic_xor: output_val = v1 != v2; break;
                default: assert(false); break;
                }

                wires[output_str] = output_val;
                to_compute.pop();
            }
        }

        uint64_t res = 0;
        for (int i = 0; i < z_bits.size(); i++) {
            if (wires[z_bits[i]]) {
                res = res | (uint64_t(1) << i);
            }
        }

        return to_string(res);
    }

    struct GateHelper {
        vector<LogicGate>& gates;
        vector<string> exchanges;
        GateHelper(vector<LogicGate>& gates) : gates(gates) {}

        LogicGate& find_gate(LogicOperator logic, const string& i1, const string& i2, const string& out, int min_eq = 2) {
            for (auto& gate : gates) {
                if (gate.logic == logic) {
                    int count_equal = 0;
                    if (gate.contains_input(i1)) count_equal++;
                    if (gate.contains_input(i2)) count_equal++;
                    if (gate.output == out) count_equal++;
                    if (count_equal >= min_eq)
                        return gate;
                }
            }
            assert(false);
            return gates[0];
        }

        void swap_outputs(const string& a, const string& b) {
            exchanges.push_back(a);
            exchanges.push_back(b);
            LogicGate* g1 = nullptr;
            LogicGate* g2 = nullptr;
            for (auto& gate : gates) {
                if (gate.output == a) {
                    g1 = &gate;
                } 
                else if (gate.output == b) {
                    g2 = &gate;
                }
            }
            swap(g1->output, g2->output);
        }

        void correct_gate(LogicGate& gate, string& i1, string& i2, string& out) {
            if (gate.output != out) {
                swap_outputs(gate.output, out);
            }
            else if (!gate.contains_input(i2)) {
                if (gate.op1 == i1) {
                    swap_outputs(gate.op2, i2);
                    i2 = gate.op2;
                }
                else {
                    swap_outputs(gate.op1, i2);
                    i2 = gate.op1;
                }
            }
            else if (!gate.contains_input(i1)) {
                if (gate.op1 == i2) {
                    swap_outputs(gate.op2, i1);
                    i1 = gate.op2;
                }
                else {
                    swap_outputs(gate.op1, i1);
                    i1 = gate.op1;
                }
            }
        }
    };

    string part2(const string& filename) override {
        get_input(filename);
        auto helper = GateHelper(gates);
        string x, y, z, s, c, t, u, c_next;
        x = get_wire_name('x', 0);
        y = get_wire_name('y', 0);
        z = get_wire_name('z', 0);
        
        LogicGate& z0_gate = helper.find_gate(logic_xor, x, y, z);
        helper.correct_gate(z0_gate, x, y, z);

        LogicGate& c0_gate = helper.find_gate(logic_and, x, y, z);
        c_next = c0_gate.output;

        for (int i = 1; i < n_bits; i++) {
            x = get_wire_name('x', i);
            y = get_wire_name('y', i);
            z = get_wire_name('z', i);
            c = c_next;

            LogicGate& s_gate = helper.find_gate(logic_xor, x, y, "");
            s = s_gate.output;
            
            LogicGate& z_gate = helper.find_gate(logic_xor, s, c, z);
            helper.correct_gate(z_gate, s, c, z);

            LogicGate& t_gate = helper.find_gate(logic_and, s, c, "");
            t = t_gate.output;

            LogicGate& u_gate = helper.find_gate(logic_and, x, y, "");
            u = u_gate.output;

            LogicGate& c_gate = helper.find_gate(logic_or, t, u, "", 1);
            c_next = c_gate.output;
            helper.correct_gate(c_gate, t, u, c_next);
        }

        ranges::sort(helper.exchanges);
        return join_strings(helper.exchanges, ',');
    }
};
