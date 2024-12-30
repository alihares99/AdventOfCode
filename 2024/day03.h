#pragma once
#include "useful.h"

struct Day03 : public Solution {
    string str;

    void get_input(const string& filename) {
        if (!str.empty())
            return;
        
        ifstream file(filename);
        stringstream ss;
        ss << file.rdbuf();
        str = ss.str();
    }

    string part1(const string& filename) override {
        get_input(filename);

        uint64_t count = 0;
        auto found = str.find("mul(");
        while(found != string::npos) {
            int a, b;
            char c;
            if (sscanf_s(str.c_str() + found + 4, "%d,%d%c", &a, &b, &c, 1) == 3 && c == ')') {
                count += a * b;
            }
            found = str.find("mul(", found + 4);
        }
        return to_string(count);
    }

    string part2(const string& filename) override {
        get_input(filename);

        uint64_t count = 0;
        auto found = str.find("mul(");
        auto found_do = str.find("do()");
        auto found_dont = str.find("don't()");
        auto prev_do = found_do;
        auto prev_dont = found_dont;

        while(found != string::npos) {
            while (found_do < found) {
                prev_do = found_do;
                found_do = str.find("do()", found_do + 4);
            }
            while (found_dont < found) {
                prev_dont = found_dont;
                found_dont = str.find("don't()", found_dont + 7);
            }

            bool enable = true;
            if (prev_dont < found) {
                if (prev_do < found) 
                    enable = prev_do > prev_dont;
                else
                    enable = false;
            }
            
            if (enable) {
                int a, b;
                char c;
                if (sscanf_s(str.c_str() + found + 4, "%d,%d%c", &a, &b, &c, 1) == 3 && c == ')') {
                    count += a * b;
                }
            }
            found = str.find("mul(", found + 4);
        }
        return to_string(count);
    }
};
