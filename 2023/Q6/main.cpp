#include <iostream>
#include <fstream>
#include <cassert>
#include <algorithm>
#include <numeric>
#include <vector>
#include <array>
#include <string>
#include <regex>
#include <sstream>
#include <optional>
#include <map>
#include <unordered_map>
#include <unordered_set>

using namespace std;

auto get_input() {
    vector<int> times, distances;

    ifstream file("../../input.txt");
    string str;
    getline(file, str);
    {
        istringstream ss(str);
        string tmp;
        ss >> tmp;
        int v;
        while (ss >> v) {
            times.push_back(v);
        }
    }

    getline(file, str);
    {
        istringstream ss(str);
        string tmp;
        ss >> tmp;
        int v;
        while (ss >> v) {
            distances.push_back(v);
        }
    }

    return make_pair(times, distances);
}

uint64_t solve() {
    auto [times, distances] = get_input();
    uint64_t res = 1;
    for (int i = 0; i < times.size(); i++) {
        auto time = times[i];
        auto distance = distances[i];
        int n = 0;
        for (int j = 1; j < time; j++) {
            if (j * (time - j) > distance)
                n++;
        }
        res *= n;
    }

    return res;
}

uint64_t solve2() {
    auto [times, distances] = get_input();
    auto concat_values = [](vector<int> in) {
        uint64_t out = 0;
        for (auto item : in) {
            auto tmp = item;
            while(tmp /= 10) out *= 10;
            out *= 10;
            out += item;
        }
        return out;
    };
    auto time = concat_values(times);
    auto distance = concat_values(distances);
    uint64_t n = 0;
    for (int j = 1; j < time; j++) {
        if (j * (time - j) > distance)
            n++;
    }

    return n;
}

int main() {
    cout << solve() << " " << solve2() << endl;

    return 0;
}