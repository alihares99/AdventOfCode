
#include "useful.h"

auto get_input() {
    auto lines = get_input_all_lines("input.txt");
    vector<array<int, 5>> locks;
    vector<array<int, 5>> keys;
    
    
    for (int i = 0; i < lines.size(); i += 8) {
        bool is_lock = lines[i][0] == '#';
        array<int, 5> heights {0};
        for (int j = 1; j < 6; j++) {
            for (int k = 0; k < 5; k++) {
                if (lines[i + j][k] == '#')
                    heights[k]++;
            }
        }
        if (is_lock)
            locks.push_back(heights);
        else
            keys.push_back(heights);
    }
    return make_pair(locks, keys);
}

int part1() {
    auto [locks, keys] = get_input();
    int count = 0;
    for (int i = 0; i < locks.size(); i++) {
        for (int j = 0 ; j < keys.size(); j++) {
            bool match = true;
            for (int k = 0; k < 5; k++) {
                if (locks[i][k] + keys[j][k] > 5) {
                    match = false;
                    break;
                }
            }

            if (match) {
                count++;
            } 
        }
    }
    
    return count;
}

int main() {
    cout << part1() << endl;
    return 0;
}