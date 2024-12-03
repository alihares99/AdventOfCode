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

using namespace std;

#define PART_2 true

int main() {
    {
        string s = "ali ali 100";
        istringstream ss (s);
        int a;
        string p1, p2;
        if (ss >> p1 >> p2 >> a) {
            cout << p1;
        }
        else {
            cout << "not match";
        }
        cout << endl;
    }

    ifstream file("../../input.txt", ios::in);

    uint64_t count = 0;

    stringstream ss;
    ss << file.rdbuf();
    string str = ss.str();

    auto found = str.find("mul(");
#if(PART_2)
    auto found_do = str.find("do()");
    auto found_dont = str.find("don't()");
    auto prev_do = found_do;
    auto prev_dont = found_dont;
#endif

    while(found != string::npos) {
#if(PART_2)
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
#endif
            int a, b;
            char c;
            if (sscanf(str.c_str() + found + 4, "%d,%d%c", &a, &b, &c) == 3 && c == ')') {
                count += a * b;
            }
#if(PART_2)
        }
#endif
        found = str.find("mul(", found + 4);
    }

    cout << count << endl;

    return 0;
}