#include <iostream>
#include <fstream>
#include <cassert>
#include <algorithm>
#include <vector>
#include <array>
#include <string>
#include <regex>

using namespace std;

int part1 () {
    ifstream file("../../input.txt", ios::in);

    int count = 0;

    string str;
    while (getline(file, str)) {
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
        count += a * 10 + b;
    }

    return count;
}

int part2() {
    ifstream file("../../input.txt", ios::in);

    int count = 0;

    string orig;
    while (getline(file, orig)) {
        struct Item {
            string s;
            string n;

            Item(string _s, string _n) : s(_s), n(_n) {}
        };
        vector<Item> digits = {
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
        count += a * 10 + b;
    }

    return count;
}

int main() {
    cout << part1() << endl;
    cout << part2() << endl;

    return 0;
}