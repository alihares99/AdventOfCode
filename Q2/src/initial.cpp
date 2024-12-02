#include <iostream>
#include <fstream>
#include <cassert>
#include <algorithm>
#include <vector>
#include <array>
#include <string>
#include <sstream>

// initial submittion to AdventOfCode

using namespace std;

static bool check_ascending(vector<int> vec, int& skip) {
    int left, right;
    if (skip == 0) {
        left = 1;
        right = 2;
    } else if (skip == 1) {
        left = 0;
        right = 2;
    } else {
        left = 0;
        right = 1;
    }
    while (right < vec.size()) {
        if (!(vec[left] < vec[right] && vec[right] <= vec[left] + 3)) {
            skip = left;
            return false;
        }
        left++;
        right++;
        if (right == skip) right++;
        if (left == skip) left++;
    }
    return true;
}

static bool check_descending(vector<int> vec, int& skip) {
    int left, right;
    if (skip == 0) {
        left = 1;
        right = 2;
    } else if (skip == 1) {
        left = 0;
        right = 2;
    } else {
        left = 0;
        right = 1;
    }
    while (right < vec.size()) {
        if (!(vec[right] < vec[left] && vec[left] <= vec[right] + 3)) {
            skip = left;
            return false;
        }
        left++;
        right++;
        if (right == skip) right++;
        if (left == skip) left++;
    }
    return true;
}

int first_try() {
    ifstream file("../../input.txt", ios::in);

    int count = 0;
    vector<int> l;
    
    int n = 0;
    array<char, 100> line;
    while (file.getline(&line[0], 100)) {
        string str(&line[0]);
        istringstream ss (str);
        int val;
        vector<int> vec;
        while (ss >> val) {
            vec.push_back(val);
        }

        int skip = -1;
        bool safe1 = check_ascending(vec, skip);
        if (!safe1) {
            int tmp = skip;
            int tmp2 = skip + 1;
            safe1 = check_ascending(vec, tmp) || check_ascending(vec, tmp2);
        }
        skip = -1;
        bool safe2 = check_descending(vec, skip);
        if (!safe2) {
            int tmp = skip;
            int tmp2 = skip + 1;
            safe2 = check_descending(vec, tmp) || check_descending(vec, tmp2);
        }

        if (safe1 || safe2) {
            count++;
        }
        n++;
    }

    return count;
}