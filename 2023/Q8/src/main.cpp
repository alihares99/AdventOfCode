#include <iostream>
#include <fstream>
#include <cassert>
#include <algorithm>
#include <numeric>
#include <vector>
#include <array>
#include <string>
#include <regex>
#include <sstream>
#include <optional>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>

using namespace std;

auto get_input() {
    ifstream file("../../input.txt");
    string instructions;
    getline(file, instructions);

    unordered_map<string, pair<string, string>> mapping;
    string str;
    getline(file, str);
    while (getline(file, str)) {
        istringstream ss(str);
        string s1, s2, s3, tmp;
        ss >> s1;
        ss.ignore(4);
        ss >> s2;
        s2.pop_back();
        ss >> s3;
        s3.pop_back();
        mapping[s1] = make_pair(s2, s3);
    }

    int n_instruction = instructions.size();
    unordered_map<string, string> reduced;
    for (auto item : mapping) {
        string in = item.first;
        string out = in;
        for (char c : instructions) {
            if (c == 'L') 
                out = mapping[out].first;
            else
                out = mapping[out].second;
        }
        reduced[in] = out;
    }

    return make_pair(n_instruction, reduced);
}

struct Sequence {
    uint64_t cycle = -1;
    uint64_t pos_z = -1;

    void fill(const unordered_map<string, string>& mapping, string cur) {
        unordered_map<string, int> seen;
        int count = 0;
        while (!seen.contains(cur)) {
            seen[cur] = count;
            if (cur[2] == 'Z') {
                if (pos_z == -1)
                    pos_z = count;
                else
                    assert (false);
            }
            count++;
            cur = mapping.at(cur);
        }
        cycle = count - seen[cur];
    }

};

uint64_t solve() {
    auto [jump, mapping] = get_input();
    uint64_t res = 0;
    string cur = "AAA";
    while (cur != "ZZZ") {
        res += jump;
        cur = mapping[cur];
    }
    return res;
}

uint64_t solve2() {
    auto [jump, mapping] = get_input();
    vector<Sequence> seqs;
    for (const auto& item : mapping) {
        if (item.first[2] == 'A') {
            seqs.emplace_back();
            seqs.back().fill(mapping, item.first);
        }
    }

    uint64_t lcm_so_far = 1;
    uint64_t res = seqs[0].pos_z;
    for (auto& seq : seqs) {
        while (res % seq.cycle != seq.pos_z % seq.cycle) {
            res += lcm_so_far;
        }
        lcm_so_far = lcm(lcm_so_far, seq.cycle);
    }

    return res * jump;
}

int main() {
    cout << solve2() << endl;
    return 0;
}