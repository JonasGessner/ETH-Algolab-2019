//
//  DHL.cpp
//  Algorithms Lab
//
//  Created by Jonas Gessner
//  Copyright © 2019 Jonas Gessner. All rights reserved.
//

#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <limits>
#include <string>
#include <vector>

using namespace std;

// aIndex and bIndex point to the last element that was taken
int minimizeLoadingCost(int aIndex, int bIndex, const vector<int> &a, const vector<int> &b, const vector<int> &aSums, const vector<int> &bSums, vector<vector<int>> &memo) {
    if (aIndex == bIndex && aIndex == 0) {
        return 0;
    } else if (aIndex == 0 || bIndex == 0) {
        return numeric_limits<int>::max();
    }

    if (aIndex == 1) {
        return bSums[bIndex - 1] * a[aIndex - 1];
    } else if (bIndex == 1) {
        return aSums[aIndex - 1] * b[bIndex - 1];
    }

    int memoVal = memo[aIndex][bIndex];

    if (memoVal != -1) {
        return memoVal;
    }

    // Yo this works because of the distributive property: (a1 + ... + an) * (b1 + ... + bm) = a1 * b1 + a2 * b1 + ... + an * b1 + a2 * b1 + ... an * bm

    int bestCost = min(
        minimizeLoadingCost(aIndex - 1, bIndex - 1, a, b, aSums, bSums, memo), // Take this pairt of two
        min(
            minimizeLoadingCost(aIndex - 1, bIndex, a, b, aSums, bSums, memo), // Take this pair of two and one additional on the left
            minimizeLoadingCost(aIndex, bIndex - 1, a, b, aSums, bSums, memo)) // Take this pair of two and one additional on the right
    );

    int cost = a[aIndex - 1] * b[bIndex - 1];

    bestCost += cost;

    memo[aIndex][bIndex] = bestCost;

    return bestCost;
}

void testcase() {
    int n;
    cin >> n;

    vector<int> a;
    vector<int> aSums;
    a.reserve(n);
    aSums.reserve(n);
    for (int i = 0; i < n; i++) {
        int x;
        cin >> x;
        x--;
        a.push_back(x);
        if (i > 0) {
            x += aSums[i - 1];
            aSums.push_back(x);
        } else {
            aSums.push_back(x);
        }
    }

    vector<int> b;
    vector<int> bSums;
    b.reserve(n);
    bSums.reserve(n);
    for (int i = 0; i < n; i++) {
        int x;
        cin >> x;
        x--;
        b.push_back(x);
        if (i > 0) {
            x += bSums[i - 1];
            bSums.push_back(x);
        } else {
            bSums.push_back(x);
        }
    }

    vector<vector<int>> dp(n, vector<int>(n, 0));

    dp[0][0] = a[0] * b[0];

    for (int i = 1; i < n; i++) {
        dp[0][i] = a[0] * bSums[i];
        dp[i][0] = aSums[i] * b[0];
    }

    for (int i = 1; i < n; i++) {
        for (int j = 1; j < n; j++) {
            dp[i][j] = a[i] * b[j] + min(dp[i - 1][j - 1], min(dp[i - 1][j], dp[i][j - 1]));
        }
    }

    cout << dp[n - 1][n - 1] << "\n";

    vector<vector<int>> memo(n + 1, vector<int>(n + 1, -1));
    // cout << minimizeLoadingCost(n, n, a, b, aSums, bSums, memo) << "\n";
}

int main() {
    std::ios_base::sync_with_stdio(false);

    int t;
    std::cin >> t;
    for (int i = 0; i < t; ++i)
        testcase();
}