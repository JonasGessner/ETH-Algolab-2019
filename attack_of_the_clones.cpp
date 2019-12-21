//
//  attack_of_the_clones.cpp
//  Algorithms Lab
//
//  Created by Jonas Gessner
//  Copyright © 2019 Jonas Gessner. All rights reserved.
//

#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>
#include <unordered_set>

using namespace std;

bool isWithinRange(int start, int end, int probe)
{
    if (start <= end)
    {
        return probe >= start && probe <= end;
    }
    else
    {
        if (probe == start || probe == end)
        {
            return true;
        }
        else
        {
            return !isWithinRange(end, start, probe);
        }
    }
}

int rangeSize_(int l, int r, int m)
{
    int diff = r - l;

    if (diff >= 0)
    {
        return diff + 1;
    }
    else
    {
        return m + diff + 1;
    }
}

int rangeSize(const pair<int, int> &range, int m)
{
    return rangeSize_(range.first, range.second, m);
}

int doEFTScheduling(int start, int n, int m, const vector<pair<int, int>> &ranges, unordered_set<int> &ignore)
{
    int rangeStart = 0;
    int currentClosest = numeric_limits<int>::max();

    for (int i = 0; i < n; i++)
    {
        if (ignore.find(i) != ignore.end())
        {
            continue;
        }

        int diff = rangeSize_(start, ranges.at(i).second, m);
        if (diff < currentClosest)
        {
            currentClosest = diff;
            rangeStart = i;
        }
    }

    int count = 1;
    int usedStart = ranges.at(rangeStart).first;
    int usedEnd = ranges.at(rangeStart).second;
    int size = rangeSize_(usedStart, usedEnd, m);

    for (int i = 1; i < n; i++)
    {
        int currentRangeIndex = (i + rangeStart) % n;

        if (ignore.find(currentRangeIndex) != ignore.end())
        {
            continue;
        }

        const auto &currentRange = ranges.at(currentRangeIndex);

        if (!isWithinRange(usedStart, usedEnd, currentRange.first) && !isWithinRange(usedStart, usedEnd, currentRange.second) && !isWithinRange(currentRange.first, currentRange.second, usedStart))
        {
            usedEnd = currentRange.second;
            int newSize = rangeSize_(usedStart, usedEnd, m);

            if (newSize < size)
            {
                break;
            }

            size = newSize;
            count++;
        }
    }

    return count;
}

static void runTestCase()
{
    int n, m;
    cin >> n >> m;

    vector<pair<int, int>> ranges(n);

    for (int i = 0; i < n; i++)
    {
        int a, b;
        cin >> a >> b;
        ranges.at(i) = {a - 1, b - 1};
    }

    // Sort ranges by end (finish time) ascending
    sort(ranges.begin(), ranges.end(), [&](const pair<int, int> &lhs, const pair<int, int> &rhs) {
        return lhs.second < rhs.second || (lhs.second == rhs.second && rangeSize(lhs, m) < rangeSize(rhs, m));
    });

    // This removes all but the shortest range for all ranges with the same finish time.
    for (int i = 0; i < ranges.size() - 1; i++)
    {
        const auto range = ranges.at(i);
        while (i < ranges.size() - 1 && ranges.at(i + 1).second == range.second)
        {
            ranges.erase(ranges.begin() + i + 1);
        }
    }

    vector<int> overlappers(10);

    // Find spot covered by small number of ranges. Such a spot has to exit after the end of one range.
    for (int i = 0; i < ranges.size(); i++)
    {
        const auto &cur = ranges.at(i);
        int nextVal = cur.second + 1;

        int overlaps = 0;
        bool ok = true;
        for (int j = 0; j < ranges.size() - 1; j++)
        {
            int otherIndex = (i + j + 1) % ranges.size();

            const auto &other = ranges.at(otherIndex);

            if (isWithinRange(other.first, other.second, nextVal))
            {
                overlaps++;

                if (overlaps > 10)
                {
                    ok = false;
                    break;
                }

                overlappers.at(overlaps - 1) = otherIndex;
            }
        }

        if (ok)
        {
            unordered_set<int> empty;
            unordered_set<int> allIgnore;
            allIgnore.reserve(overlaps);

            // For this spot, the number of scenarios is limited to a small number now:

            // Test all scenarios: Test using each of the ranges that cover this spot.
            int maxCount = 0;
            for (int j = 0; j < overlaps; j++)
            {
                int rangeIndex = overlappers.at(j);
                allIgnore.insert(rangeIndex);
                maxCount = max(maxCount, doEFTScheduling(ranges.at(rangeIndex).second, ranges.size(), m, ranges, empty));
            }

            // Or don't use any of the ranges that overlap this spot.
            maxCount = max(maxCount, doEFTScheduling(nextVal, ranges.size(), m, ranges, allIgnore));
            cout
                << maxCount << "\n";
            return;
        }
    }

    cout << "BROKEN!\n";
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
