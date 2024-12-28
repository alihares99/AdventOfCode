#include <iostream>
#include <fstream>
#include <cassert>
#include <algorithm>
#include <vector>
#include <array>
#include <string>
#include <string_view>
#include <sstream>
#include <set>
#include <cstdio>
#include <optional>
#include <unordered_map>
#include <unordered_set>
#include <forward_list>
#include <chrono>
#include <regex>
#include <functional>
#include <stack>
#include <queue>

using namespace std;

enum Direction : int {dir_up = 0, dir_right, dir_down, dir_left};

Direction mirror(Direction dir) {
    return Direction((dir + 2) % 4);
}

Direction rotate_clockwise(Direction dir) {
    return Direction((dir + 1) % 4);
}

Direction rotate_counter_clockwise(Direction dir) {
    return Direction((dir + 3) % 4);
}

struct Point {
    int i, j;

    Point() : i(0), j(0) {}
    Point(int i, int j) : i(i), j(j) {}

    void move(Direction dir) {
        switch (dir)
        {
        case dir_up: i--; break;
        case dir_down: i++; break;
        case dir_right: j++; break;
        case dir_left: j--; break;
        }
    }

    bool operator==(const Point& p) const = default;
};

template <>
struct hash<Point> {
    size_t operator()(const Point& p) const noexcept {
        std::size_t h1 = std::hash<int>{}(p.i);
        std::size_t h2 = std::hash<int>{}(p.j);
        return h1 ^ (h2 << 1); // or use boost::hash_combine
    }
};

template <class T1, class T2>
struct hash<pair<T1, T2>> {
    size_t operator()(const pair<T1, T2>& p) const noexcept {
        std::size_t h1 = std::hash<T1>{}(p.first);
        std::size_t h2 = std::hash<T2>{}(p.second);
        return h1 ^ (h2 << 1); // or use boost::hash_combine
    }
};

struct DirectedPoint : public Point {
    Direction dir;

    void move() {
        Point::move(dir);
    }
    void turn_right() {
        dir = rotate_clockwise(dir);
    }
    void turn_left() {
        dir = rotate_counter_clockwise(dir);
    }
};

vector<string> get_input_all_lines(string file_name) {
    ifstream file("../../" + file_name);
    vector<string> lines;
    string str;
    while(getline(file, str)) {
        lines.push_back(str);
    }
    return lines;
}

string join_strings(const vector<string>& strs, char seprator) {
    size_t size = 0;
    for (auto& str : strs) {
        size += str.size() + 1;
    }
    if (size == 0)
        return "";
    string s;
    s.reserve(size - 1);

    bool write = false;
    for (auto& str : strs) {
        if (write)
            s.push_back(seprator);
        write = true;
        s.append(str);
    }
    return s;
}