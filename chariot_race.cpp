//
//  chariot_race.cpp
//  Algorithms Lab
//
//  Created by Jonas Gessner
//  Copyright © 2019 Jonas Gessner. All rights reserved.
//

#include <iostream>
#include <vector>
#include <limits>
#include <unordered_set>
#include <algorithm>
#include <array>

using namespace std;

static int save(const vector<int> &costs, const vector<vector<int>> &stages, int city, bool parentIsRepaired, vector<array<int, 2>> &memo)
{
    int memoVal = memo.at(city).at(parentIsRepaired);

    if (memoVal != -1)
    {
        return memoVal;
    }

    if (stages.at(city).size() == 0)
    {
        if (parentIsRepaired)
        {
            return 0;
        }
        else
        {
            return costs.at(city);
        }
    }

    int costRepairOthers;

    if (parentIsRepaired)
    {
        costRepairOthers = 0;
        for (auto &&child : stages.at(city))
        {
            costRepairOthers += save(costs, stages, child, false, memo);
        }
    }
    else // Self is not saved by its parent — one child must be repaired at least to save self
    {
        costRepairOthers = 0;
        int currentBestDiff = numeric_limits<int>::max();

        // We need to pick option A for at least one child — pick the one where optionA - optionB is minimal
        // To pick the best child to save we do the following:
        // For each child calculate the cost of repairing the child and then saving its child trees (optionA) or not repairing the child and then saving its child trees (optionB).
        // Take the diff between those two, clamped to [0, inf).
        // We choose the child to save as the one where this diff is the lowest. If the diff is zero then choosing optionA is not worse than choosing optionB anyway, in which case we would repair this child and automatically satisfy the condition of repairing at least one child to save self.
        // We do this iteratively: When a diff is lower than the last lowest diff, we subtract the older diff from the total cost (which means we chose the more optimal option for the child that yielded the previous lowest diff). set the diff to the new diff and choose optionA.
        for (auto &&child : stages.at(city))
        {
            int optionA = costs.at(child);
            for (int grandChild : stages.at(child))
            {
                optionA += save(costs, stages, grandChild, true, memo);
            }

            int optionB = save(costs, stages, child, false, memo);

            int diff = max(0, optionA - optionB);

            if (diff < currentBestDiff)
            {
                if (currentBestDiff < numeric_limits<int>::max())
                {
                    costRepairOthers -= currentBestDiff;
                }
                costRepairOthers += optionA;

                currentBestDiff = diff;
            }
            else
            {
                costRepairOthers += min(optionA, optionB);
            }
        }
    }

    int val;

    int costRepairSelf = costs.at(city);

    if (costRepairSelf < costRepairOthers) // Small optimization
    {
        for (auto &&child : stages.at(city))
        {
            costRepairSelf += save(costs, stages, child, true, memo);
        }

        val = min(costRepairSelf, costRepairOthers);
    }
    else
    {
        val = costRepairOthers;
    }

    memo.at(city).at(parentIsRepaired) = val;

    return val;
}
static void runTestCase()
{
    int n;
    cin >> n;

    vector<vector<int>> stages(n);
    vector<int> costs(n);

    for (int i = 1; i < n; i++)
    {
        int start;
        cin >> start;
        int end;
        cin >> end;

        stages.at(start).push_back(end);
    }

    for (int i = 0; i < n; i++)
    {
        int x;
        cin >> x;

        costs.at(i) = x;
    }

    vector<array<int, 2>> memo(n, {-1, -1});

    cout << save(costs, stages, 0, false, memo) << "\n";
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
