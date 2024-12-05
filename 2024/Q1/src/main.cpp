#include <iostream>
#include <fstream>
#include <cassert>
#include <algorithm>
#include <vector>
#include <array>

using namespace std;

int main() {
    ifstream file("../../input.txt", ios::in);

    vector<int> l, r;
    int v1, v2;
    while (file >> v1 >> v2) {
        l.push_back(v1);
        r.push_back(v2);
    }

    sort(l.begin(), l.end());
    sort(r.begin(), r.end());

    int diff = 0;
    for (int i = 0; i < l.size(); i++) {
        diff += abs(l[i] - r[i]);
    }

    // part 1:
    cout << diff << endl;

    uint64_t count_2 = 0;
    for (int item : l) {
        auto [it1, it2] = equal_range(r.begin(), r.end(), item);
        count_2 += item * (it2 - it1);
    }

    // part 2:
    cout << count_2 << endl;

    return 0;
}