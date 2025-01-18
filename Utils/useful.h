#pragma once

#include <iostream>
#include <filesystem>
#include <fstream>
#include <memory>
#include <cassert>
#include <algorithm>
#include <numeric>
#include <vector>
#include <array>
#include <string>
#include <string_view>
#include <sstream>
#include <set>
#include <map>
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

#include <boost/functional/hash.hpp>
#include <boost/algorithm/string.hpp>

#include "binary_tree.h"

using namespace std;

enum Direction : int {dir_up = 0, dir_right, dir_down, dir_left};

enum DirectionFlag : int {dirflag_up = 1, dirflag_right = 2, dirflag_down = 4, dirflag_left = 8};

Direction mirror(Direction dir) {
    return Direction((dir + 2) % 4);
}

DirectionFlag mirror(DirectionFlag dir) {
    return DirectionFlag(0xF & (dir >> 2 | dir << 2));
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

    Point& move(Direction dir, int n = 1) {
        switch (dir)
        {
        case dir_up: i -= n; break;
        case dir_down: i += n; break;
        case dir_right: j += n; break;
        case dir_left: j -= n; break;
        }
        return *this;
    }

    Point& move(DirectionFlag dir, int n = 1) {
        switch (dir)
        {
        case dirflag_up: i -= n; break;
        case dirflag_down: i += n; break;
        case dirflag_right: j += n; break;
        case dirflag_left: j -= n; break;
        default:
            assert(false);
        }
        return *this;
    }

    bool operator==(const Point& p) const = default;
};

struct DirectedPoint : public Point {
    Direction dir;

    DirectedPoint& move(int n = 1) {
        Point::move(dir, n);
        return *this;
    }
    DirectedPoint& turn_right() {
        dir = rotate_clockwise(dir);
        return *this;
    }
    DirectedPoint& turn_left() {
        dir = rotate_counter_clockwise(dir);
        return *this;
    }
    DirectedPoint& turn_around() {
        dir = mirror(dir);
        return *this;
    }
};

template <>
struct hash<Point> {
    size_t operator()(const Point& p) const noexcept {
        size_t seed = 0;
        boost::hash_combine(seed, p.i);
        boost::hash_combine(seed, p.j);
        return seed;
    }
};

template <>
struct hash<DirectedPoint> {
    size_t operator()(const DirectedPoint& p) const noexcept {
        size_t seed = 0;
        boost::hash_combine(seed, p.i);
        boost::hash_combine(seed, p.j);
        boost::hash_combine(seed, p.dir);
        return seed;
    }
};

template <class T1, class T2>
struct hash<pair<T1, T2>> {
    size_t operator()(const pair<T1, T2>& p) const noexcept {
        size_t seed = 0;
        boost::hash_combine(seed, p.first);
        boost::hash_combine(seed, p.second);
        return seed;
    }
};

vector<string> get_input_all_lines(string file_name) {
    ifstream file(file_name);
    vector<string> lines;
    string str;
    while(getline(file, str)) {
        lines.push_back(str);
    }
    return lines;
}

/**
 * joins strings optionally separated by @param(separator)
 */
string join_strings(const vector<string>& strs, optional<char> seprator = nullopt) {
    size_t size = 0;
    for (auto& str : strs) {
        size += str.size();
    }
    if (size == 0)
        return "";
    if (seprator.has_value())
        size += strs.size() - 1;
    string s;
    s.reserve(size);

    bool write = false;
    for (auto& str : strs) {
        if (write && seprator.has_value())
            s.push_back(*seprator);
        write = true;
        s.append(str);
    }
    return s;
}

/**
 * repeats @param(v), @param(n) times - optionally separated by @param(separator)
 * @returns a container same a @param(v) with the desired output
 */
template<class T>
auto repeat(const auto& v, int n, optional<T> separator = nullopt) {
    remove_cvref_t<decltype(v)> res;
    res.reserve(v.size() * n + (separator.has_value() ? n - 1 : 0));
    bool first = true;
    for (int i = 0; i < n; i++) {
        if (!first && separator.has_value())
            res.push_back(*separator);
        first = false;
        res.insert(res.end(), v.begin(), v.end());
    }
    return res;
}

/**
 * creates a std::array with initialization to @param(v)
 */
template <class T, size_t N>
array<T, N> make_array(const T &v) {
    array<T,N> ret;
    ret.fill(v);
    return ret;
}

int get_n_digits(auto n) {
    int n_digits = 1;
    while (n /= 10) {
        n_digits++;
    }
    return n_digits;
}

uint64_t lcm_with_remainder(const auto& divisors_begin, const auto& divisors_end,
                            const auto& remainders_begin) {
    auto it_d = divisors_begin;
    auto it_r = remainders_begin;
    uint64_t v = *it_r;
    uint64_t lcm_so_far = *it_d;
    ++it_r; ++it_d;
    for (; it_d != divisors_end; ++it_d, ++it_r) {
        while (v % *it_d != *it_r) {
            v += lcm_so_far;
        }
        lcm_so_far = lcm(lcm_so_far, *it_d);
    }
    return v == 0 ? lcm_so_far : v;
}

/**
 * returns the lowest number that has a @param(remainders) when devided by the @param(divisors)
 */
uint64_t lcm_with_remainder(const auto& divisors, const auto& remainders) {
    return lcm_with_remainder(divisors.begin(), divisors.end(), remainders.begin());
}

struct Solution {
    virtual string part1(const string& filename) = 0;
    virtual string part2(const string& filename) = 0;
    virtual ~Solution() {}
};

struct SolutionPair {
    virtual pair<string, string> solve(const string& filename) = 0;
    virtual ~SolutionPair() {}
};