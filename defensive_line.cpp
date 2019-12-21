//
//  defensive_line.cpp
//  Algorithms Lab
//
//  Created by Jonas Gessner
//  Copyright © 2019 Jonas Gessner. All rights reserved.
//

#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <stdexcept>

using namespace std;

int findLongestDisjointRanges(int rangeIndex, int attacksLeft, const vector<pair<int, int>> &ranges, const vector<int> &independentPredecessorLookup, vector<vector<int>> &memo)
{
    if (attacksLeft == 0)
    {
        return 0;
    }

    if (rangeIndex < 0)
    {
        return -1;
    }

    const int memoVal = memo.at(rangeIndex).at(attacksLeft);

    if (memoVal != -1)
    {
        if (memoVal == -2)
        {
            return -1;
        }
        else
        {
            return memoVal;
        }
    }

    const int skipRange = findLongestDisjointRanges(rangeIndex - 1, attacksLeft, ranges, independentPredecessorLookup, memo);
    int rangeIndexForFit = independentPredecessorLookup.at(ranges.at(rangeIndex).first);
    const int useThisRange = rangeIndexForFit >= 0 ? findLongestDisjointRanges(rangeIndexForFit, attacksLeft - 1, ranges, independentPredecessorLookup, memo) : (attacksLeft == 1 ? 0 : -1); // If this range is the only range that fits, it is only accepted if we are only looking for one attack

    if (skipRange == -1 && useThisRange == -1)
    {
        memo.at(rangeIndex).at(attacksLeft) = -2;
        return -1;
    }
    else
    {
        int best = max(skipRange, useThisRange + ranges.at(rangeIndex).second - ranges.at(rangeIndex).first);
        memo.at(rangeIndex).at(attacksLeft) = best;
        return best;
    }
}

void runTestCase()
{
    int n, m, k;
    cin >> n >> m >> k;

    vector<int> values;
    values.reserve(n);
    for (int i = 0; i < n; i++)
    {
        int val;
        cin >> val;
        values.push_back(val);
    }

    // Use sliding window to find all intervals that sum to k
    int l = 0;
    int r = 0;
    int currentSum = 0;

    vector<pair<int, int>> ranges; // [l, r)

    while (r <= n)
    {
        while (currentSum > k) // Keep up with left
        {
            currentSum -= values.at(l);
            l++;
        }

        if (currentSum == k) // If left and right match
        {
            ranges.push_back({l, r});
        }

        if (r < n)
        {
            int atR = values.at(r); // Advance right
            currentSum += atR;
        }
        r++;
    }

    sort(ranges.begin(), ranges.end(), [](const pair<int, int> &lhs, const pair<int, int> &rhs) {
        return lhs.second < rhs.second;
    });

    if (ranges.size() < m)
    {
        cout << "fail\n";
        return;
    }

    vector<int> independentPredecessorLookup(n); // For a given index, this gives the index of the closest predecessor range that is independent
    int currentRangeIndex = -1;
    for (int i = 0; i < n; i++)
    {
        int nextIndex = currentRangeIndex;
        while (nextIndex + 1 < ranges.size() && ranges.at(nextIndex + 1).second <= i)
        {
            nextIndex++;
        }

        currentRangeIndex = nextIndex;
        independentPredecessorLookup.at(i) = currentRangeIndex;
    }

    vector<vector<int>> memo(ranges.size(), vector<int>(m + 1, -1));
    // Find the m largest disjoint ranges in the ranges vector, if exists
    int value = findLongestDisjointRanges(ranges.size() - 1, m, ranges, independentPredecessorLookup, memo);

    if (value == -1)
    {
        cout << "fail\n";
    }
    else
    {
        cout << value << "\n";
    }
}

int main(int argc, char const *argv[])
{
    ios_base::sync_with_stdio(false);
    int t;
    cin >> t;
    while (t-- > 0)
    {
        runTestCase();
    }
}
