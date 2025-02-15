#pragma once
#include "useful.h"

struct Day16 : public Solution {
    vector<string> maze;
    int n_row;
    int n_col;
    Point end_point;
    Point start_point;
    vector<vector<array<int, 4>>> min_scores;
    int min_score;
    unordered_set<Point> best_path;
    optional<pair<int, size_t>> result;

    struct Reindeer {
        const vector<string>& maze;
        Point p;
        Direction dir = dir_right;
        int score = 0;

        Reindeer(Point p, const vector<string>& maze) : p(p), maze(maze) {}
        Reindeer(const Reindeer& n) : maze(n.maze), p(n.p), dir(n.dir), score(n.score) {}
        Reindeer& operator=(const Reindeer& n) { p = n.p; dir = n.dir; score = n.score; return *this; }
        Reindeer copy() { return *this; }
        Reindeer& go_ahead() {
            p.move(dir);
            score ++;
            return *this;
        }
        Reindeer& go_right() {
            dir = rotate_clockwise(dir);
            score += 1000;
            go_ahead();
            return *this;
        }
        Reindeer& go_left() {
            dir = rotate_counter_clockwise(dir);
            score += 1000;
            go_ahead();
            return *this;
        }
        bool hit_wall() {
            return maze[p.i][p.j] == '#';
        }
    };

    void run(Reindeer r) {
        while (r.p != end_point) {    
            Reindeer r1 = r.copy().go_ahead();
            Reindeer r2 = r.copy().go_left();
            Reindeer r3 = r.copy().go_right();

            vector<Reindeer> valids;
            if (!r1.hit_wall()) valids.push_back(r1);
            if (!r2.hit_wall()) valids.push_back(r2);
            if (!r3.hit_wall()) valids.push_back(r3);
            if (valids.size() == 0)
                return;
            
            r = valids[0];
            if (min_scores[r.p.i][r.p.j][r.dir] <= r.score)
                return;
            min_scores[r.p.i][r.p.j][r.dir] = r.score;

            for (int i = 1; i < valids.size(); i++) {
                run(valids[i]);
            }
        }
    }

    void run_part2(Reindeer r, vector<Point> points = {}) {
        while (r.p != end_point) {    
            Reindeer r1 = r.copy().go_ahead();
            Reindeer r2 = r.copy().go_left();
            Reindeer r3 = r.copy().go_right();

            vector<Reindeer> valids;
            if (!r1.hit_wall()) valids.push_back(r1);
            if (!r2.hit_wall()) valids.push_back(r2);
            if (!r3.hit_wall()) valids.push_back(r3);
            if (valids.size() == 0)
                return;
            
            r = valids[0];
            if (min_scores[r.p.i][r.p.j][r.dir] != r.score)
                return;

            for (int i = 1; i < valids.size(); i++) {
                vector<Point> tmp = points;
                tmp.push_back(valids[i].p);
                run_part2(valids[i], move(tmp));
            }
            points.push_back(r.p);
        }

        if (r.score == min_score) {
            for (auto& p : points)
                best_path.insert(p);
        }
    }

    void solve(const string& filename) {
        if (result.has_value())
            return;
        maze = get_input_all_lines(filename);
        n_row = int(maze.size());
        n_col = int(maze[0].size());
        start_point = Point(n_row - 2, 1);
        end_point = Point(1, n_col - 2);

        min_scores.resize(n_row);
        for (auto& vec : min_scores) {
            auto v = numeric_limits<int>::max();
            vec.resize(n_col, {v, v, v, v});
        }
        min_scores[0][0][0] = 0;
        min_scores[0][0][1] = 0;
        min_scores[0][0][2] = 0;
        min_scores[0][0][3] = 0;

        Reindeer r { start_point, maze };
        run(r);

        min_score =                min_scores[end_point.i][end_point.j][0];
        min_score = min(min_score, min_scores[end_point.i][end_point.j][1]);
        min_score = min(min_score, min_scores[end_point.i][end_point.j][2]);
        min_score = min(min_score, min_scores[end_point.i][end_point.j][3]);
        
        Reindeer r2 { start_point, maze };
        run_part2(r2);

        result = make_pair(min_score, best_path.size() + 1);
    }

    string part1(const string& filename) override {
        solve(filename);
        return to_string(result->first);
    }

    string part2(const string& filename) override {
        solve(filename);
        return to_string(result->second);
    }
};
