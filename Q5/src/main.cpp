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

using namespace std;

pair<int, int> solve() {
    ifstream file("../../input.txt", ios::in);

    int count = 0;
    int count_2 = 0;
    unordered_map<int, unordered_set<int>> m;

    string str;
    while(getline(file, str)) {
        if (str.empty())
            break;

        istringstream ss (str);
        int a, b;
        char c;
        if (ss >> a >> c >> b && c == '|') {
            m[a].insert(b);
        }
    }

    while(getline(file, str)) {
        vector<int> test;
        test.reserve(20);
        istringstream ss(str);
        int a;
        while (ss >> a) {
            test.push_back(a);
            ss.ignore(1);
        }

        bool safe = true;
        bool fail = true;
        while (fail) {
            fail = false;
            for (int i = 0; i < test.size() - 1 && !fail; i++) {
                for (int j = i + 1; j < test.size() && !fail; j++) {
                    if (m.contains(test[i]) && !m[test[i]].contains(test[j])) {
                        swap(test[i], test[j]);
                        safe = false;
                        fail = true;
                    }
                }
            }
        }

        if (safe) // part 1
            count += test[test.size() / 2];
        else // part 2
            count_2 += test[test.size() / 2];
    }

    return {count, count_2};
}

int main () {
    auto res = solve();
    cout << "part 1: " << res.first << endl;
    cout << "part 2: " << res.second << endl;
}
