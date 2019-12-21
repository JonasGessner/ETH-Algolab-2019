//
//  magician_coin.cpp
//  Algorithms Lab
//
//  Created by Jonas Gessner
//  Copyright © 2019 Jonas Gessner. All rights reserved.
//

#include <algorithm>
#include <cassert>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <limits>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

inline int minimumMoneyAfterRevealOnDay(const int day, const int endMoney, const int n) {
    return ceil(endMoney * pow(0.5, (n - 1 - day)));
}

// 0 ... n-1
inline int requiredBetOnDayToMeetMinimumAfterWinning(const int day, const int currentWinning, const int endMoney, const int n) {
    const int minimumOutgoing = minimumMoneyAfterRevealOnDay(day, endMoney, n);

    if (currentWinning >= minimumOutgoing) {
        return 0;
    }

    const int diff = minimumOutgoing - currentWinning;

    return diff;
}

// 0 ... n-1
inline int maxAccepptableLosingBetOnDay(const int day, const int currentWinning, const int endMoney, const int n) {
    const int minimumOutgoing = minimumMoneyAfterRevealOnDay(day, endMoney, n);

    if (currentWinning < minimumOutgoing) {
        return -1; // Cannot afford to lose anything
    }

    const int diff = currentWinning - minimumOutgoing;

    return diff;
}

double bet(const int day, const int n, const int currentMoney, const int endMoney, const vector<double> &winP, vector<vector<double>> &memo) {
    if (currentMoney >= endMoney) {
        return 1.0;
    }

    if (day >= n) {
        return 0.0;
    }

    double memoVal = memo.at(day).at(currentMoney);

    if (memoVal >= 0.0) {
        return memoVal;
    }

    // int minMoney = minimumMoneyAfterRevealOnDay(day - 1, endMoney, n);

    // assert(currentMoney >= minMoney);

    // int minimumBet = requiredBetOnDayToMeetMinimumAfterWinning(day, currentMoney, endMoney, n);

    // int maxAcceptableLoss = maxAccepptableLosingBetOnDay(day, currentMoney, endMoney, n);

    double winChance = winP.at(day);
    double loseChance = 1.0 - winChance;

    double bestCombo = 0.0;

    for (int i = 0; i <= currentMoney; i++) { // All possible bets
        if (i == 0) {
            bestCombo = max(bestCombo, bet(day + 1, n, currentMoney, endMoney, winP, memo));
        } else {
            double lose;
            // if (i > 0 && i <= maxAcceptableLoss) { // Can afford to lose, continue game with lost money
            lose = bet(day + 1, n, currentMoney - i, endMoney, winP, memo);
            // } else {
            //     lose = 0.0;
            // }

            double win = bet(day + 1, n, currentMoney + i, endMoney, winP, memo);

            double combo = (winChance * win) + (loseChance * lose);

            bestCombo = max(bestCombo, combo);
        }
    }

    memo.at(day).at(currentMoney) = bestCombo;

    return bestCombo;
}

void testcase() {
    int n, startMoney, endMoney;
    cin >> n >> startMoney >> endMoney;

    vector<double> winP(n);
    for (int i = 0; i < n; i++) {
        cin >> winP.at(i);
    }

    // 0 ... n-1

    if (minimumMoneyAfterRevealOnDay(-1, endMoney, n) > startMoney) {
        cout << 0.0 << "\n";
        return;
    }

    vector<vector<double>> memo(n, vector<double>(endMoney, -1.0));

    cout << bet(0, n, startMoney, endMoney, winP, memo) << "\n";
}

int main() {
    std::ios_base::sync_with_stdio(false);

    cout << fixed << setprecision(5);

    int t;
    std::cin >> t;
    for (int i = 0; i < t; ++i)
        testcase();
}
