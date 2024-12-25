
#include "useful.h"

struct Maze {
    vector<string> mat;
    int n_row, n_col;
    Point start, finish;
    vector<vector<int>> distances;

    Maze(vector<string>&& mat_) : mat(move(mat_)) {
        n_row = int(mat.size());
        n_col = int(mat[0].size());
        int found = 0;
        for (int i = 0; i < n_row; i++) {
            for (int j = 0; j < n_col; j++) {
                if (mat[i][j] == 'S') {
                    start = {i, j};
                    mat[i][j] = '.';
                    found ++;
                }
                else if (mat[i][j] == 'E') {
                    finish = {i, j};
                    mat[i][j] = '.';
                    found ++;
                }

                if (found == 2)
                    break;
            }
        }

        distances.resize(n_row, vector<int>(n_col, -1));
        int n = 0;
        for (auto it = begin(); it.is_valid(); ++it) {
            it.distance() = n++;
        }
    }

    struct iterator {
        DirectedPoint p;
        Maze* maze;

        iterator(Maze* maze) : maze(maze) {}

        iterator& operator++() {
            for (int i = 0; i < 3; i++) {
                auto tmp = *this;
                if (i == 1) tmp.p.turn_left();
                else if (i == 2) tmp.p.turn_right();
                tmp.p.move();
                if (*tmp == '.') {
                    p = tmp.p;
                    return *this;
                }
            }
            
            p.i = -1;
            p.j = -1;
            return *this;
        }

        iterator clone_to(Point p) {
            iterator tmp = *this;
            static_cast<Point&>(tmp.p) = p;
            return tmp;
        }

        bool is_valid() {
            return p.i >= 0 && p.j >= 0 && p.i < maze->n_row && p.j < maze->n_col;
        }

        int& distance() {
            return maze->distances[p.i][p.j];
        }

        char& operator*() {
            return maze->mat[p.i][p.j];
        }
    };

    iterator begin() {
        iterator it(this);
        static_cast<Point&>(it.p) = start;
        for (auto dir : {dir_up, dir_down, dir_left, dir_right}) {
            it.p.dir = dir;
            auto tmp = it;
            tmp.p.move();
            if (*tmp == '.') 
                break;
        }
        return it;
    }
};

auto get_input(const string& filename) {
    auto lines = get_input_all_lines(filename);
    Maze maze(move(lines));
    return maze;
}

int part1(const string& filename, int min_save) {
    auto maze = get_input(filename);

    vector<int> cheats;
    for (auto it = maze.begin(); it.is_valid(); ++it) {
        auto p = it;
        for (int i = 0; i < 3; i++) {
            auto tmp = p;
            if (i == 1) tmp.p.turn_left();
            if (i == 2) tmp.p.turn_right();
            tmp.p.move();
            if (!tmp.is_valid() || tmp.distance() != -1) 
                continue;
            tmp.p.move();
            if (!tmp.is_valid() || tmp.distance() == -1)
                continue;
            
            auto t2 = tmp.distance();
            auto t1 = it.distance();
            if (t2 < t1 - 2)
                cheats.push_back(t1 - 2 - t2);
        }
    }
    ranges::sort(cheats);
    auto it = lower_bound(cheats.begin(), cheats.end(), min_save);
    return int(cheats.end() - it);
}

int part2(const string& filename, int min_save) {
    auto maze = get_input(filename);
    vector<int> cheats;
    cheats.reserve(2000000);
    for (auto it = maze.begin(); it.is_valid(); ++it) {
        int i_start = max(it.p.i - 20, 0);
        int i_end = min(it.p.i + 20, maze.n_row - 1);
        for (int i = i_start; i <= i_end; i++) {
            int j_start = max(it.p.j - (20 - abs(i - it.p.i)), 0);
            int j_end = min(it.p.j + (20 - abs(i - it.p.i)), maze.n_col - 1);
            for (int j = j_start; j <= j_end; j++) {
                int dist = abs(it.p.i - i) + abs(it.p.j - j);
                auto tmp = it.clone_to({i, j});
                int discount = tmp.distance() - it.distance() - dist;
                if (discount > 0)
                    cheats.push_back(discount);
            }
        }
    }

    ranges::sort(cheats);
    auto it = lower_bound(cheats.begin(), cheats.end(), min_save);
    return int(cheats.end() - it);
}

int main() {
    cout << part1("input.txt", 100) << endl;
    cout << part2("input.txt", 100) << endl;
    return 0;
}