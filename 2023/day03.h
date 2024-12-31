#pragma once
#include "useful.h"

struct Day03 : public SolutionPair {

    static bool is_symbol(char c) {
        return (c < '0' || c > '9') && c != '.';
    }

    static bool is_star(char c) {
        return c == '*';
    }

    template <class Func>
    optional<Point> search_box_for_symbol(const vector<string>& mat, int i, int pos, int len, Func search) {
        const int n_col = int(mat[0].size());
        if (pos > 0 && search(mat[i][pos - 1])) return Point{i, pos - 1};
        if (pos + len < n_col && search(mat[i][pos + len])) return Point{i, pos + len};
        if (i > 0) {
            for (int j = max(0, pos - 1); j < min(pos + len + 1, n_col); j++) {
                if (search(mat[i - 1][j]))
                    return Point{i - 1, j};
            }
        }
        if (i < mat.size() - 1) {
            for (int j = max(0, pos - 1); j < min(pos + len + 1, n_col); j++) {
                if (search(mat[i + 1][j]))
                    return Point{i + 1, j};
            }
        }

        return nullopt;
    }

    
    pair<string, string> solve(const string& filename) override {
        auto mat = get_input_all_lines(filename);

        int count = 0;
        uint64_t count2 = 0;

        unordered_map<uint64_t, vector<int>> gears;
        for (int i = 0; i < mat.size(); i++) {
            auto& str = mat[i];
            regex reg("\\.*(\\d+)");
            sregex_iterator it_begin(str.begin(), str.end(), reg), it_end;
            for (auto it = it_begin; it != it_end; it++) {
                smatch found = *it;
                if (found.size() >= 2) {
                    int pos = int(found.position(1));
                    int len = int(found[1].length());
                    bool has_symbol = search_box_for_symbol(mat, i, pos, len, &is_symbol).has_value();
                    auto star_position = search_box_for_symbol(mat, i, pos, len, &is_star);
                    if (has_symbol || star_position.has_value()) {
                        istringstream ss(found.str(1));
                        int val; ss >> val;

                        if (has_symbol)
                            count += val;

                        if (star_position.has_value()) {
                            uint64_t pos_64 = uint64_t(star_position->i) + (uint64_t(star_position->j) << 32);
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

        return {to_string(count), to_string(count2)};
    }
};
