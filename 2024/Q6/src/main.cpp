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

enum class Direction : int 
{
    up = 0, right = 1, down = 2, left = 3
};

union PathComplex {
    struct {
        uint16_t i;
        uint16_t j;
        uint8_t direction;
    };
    uint64_t val;
};

uint64_t get_pos_64(int i, int j) {
    return uint64_t(i) + (uint64_t(j) << 32);
};

struct Matrix {
    struct iterator;
    Matrix(vector<string>&& vec) : 
        mat (vec), 
        n_row(mat.size()), 
        n_col(mat[0].size()),
        start_i(),
        start_j(),
        start_dir()
    {    
        for (int i = 0; i < n_row; i++) {
            for (int j = 0; j < n_col; j++) {
                if (mat[i][j] == '^') {
                    mat[i][j] = '.';
                    const_cast<int&> (start_i) = i;
                    const_cast<int&> (start_j) = j;
                    const_cast<Direction&> (start_dir) = Direction::up;
                }
            }
        }
    }

    iterator begin() {
        return iterator(*this, start_i, start_j, start_dir);
    }

    vector<string> mat;
    const int n_col;
    const int n_row;
    const int start_i;
    const int start_j;
    const Direction start_dir;

    struct iterator {
        iterator(Matrix& parent, int i, int j, Direction direction) 
            : parent(parent), i(i), j(j), direction(direction)
        {}

        iterator(const iterator& other) 
            : parent(other.parent), i(other.i), j(other.j), direction(other.direction) 
        {}

        iterator& operator=(const iterator& other) {
            i = other.i;
            j = other.j;
            direction = other.direction;
            return *this;
        }

        char& operator*() {
            return parent.mat[i][j];
        }

        void move(int n) {
            switch (direction)
            {
            case Direction::up: i -= n; break;
            case Direction::down: i += n; break;
            case Direction::left: j -= n; break;
            case Direction::right: j += n; break;
            }
        }

        void operator++() {
            while(true) {
                move(1);
                if (is_valid() && is_obstacle()) {
                    move(-1);
                    turn();
                }
                else {
                    break;
                }
            }
        }

        void turn() {
            direction = Direction(((int) direction + 1) % 4);
        }

        bool obstacle_ahead() {
            bool ret = false;
            move(1);
            if (is_valid() && is_obstacle())
                ret = true;
            move(-1);
            return ret;
        }

        uint64_t get_path_complex() {
            PathComplex c;
            c.i = i;
            c.j = j;
            c.direction = (uint8_t) direction;
            return c.val;
        }

        uint64_t get_pos_64() {
            return ::get_pos_64(i, j);
        }

        bool is_valid() {
            return 0 <= i && i < parent.n_row && 0 <= j && j < parent.n_col;
        }

        bool is_obstacle() {
            return parent.mat[i][j] == '#';
        }

        Matrix& parent;
        int i;
        int j;
        Direction direction;
    };
};

bool check_cyclic(Matrix& mat) {
    unordered_set<uint64_t> path;
    for (auto p = mat.begin(); p.is_valid(); ++p) {
        if (p.obstacle_ahead()) {
            auto path_comp = p.get_path_complex();
            if (path.contains(path_comp)) 
                return true;
            else
                path.insert(path_comp);
        }
    }
    return false;
}

pair<int, int> solve() {
    ifstream file("../../input.txt", ios::in);

    int count = 0;
    auto get_lines = [&](){
        vector<string> lines;
        string str;
        while(getline(file, str)) {
            lines.push_back(str);
        }
        return lines;
    };
    Matrix mat(get_lines());

    unordered_set<uint64_t> obstacles;
    for (auto it = mat.begin(); it.is_valid(); ++it) {
        if (*it == '.') {
            *it = 'x';
            count++;
        }

        // part2:
        auto candidate = it.get_pos_64();
        if (candidate == mat.begin().get_pos_64()) 
            continue;
        if (obstacles.contains(candidate))
            continue;
        
        auto old = *it;
        *it = '#';
        if (check_cyclic(mat)) {
            obstacles.insert(candidate);
        }
        *it = old;
    }
    return {count, int(obstacles.size())};
}

int main () {
    auto res = solve();
    cout << res.first << " " << res.second << endl;
}
