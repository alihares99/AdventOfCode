#pragma once
#include "useful.h"

struct Day13 : public Solution {
    struct Point {
        uint64_t x, y;
    };
    struct Data{
        Point p1, p2, prize;
    };

    auto get_input(const string& filename, bool part2 = false) {
        vector<Data> input;
        ifstream file(filename);
        regex reg("Button A: X\\+(\\d+), Y\\+(\\d+)Button B: X\\+(\\d+), Y\\+(\\d+)Prize: X=(\\d+), Y=(\\d+)");
        string str;
        while(getline(file, str)) {
            string tmp;
            getline(file, tmp);
            str.append(tmp);
            getline(file, tmp);
            str.append(tmp);

            Data data;
            smatch match;
            if (regex_match(str, match, reg)) {
                data.p1.x = stoull(match[1]);
                data.p1.y = stoull(match[2]);
                data.p2.x = stoull(match[3]);
                data.p2.y = stoull(match[4]);
                data.prize.x = stoull(match[5]);
                data.prize.y = stoull(match[6]);
            }
            
            if (part2) {
                data.prize.x += 10000000000000;
                data.prize.y += 10000000000000;
            }

            input.push_back(data);

            getline(file, str);
        }
        return input;
    }

    uint64_t solve(const string& filename, bool part2) {
        auto input = get_input(filename, part2);
        uint64_t res = 0;
        for (auto& data : input) {
            bool possible = true;
            if (data.p1.x * data.p2.y == data.p2.x * data.p1.y) { // slopes are the same
                // simplifying assumption...
                assert(false); 
            }
            else {
                // slopes are different
                int64_t denom = data.p2.x * data.p1.y - data.p1.x * data.p2.y;
                int64_t x_numer = data.p1.x * (data.p2.x * data.prize.y - data.p2.y * data.prize.x);
                int64_t y_numer = data.p1.y * (data.p2.x * data.prize.y - data.p2.y * data.prize.x);

                if (x_numer % denom == 0 && y_numer % denom == 0) {
                    Point collision;
                    collision.x = x_numer / denom;
                    collision.y = y_numer / denom;
                    if (collision.x % data.p1.x == 0 && (data.prize.x - collision.x) % data.p2.x == 0) {
                        int64_t n_a = collision.x / data.p1.x;
                        int64_t n_b = (data.prize.x - collision.x) / data.p2.x;
                        if (n_a >= 0 && n_b >= 0)
                            res += n_a * 3 + n_b;
                    }
                }
            }
        }

        return res;
    }

    string part1(const string& filename) override {
        return to_string(solve(filename, false));
    }

    string part2(const string& filename) override {
        return to_string(solve(filename, true));
    }
};
