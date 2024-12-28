
#include "useful.h"

using Connection_t = unordered_map<string, unordered_set<string>>;

auto get_input(const string& filename) {
    auto lines = get_input_all_lines(filename);
    Connection_t connections;
    for (auto& line : lines) {
        auto s1 = line.substr(0, 2);
        auto s2 = line.substr(3, 2);
        connections[min(s1, s2)].insert(max(s1, s2));
    }
    return connections;
}

int part1(const string& filename) {
    auto connections = get_input(filename);
    int n = 0;
    for (auto& it : connections) {
        const string& s1 = it.first;
        for (const string& s2 : it.second) {
            for (const string& s3 : connections[s2]) {
                if (connections[s1].contains(s3)
                    && (s1[0] == 't' || s2[0] == 't' || s3[0] == 't')
                ) {
                    n++;
                }
            }
        }
    }
    return n;
}

void find(vector<string> prevs, Connection_t& connections, string& largest) {
    size_t n = (largest.size() + 1) / 3;
    if (prevs.size() + connections[prevs.back()].size() <= n)
        return;

    bool added = false;
    for (auto& str : connections[prevs.back()]) {
        bool prevs_connected = true;
        for (int i = 0; i < prevs.size() - 1; i++) {
            if (!connections[prevs[i]].contains(str)) {
                prevs_connected = false;
                break;
            }
        }

        if (prevs_connected) {
            auto tmp = prevs;
            tmp.push_back(str);
            find(tmp, connections, largest);
            added = true;
        }
    }

    if (!added && prevs.size() > n) {
        largest = join_strings(prevs, ',');
    }   
}

string part2(const string& filename) {
    auto connections = get_input(filename);
    string largest;
    for (auto& it : connections) {
        find({it.first}, connections, largest);
    }
    return largest;
}

int main() {
    cout << part1("input.txt") << endl;
    cout << part2("input.txt") << endl;
    return 0;
}