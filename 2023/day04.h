#pragma once
#include "useful.h"

struct Day04 : public SolutionPair {

    pair<string, string> solve(const string& filename) override {
        ifstream file(filename);
        vector<int> n_match;
        string str;
        while (getline(file, str)) {
            istringstream ss (str);
            ss.ignore(4);
            int tmp;
            ss >> tmp;
            ss.ignore();

            unordered_set<int> winning;
            int v;
            while (ss >> v) {
                winning.insert(v);
            }

            ss.clear();
            ss.ignore();
            
            int n = 0;
            while (ss >> v) {
                if (winning.contains(v))
                    n++;
            }

            n_match.push_back(n);
        }

        int count = 0;
        for (auto n : n_match) {
            if (n > 0) {
                count += (1 << (n - 1));
            }
        }

        vector<int> acc (n_match.size(), 1);
        for (int i = 0; i < n_match.size(); i++) {
            auto n = n_match[i];
            int j = i + 1;
            for (int cc = 0; cc < n; j++, cc++) {
                acc[j] += acc[i];
            }
        }

        return {to_string(count), to_string(accumulate(acc.begin(), acc.end(), 0))};
    }
    
};
