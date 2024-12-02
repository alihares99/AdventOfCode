#include <iostream>
#include <fstream>
#include <cassert>
#include <algorithm>
#include <vector>
#include <array>
#include <string>
#include <sstream>
#include <set>

using namespace std;

template <class Condition>
bool check(const vector<int>& vec, Condition cond) {
    set<int> skip_candidate;
    for (int i = 0; i < vec.size() - 1; i++) {
        if (!cond(vec[i], vec[i + 1])) {
            bool can_skip_i = (i == 0) || cond(vec[i - 1], vec[i + 1]);
            bool can_skip_i_1 = (i == vec.size() - 2) || cond(vec[i], vec[i + 2]);

            if (!can_skip_i && !can_skip_i_1)
                return false;
            if (skip_candidate.size() == 0) {
                if (can_skip_i) skip_candidate.insert(i);
                if (can_skip_i_1) skip_candidate.insert(i + 1);
            } else if (can_skip_i && skip_candidate.contains(i)) {
                // do nothing
            } else {
                return false;
            }
        }
    }
    
    return true;
}

struct ConditionAscending {
    bool operator() (int l, int r) {
        return (l < r && r <= l + 3);
    }
};

struct ConditionDescending {
    bool operator() (int r, int l) {
        return (l < r && r <= l + 3);
    }
};

bool check_ascending(const vector<int>& vec) {
    return check<ConditionAscending>(vec, ConditionAscending());
}

bool check_descending(const vector<int>& vec) {
    return check<ConditionDescending>(vec, ConditionDescending());
}

int get() {
    ifstream file("../../input.txt", ios::in);

    int count = 0;
    
    string str;
    while(getline(file, str)) {
        istringstream ss(str);
        int val;
        vector<int> vec;
        while (ss >> val) {
            vec.push_back(val);
        }

        if (check_ascending(vec) || check_descending(vec)) {
            count++;
        }
    }

    return count;
}

int first_try();

int main() {
    cout << get();
    return 0;

    // my first try to solve the question:
    cout << first_try();
    return 0;
}