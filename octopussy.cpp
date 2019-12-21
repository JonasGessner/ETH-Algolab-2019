//
//  octopussy.cpp
//  Algorithms Lab
//
//  Created by Jonas Gessner
//  Copyright © 2019 Jonas Gessner. All rights reserved.
//

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

void disarmChildTree(const int index, const int n, vector<bool> &disarmed, int *currentTime)
{
    if (disarmed.at(index))
    {
        return;
    }

    if (index <= ((n - 3) / 2)) // We know that the bomb at index has an earlier explosion time than all of its children. It therefore does not matter in which order we disarm the children.
    {
        disarmChildTree(2 * index + 1, n, disarmed, currentTime);
        disarmChildTree(2 * index + 2, n, disarmed, currentTime);
    }

    *currentTime = (*currentTime) + 1;
    disarmed.at(index) = true;
}

static void runTestCase()
{
    int n;
    cin >> n;

    vector<pair<int, int>> timestamps(n);

    for (int i = 0; i < n; i++)
    {
        int x;
        cin >> x;
        timestamps.at(i) = {i, x};
    }

    sort(timestamps.begin(), timestamps.end(), [](const pair<int, int> &lhs, const pair<int, int> &rhs) {
        return lhs.second < rhs.second;
    });

    vector<bool> disarmed(n, false);

    int currentTime = 0;

    for (const pair<int, int> &entry : timestamps)
    {
        int timestamp = entry.second;
        int index = entry.first;

        if (disarmed.at(index))
        {
            continue;
        }

        // Disarm entire child tree of ball at index
        disarmChildTree(index, n, disarmed, &currentTime);

        if (currentTime > timestamp)
        {
            cout << "no\n";
            return;
        }
    }

    cout << "yes\n";
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
