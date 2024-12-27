
#include "useful.h"

vector<uint32_t> get_input(const string& filename) {
    auto lines = get_input_all_lines(filename);
    vector<uint32_t> res(lines.size(), 0);
    for (int i = 0; i < lines.size(); i++) {
        istringstream ss(lines[i]);
        ss >> res[i]; 
    }
    return res;
}

uint32_t get_next(uint32_t in) {
    in = (in ^ (in << 6)) & 0xFFFFFF;
    in = (in ^ ((in >> 5))) & 0xFFFFFF;
    in = (in ^ (in << 11)) & 0xFFFFFF;
    return in;
}

uint64_t part1(const string& filename) {
    uint64_t res = 0;
    auto inputs = get_input(filename);
    for (auto input : inputs) {
        for (int i = 0; i < 2000; i++)
            input = get_next(input);
        res += input;
    }
    return res;
}

uint64_t part2(const string& filename) {
    unordered_map<int, int> bananas;
    auto inputs = get_input(filename);
    for (auto input : inputs) {
        unordered_map<int, int> part;
        int a = 0, b = 0, c = 0, d = 0;
        for (int i = 0; i < 2000; i++) {
            auto next = get_next(input);
            a = b;
            b = c;
            c = d;
            d = (next % 10) - (input % 10);

            if (i >= 3) {
                int key = 0;
                key += a + 10; key *= 100;
                key += b + 10; key *= 100;
                key += c + 10; key *= 100;
                key += d + 10;
                if (!part.contains(key)){
                    part[key] = (next % 10);
                }
            }
            
            input = next;
        }

        for (auto& it : part) {
            bananas[it.first] += it.second;
        }
    }

    auto max_elem = ranges::max_element(bananas, [](auto& a, auto& b) { return a.second < b.second; });
    return max_elem->second;
}

int main() {
    cout << part1("input.txt") << endl;
    cout << part2("input.txt") << endl;
    return 0;
}