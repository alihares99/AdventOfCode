#include <iostream>
#include <fstream>
#include <cassert>
#include <algorithm>
#include <vector>
#include <array>
#include <string>
#include <regex>
#include <sstream>

using namespace std;

pair<int, uint64_t> solve () {
    ifstream file("../../test.txt", ios::in);

    int count = 0;
    uint64_t power = 0;
    const int max_red = 12;
    const int max_green = 13;
    const int max_blue = 14;

    string str;
    while (getline(file, str)) {
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

    return {count, power};
}

int main() {
    auto res = solve();
    cout << res.first << " " << res.second << endl;

    return 0;
}