//
//  burning_coins.cpp
//  Algorithms Lab
//
//  Created by Jonas Gessner
//  Copyright © 2019 Jonas Gessner. All rights reserved.
//

#include <iostream>
#include <vector>

using namespace std;

int maxProfit(vector<int> &coins, int offset, int length, vector<vector<int>> &memo)
{
    if (length <= 0)
    {
        return 0;
    }

    int memoVal = memo.at(offset).at(length);
    if (memoVal != -1)
    {
        return memoVal;
    }

    int takeLeftCoin = coins[offset] + min(maxProfit(coins, offset + 1, length - 2, memo), maxProfit(coins, offset + 2, length - 2, memo));
    // Take first coin (decreases length by 1 and increases offset by 1). Opponent can then take the last coin on their move (which leaves offset equal and decreases length by 1), or the first coin on their move (which increases offset by 1 and decreases length by 1).

    int takeRightCoin = coins[offset + length - 1] + min(maxProfit(coins, offset, length - 2, memo), maxProfit(coins, offset + 1, length - 2, memo));
    // Take last coin (decreases length by 1). Opponent can then take the last coin on their move (which leaves offset equal and decreases length by 1), or the first coin on their move (which increases offset by 1 and decreases length by 1).

    int val = max(takeLeftCoin, takeRightCoin);

    memo.at(offset).at(length) = val;

    return val;
}

static void runTestCase()
{
    int n;
    cin >> n;
    vector<int> coins(n);
    vector<vector<int>> memo(n, vector<int>(n + 1, -1));

    for (int i = 0; i < n; i++)
    {
        int x;
        cin >> x;
        coins[i] = x;
    }

    cout << maxProfit(coins, 0, n, memo) << "\n";
}

int main(int argc, const char *argv[])
{
    ios_base::sync_with_stdio(false);

    int t;
    cin >> t;

    for (int i = 0; i < t; i++)
    {
        runTestCase();
    }

    return 0;
}
