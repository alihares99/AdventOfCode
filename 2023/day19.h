#pragma once
#include "useful.h"

struct Day19 : public SolutionPair {

    using Part = array<int, 4>;

    struct Condition {
        int index;
        bool is_greater;
        int val;
    };

    struct Rule{
        optional<Condition> cond;
        string res;
    };

    struct Range {
        Part lo = make_part(1);
        Part hi = make_part(4001);

        bool has_range() {
            for (int i = 0; i < 4; i++) {
                if (hi[i] <= lo[i])
                    return false;
            }
            return true;
        }

        uint64_t get_product() {
            uint64_t res = 1;
            for (int i = 0; i < 4; i++) {
                res *= hi[i] - lo[i];
            }
            return res;
        }
    };

    unordered_map<string, vector<Rule>> workflows;
    vector<Part> parts;

    static Part make_part(const int& v) { 
        return make_array<int, 4>(v); 
    }
    
    static int get_index(char c) {
        switch(c) {
            case 'x': return 0;
            case 'm': return 1;
            case 'a': return 2;
            case 's': return 3;
        }
        return -1;
    }

    void get_input(const string& filename) {
        auto lines = get_input_all_lines(filename);
        auto it = lines.begin();
        for (; it != lines.end(); ++it) {
            if (it->empty())
                break;
            auto& str = *it;
            auto sep = str.find('{');
            string name = str.substr(0, sep);
            regex reg("(?:(\\w)([<>])(\\d+):)?([^,]+)(?:,|\\})");
            sregex_iterator begin = sregex_iterator(str.begin() + sep + 1, str.end(), reg), end;
            for (auto it = begin; it != end; ++it) {
                smatch match = *it;
                Rule rule;
                if (match[1].matched) {
                    Condition cond;
                    cond.index = get_index(match[1].str()[0]);
                    cond.is_greater = match[2].str()[0] == '>';
                    cond.val = stoi(match[3].str());
                    rule.cond = cond;
                }
                rule.res = match[4].str();
                workflows[name].push_back(rule);
            }
        }
        ++it;
        for (; it != lines.end(); ++it) {
            istringstream ss(*it);
            Part p;
            for (int i = 0; i < 4; i++) {
                ss.ignore(3);
                ss >> p[i];
            }
            parts.push_back(p);
        }
    }

    bool is_accepted(Part& p) {
        string cur = "in";
        while (cur != "A" && cur != "R") {
            for (auto& rule : workflows[cur]) {
                if (rule.cond.has_value()) {
                    auto& cond = *rule.cond;
                    if ((cond.is_greater && p[cond.index] > cond.val) || 
                        (!cond.is_greater && p[cond.index] < cond.val)){
                        cur = rule.res;
                        break;
                    }
                }
                else {
                    cur = rule.res;
                    break;
                }
            }
        }
        return cur == "A";
    }


    uint64_t count_all_accepted(Range range, string cur) {
        if (!range.has_range() || cur == "R")
            return 0;
        if (cur == "A") 
            return range.get_product();
        
        uint64_t res = 0;
        for (Rule& rule : workflows[cur]) {
            if (rule.cond.has_value()) {
                Range another = range;
                Condition& cond = *rule.cond;
                if (cond.is_greater) {
                    range.hi[cond.index] = min(range.hi[cond.index], cond.val + 1);
                    another.lo[cond.index] = max(another.lo[cond.index], cond.val + 1);
                }
                else {
                    range.lo[cond.index] = max(range.lo[cond.index], cond.val);
                    another.hi[cond.index] = min(another.hi[cond.index], cond.val);
                }
                res += count_all_accepted(another, rule.res);
            }
            else {
                res += count_all_accepted(range, rule.res);
            }
        }
        return res;
    }

    pair<string, string> solve(const string& filename) override {
        get_input(filename);

        uint64_t part1 = 0;
        for (auto& p : parts) {
            if (is_accepted(p))
                part1 += accumulate(p.begin(), p.end(), 0);
        }

        uint64_t part2 = count_all_accepted(Range{}, "in");
        
        return {to_string(part1), to_string(part2)};
    }
};
