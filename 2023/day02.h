#pragma once
#include "useful.h"

struct Day02 : public SolutionPair {

    pair<string, string> solve(const string& filename) override {
        auto lines = get_input_all_lines(filename);

        int count = 0;
        int power = 0;
        const int max_red = 12;
        const int max_green = 13;
        const int max_blue = 14;

        for (auto& str : lines) {
            istringstream ss(str);
            string game;
            int number;
            ss >> game >> number;
            ss.ignore(2);

            string color;
            int red = 0, green = 0, blue = 0;
            int v;
            while (ss >> v >> color) {
                char delim = 0;
                if (color.back() == ',' || color.back() == ';') {
                    delim = color.back();
                    color.pop_back();
                }
                if (color == "red") {
                    if (v > red) red = v;
                } else if (color == "green") {
                    if (v > green) green = v;
                } else if (color == "blue") {
                    if (v > blue) blue = v;
                }
            }

            bool possible = 
                    blue <= max_blue && 
                    red <= max_red && 
                    green <= max_green;

            if (possible) count += number; 
            power += (red * green * blue);
        }

        return {to_string(count), to_string(power)};
    }
};
