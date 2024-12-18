#include <iostream>
#include <fstream>
#include <cassert>
#include <algorithm>
#include <vector>
#include <array>
#include <string>
#include <regex>
#include <sstream>
#include <optional>
#include <unordered_map>

using namespace std;

bool is_symbol(char c) {
    return (c < '0' || c > '9') && c != '.';
}

bool is_star(char c) {
    return c == '*';
}

template <class Func>
optional<pair<int, int>> search_box_for_symbol(const vector<string>& mat, int i, int pos, int len, Func search) {
    const int n_col = mat[0].size();
    if (pos > 0 && search(mat[i][pos - 1])) return make_pair(i, pos - 1);
    if (pos + len < n_col && search(mat[i][pos + len])) return make_pair(i, pos + len);
    if (i > 0) {
        for (int j = max(0, pos - 1); j < min(pos + len + 1, n_col); j++) {
            if (search(mat[i - 1][j]))
                return make_pair(i - 1, j);
        }
    }
    if (i < mat.size() - 1) {
        for (int j = max(0, pos - 1); j < min(pos + len + 1, n_col); j++) {
            if (search(mat[i + 1][j]))
                return make_pair(i + 1, j);
        }
    }

    return nullopt;
}

pair<int, uint64_t> solve_reg() {
    ifstream file("../../input.txt", ios::in);

    int count = 0;
    uint64_t count2 = 0;

    vector<string> mat;
    mat.reserve(140);
    {
        string str;
        while (getline(file, str)) {
            mat.push_back(str);
        }
    }

    unordered_map<uint64_t, vector<int>> gears;
    for (int i = 0; i < mat.size(); i++) {
        auto& str = mat[i];
        regex reg("\\.*(\\d+)");
        sregex_iterator it_begin(str.begin(), str.end(), reg), it_end;
        for (auto it = it_begin; it != it_end; it++) {
            smatch found = *it;
            if (found.size() >= 2) {
                int pos = found.position(1);
                int len = found[1].length();
                bool has_symbol = search_box_for_symbol(mat, i, pos, len, &is_symbol).has_value();
                auto star_position = search_box_for_symbol(mat, i, pos, len, &is_star);
                if (has_symbol || star_position.has_value()) {
                    istringstream ss(found.str(1));
                    int val; ss >> val;

                    if (has_symbol)
                        count += val;

                    if (star_position.has_value()) {
                        uint64_t pos_64 = uint64_t((*star_position).first) + (uint64_t((*star_position).second) << 32);
                        gears[pos_64].push_back(val);
                    }
                }
            }
        }
    }

    for (const auto& vec : gears) {
        if (vec.second.size() == 2) {
            count2 += (vec.second[0] * vec.second[1]);
        }
    }

    return {count, count2};
}

int main() {
    auto [part1, part2] = solve_reg();
    cout << part1 << " " << part2 << endl;

    return 0;
}