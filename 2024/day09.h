#pragma once
#include "useful.h"

struct Day09 : public Solution {
    vector<int> get_input(const string& filename) {
        string line = get_input_all_lines(filename)[0];

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

        return decode;
    }

    string part1(const string& filename) override {
        auto decode = get_input(filename);
        size_t n_empty = std::count_if(decode.begin(), decode.end(), [](auto item) {return item == -1;});

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
        
        return to_string(res);
    }

    string part2(const string& filename) override {
        auto decode = get_input(filename);
        int j = int(decode.size()) - 1;
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
        return to_string(res);
    }
};
