#include <iostream>
#include <fstream>
#include <cassert>
#include <algorithm>
#include <vector>
#include <array>
#include <string>
#include <sstream>
#include <set>
#include <cstdio>
#include <optional>
#include <unordered_map>
#include <unordered_set>
#include <chrono>

using namespace std;

uint64_t part1(vector<int> decode) {
    int n_empty = std::count_if(decode.begin(), decode.end(), [](auto item) {return item == -1;});

    auto front = decode.begin();
    auto back = prev(decode.end());
    while (n_empty) {
        if (*back == -1) {
            n_empty--;
            --back;
        }
        else {
            while (*front != -1) {
                ++front;
            }

            *front = *back;
            *back = -1;
            --back;
            n_empty--;
        }
    }

    uint64_t res = 0;
    uint64_t i = 0;
    while(decode[i] != -1) {
        res += i * uint64_t(decode[i]);
        i++;
    }
    
    return res;
}

uint64_t part2(vector<int> decode) {
    int j = decode.size() - 1;
    int first_empty = 0;
    while (j > first_empty) {
        if (decode[j] == -1) {
            j--;
            continue;
        }

        int size_2 = 1;
        while (j - size_2 >= 0 && decode[j - size_2] == decode[j])
            size_2++;

        for (int i = first_empty; i < j; i++) {
            if (decode[i] != -1) {
                if (i == first_empty)
                    first_empty++;
                continue;
            }

            int size_1 = 1;
            while (i + size_1 < j && decode[i + size_1] == decode[i])
                size_1++;

            if (size_1 >= size_2) {
                for (int k = 0; k < size_2; k++) {
                    decode[i + k] = decode[j];
                }
                for (int k = 0; k < size_2; k++) {
                    decode[j - k] = -1;
                }
                break;
            }
        }
        j -= size_2;
    }

    uint64_t res = 0;
    for(uint64_t i = 0; i < decode.size(); i++) {
        if (decode[i] != -1)
            res += i * uint64_t(decode[i]);
    }

    return res;
}

pair<uint64_t, uint64_t> solve() {
    auto get_line = [](){
        ifstream file("../../input.txt");
        string str;
        getline(file, str);
        return str;
    };
    string line = get_line();

    bool is_file_section = true;
    vector<int> decode;
    decode.reserve(1000000);
    int id = 0;
    for (auto c : line) {
        int v = c - '0';
        for (int i = 0; i < v; i++) {
            if (is_file_section)
                decode.push_back(id);
            else {
                decode.push_back(-1);
            }
        }
        if (is_file_section)
            id++;
        is_file_section = !is_file_section;
    }

    auto p1 = part1(decode);
    auto p2 = part2(move(decode));
    return {p1, p2};
}

int main () {
    auto [p1, p2] = solve();
    cout << p1 << " " << p2 << endl;
}
