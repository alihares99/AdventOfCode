#pragma once
#include "useful.h"

struct Day20 : public SolutionPair {

    enum Type { type_broadcaster, type_fligflop, type_conjunction };

    struct Module {
        Type type = type_broadcaster;
        vector<string> outputs;
        vector<string> inputs;
    };

    unordered_map<string, Module> network;

    void get_input(const string& filename) {
        network.clear();
        auto lines = get_input_all_lines(filename);
        for (auto& line : lines) {
            string name;
            string connection_name;
            istringstream ss(line);
            Type type = type_broadcaster;
            if (ss.peek() == '%') {
                type = type_fligflop;
                ss.ignore();
            }
            else if (ss.peek() == '&') {
                type = type_conjunction;
                ss.ignore();
            }
            ss >> name;
            network[name].type = type;
            ss.ignore(4);
            while (getline(ss, connection_name, ',')) {
                boost::trim(connection_name);
                network[name].outputs.push_back(connection_name);
                network[connection_name].inputs.push_back(name);
            }
        }
    }

    unordered_map<string, bool> output_values;

    unordered_map<string, pair<int, int>> push_button() {
        unordered_map<string, pair<int, int>> out_pulses;
        out_pulses["button"].first = 1;

        // What "value" does "current" remember from "input"?
        //---------- current -------------- input - value --------------
        unordered_map<string, unordered_map<string, bool>> input_values;
        for (auto& it : network) {
            const string& name = it.first;
            Module& m = it.second;
            for (auto& input : m.inputs) {
                input_values[name][input] = output_values[input];
            }
        }

        queue<string> to_visit;
        to_visit.push("broadcaster");
        unordered_map<string, vector<pair<string, bool>>> pulses {{string("broadcaster"), {{string("button"), false}}}}; // for broadcaster, there is 1 low pulse from button
        while (!to_visit.empty()) {
            string& name = to_visit.front();
            Module& m = network[name];
            bool val;

            auto push_outputs = [&]() {
                for (auto& output : m.outputs) {
                    pulses[output].push_back({name, val});
                    if (pulses[output].size() == 1) {
                        to_visit.push(output);
                    }
                }
                if (val)
                    out_pulses[name].second += int(m.outputs.size());
                else
                    out_pulses[name].first += int(m.outputs.size());
            };

            switch (m.type)
            {
            case type_broadcaster:
                val = false;
                push_outputs();
                break;
            case type_fligflop:
                val = output_values[name];
                for (auto& pulse : pulses[name]) {
                    if (!pulse.second) {
                        val = !val;
                        push_outputs();
                    }
                }
                output_values[name] = val;
                break;
            case type_conjunction:
                for (auto& pulse : pulses[name]) {
                    input_values[name][pulse.first] = pulse.second;
                    val = false;
                    for (auto& input : m.inputs) {
                        if (!input_values[name][input]) {
                            val = true;
                            break;
                        }
                    }
                    push_outputs();
                }
                output_values[name] = val;
                break;
            default:
                break;
            }

            pulses[name].clear();
            to_visit.pop();
        }

        return out_pulses;
    }

    pair<string, string> solve(const string& filename) override {
        get_input(filename);

        uint64_t part1 = 0;
        int lo_sum = 0;
        int hi_sum = 0;
        output_values.clear();
        for (int i = 0; i < 1000; i++) {
            auto out_pulses = push_button();
            for (auto& pulse : out_pulses) {
                lo_sum += pulse.second.first;
                hi_sum += pulse.second.second;
            }
        }
        part1 = lo_sum * hi_sum;

        uint64_t part2 = 0;
        int count = 0;
        output_values.clear();
        unordered_map<string, int> cycles;
        assert(network["rx"].inputs.size() == 1);
        string rx_input = network["rx"].inputs[0];
        unordered_set<string> remaining_cycles (network[rx_input].inputs.begin(), network[rx_input].inputs.end());
        while (count++, !remaining_cycles.empty()) {
            auto out_pulses = push_button();
            decltype(remaining_cycles) to_remove;
            for (auto& str : remaining_cycles) {
                if (out_pulses.contains(str) && out_pulses[str].second) {
                    cycles[str] = count;
                    to_remove.insert(str);
                }
            }

            for (auto& item : to_remove)
                remaining_cycles.erase(item);
        }

        part2 = 1;
        for (auto& it : cycles) {
            part2 = lcm(part2, it.second);
        }
        
        return {to_string(part1), to_string(part2)};
    }
};
