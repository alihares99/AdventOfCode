#pragma once
#include "useful.h"

struct Day07 : public SolutionPair {

    template<bool part2> 
    struct Card {
        // part2:
        inline static constexpr int J_value = part2 ? 1 : 11;
        
        inline static unordered_map<char, int> values = {
            {'A', 14}, 
            {'K', 13}, 
            {'Q', 12}, 
            {'J', J_value}, 
            {'T', 10}, 
            {'9', 9}, 
            {'8', 8}, 
            {'7', 7}, 
            {'6', 6}, 
            {'5', 5}, 
            {'4', 4}, 
            {'3', 3}, 
            {'2', 2},
        };  

        char c;
        Card(char c) : c(c) {}
        bool operator<(Card other) { return values[c] < values[other.c]; }
    };

    template<bool part2>
    struct Hand {
        enum Combinaton {
            HighCard = 0,
            OnePair = 1,
            TwoPair = 2,
            ThreeOfAKind = 3,
            FullHouse = 4,
            FourOfAKind = 5,
            FiveOfAKind = 6,
        };

        array<Card<part2>, 5> cards;
        Combinaton combination;
        Hand(const string& card_str) : cards({card_str[0], card_str[1], card_str[2], card_str[3], card_str[4]}) {
            unordered_map<char, int> same;
            for (auto card : cards) {
                same[card.c]++;
            }

            // part2:
            if (part2) {
                if (same.contains('J') && same.size() > 1) {
                    int j_count = same['J'];
                    same.erase('J');
                    auto it_max = max_element(same.begin(), same.end(), [](auto& it1, auto& it2) { return it1.second < it2.second; });
                    it_max->second += j_count;
                }
            }

            if (same.size() == 1) {
                combination = FiveOfAKind;
            }
            else if (same.size() == 2) {
                int sample = same.begin()->second;
                if (sample == 1 || sample == 4)
                    combination = FourOfAKind;
                else if (sample == 2 || sample == 3)
                    combination = FullHouse;
            }
            else if (same.size() == 3) {
                int max = max_element(same.begin(), same.end(), [](auto& it1, auto& it2) { return it1.second < it2.second; })->second;
                if (max == 3)
                    combination = ThreeOfAKind;
                else
                    combination = TwoPair;
            }
            else if (same.size() == 4) {
                combination = OnePair;
            }
            else {
                combination = HighCard;
            }
        }
        bool operator<(const Hand& other) {
            if (combination != other.combination)
                return combination < other.combination;
            
            for (int i = 0; i < cards.size(); i++) {
                if (cards[i].c != other.cards[i].c)
                    return cards[i] < other.cards[i];
            }
            return false;
        }
    };

    template<bool part2>
    auto get_input(const string& filename) {
        vector<pair<Hand<part2>, int>> games;

        ifstream file(filename);
        string str;
        while (getline(file, str)) {
            istringstream ss(str);
            int bid;
            string card_str;
            ss >> card_str >> bid;

            Hand<part2> hand(card_str);
            games.push_back(make_pair(hand, bid));
        }

        return games;
    }

    template <bool part2>
    uint64_t solve(const string& filename) {
        auto games = get_input<part2>(filename);
        sort(games.begin(), games.end(), [](auto& a, auto& b) { return a.first < b.first; });

        uint64_t res = 0;
        for (int i = 0; i < games.size(); i++) {
            res += games[i].second * (i + 1);
        }
        return res;
    }

    pair<string, string> solve(const string& filename) override {
        return {to_string(solve<false>(filename)), to_string(solve<true>(filename))};
    }
};
