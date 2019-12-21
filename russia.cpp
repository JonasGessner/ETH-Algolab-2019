//
//  russia.cpp
//  Algorithms Lab
//
//  Created by Jonas Gessner
//  Copyright © 2019 Jonas Gessner. All rights reserved.
//

using namespace std;

int maxProfit(int windowOffset, int windowLength, int opponentCount, vector<int> &coins, vector<vector<int>> &memo)
{
    if (windowLength <= 0)
    {
        return 0;
    }

    int memoVal = memo[windowOffset][windowLength];

    if (memoVal != -1)
    {
        return memoVal;
    }

    int nextWindowLength = windowLength - opponentCount - 1;

    // First try taking left coin
    int minRemainingProfitAfterTakingLeftCoin = INT_MAX;

    for (int i = (windowOffset + 1); i <= (windowOffset + 1 + opponentCount); i++)
    {
        minRemainingProfitAfterTakingLeftCoin = min(
            minRemainingProfitAfterTakingLeftCoin,
            maxProfit(i, nextWindowLength, opponentCount, coins, memo));
    }

    // Then try taking right coin.
    int minRemainingProfitAfterTakingRightCoin = INT_MAX;

    for (int i = windowOffset; i <= (windowOffset + opponentCount); i++)
    {
        minRemainingProfitAfterTakingRightCoin = min(
            minRemainingProfitAfterTakingRightCoin,
            maxProfit(i, nextWindowLength, opponentCount, coins, memo));
    }

    // In both cases pick the minimum of all possible next moves as the best profit achievable without knowing the strategy of others.

    // The best we can do independently of what the others do is the best choice this move offers us.
    int maxProfitForMove = max(coins[windowOffset] + minRemainingProfitAfterTakingLeftCoin,
                               coins[windowOffset + windowLength - 1] + minRemainingProfitAfterTakingRightCoin);

    memo[windowOffset][windowLength] = maxProfitForMove;

    return maxProfitForMove;
}

static void runTestCase()
{
    int n;
    cin >> n;
    int m;
    cin >> m;
    int k;
    cin >> k;

    vector<int> coins(n);

    for (int i = 0; i < n; i++)
    {
        int x;
        cin >> x;
        coins[i] = x;
    }

    vector<vector<int>> memo(n + 1, vector<int>(n + 1, -1));

    int minProfit = INT_MAX;

    for (int i = 0; i <= k; i++)
    {
        minProfit = min(minProfit, maxProfit(i, n - k, (m - 1), coins, memo));
    }

    cout << minProfit << "\n";
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
