
#include "useful.h"

auto get_input() {
    set<string> towels;
    vector<string> designs;

    auto lines = get_input_all_lines("input.txt");
    
    istringstream ss(lines[0]);
    string towel;
    while (getline(ss, towel, ',')) {
        towels.insert(towel);
        ss.ignore();
    }
    
    for (int i = 2; i < lines.size(); i++) {
        designs.push_back(lines[i]);
    }
    
    return make_pair(towels, designs);
}

uint64_t is_possible(const set<string>& towels, string& design) {
    vector<uint64_t> dp(design.size(), 0);
    for (int i = 0; i < design.size(); i++) {
        if (i > 0 && dp[i - 1] == 0) 
            continue;
        
        uint64_t n_ways_so_far = i > 0 ? dp[i - 1] : 1;
        char c = design[i];
        for (auto towel_it = towels.lower_bound(string({c})); 
            towel_it != towels.end() && towel_it->at(0) == c; ++towel_it
            ) {
            int n = i + int(towel_it->size());
            if (n > design.size()) {
                continue;
            }
            string_view sub_design(design.begin() + i, design.begin() + n);
            if (sub_design == *towel_it) {
                dp[n - 1] += n_ways_so_far;
            }
        }
    }
    
    return dp.back();
}

int main() {
    auto [towels, designs] = get_input();

    // part1:
    int count_possible = 0;
    for (auto& design : designs) {
        if (is_possible(towels, design) > 0) 
            count_possible++;
    }
    cout << count_possible << endl;

    // part2:
    uint64_t n_possibility = 0;
    for (auto& design : designs) {
        n_possibility += is_possible(towels, design);
    }
    cout << n_possibility << endl;
    
    return 0;
}