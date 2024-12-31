#pragma once
#include "useful.h"

struct Day01 : public SolutionPair {
    struct Item {
        string s;
        string n;

        Item(string _s, string _n) : s(_s), n(_n) {}
    };

    vector<string> lines;

    void get_input(const string& filename) {
        if (!lines.empty()) 
            return;
        
        lines = get_input_all_lines(filename);
    }

    int calc(const string& str) {
        int a = -1;
        int b = -1;
        for (char c : str) {
            if (c >= '1' && c <= '9') {
                if (a == -1)
                    a = c - '0';
                else
                    b = c - '0';
            }
        }
        if (b == -1)
            b = a;
        
        assert(a != -1);
        return a * 10 + b;
    }

    pair<string, string> solve(const string& filename) override {
        get_input(filename);

        const vector<Item> digits = {
            {string("one"), string("1")},
            {string("two"), string("2")},
            {string("three"), string("3")},
            {string("four"), string("4")},
            {string("five"), string("5")},
            {string("six"), string("6")},
            {string("seven"), string("7")},
            {string("eight"), string("8")},
            {string("nine"), string("9")},
        };

        int part1 = 0;
        int part2 = 0;
        for (auto& orig : lines){
            string str;
            array<int, 9> track = {0};
            for (char c : orig) {
                if (c >= '0' && c <= '9') {
                    str.push_back(c);
                    fill(track.begin(), track.end(), 0);
                } else {
                    for (int i = 0; i < 9; i++) {
                        if (c == digits[i].s[track[i]])
                            track[i]++;
                        else if (c == digits[i].s[0])
                            track[i] = 1;
                        else
                            track[i] = 0;
                        
                        if (track[i] == digits[i].s.size()) {
                            track[i] = 0;
                            str.append(digits[i].n);
                        }
                    }
                }
            }
                
            part2 += calc(str);
            part1 += calc(orig);
        }

        return {to_string(part1), to_string(part1)};
    }

};
