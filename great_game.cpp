//
//  great_game.cpp
//  Algorithms Lab
//
//  Created by Jonas Gessner
//  Copyright © 2019 Jonas Gessner. All rights reserved.
//

#include <iostream>
#include <vector>
#include <climits>

using namespace std;

static int minimumNumberOfStepsNeededToReachEndStartingWithMinimize(vector<vector<int>> &transitions, int position, int n, vector<int> &memoMinimize, vector<int> &memoMaximize);

static int maximumNumberOfStepsNeededToReachEndStartingWithMmaximize(vector<vector<int>> &transitions, int position, int n, vector<int> &memoMinimize, vector<int> &memoMaximize)
{
    if (position == n)
    {
        return 0;
    }

    int memoValue = memoMaximize[position];
    if (memoValue != -1)
    {
        return memoValue;
    }

    int maxSteps = 0;
    for (int &transition : transitions[position])
    {
        maxSteps = max(maxSteps, 1 + minimumNumberOfStepsNeededToReachEndStartingWithMinimize(transitions, transition, n, memoMinimize, memoMaximize));
    }

    memoMaximize[position] = maxSteps;

    return maxSteps;
}

int minimumNumberOfStepsNeededToReachEndStartingWithMinimize(vector<vector<int>> &transitions, int position, int n, vector<int> &memoMinimize, vector<int> &memoMaximize)
{
    if (position == n)
    {
        return 0;
    }

    int memoValue = memoMinimize[position];
    if (memoValue != -1)
    {
        return memoValue;
    }

    int minSteps = INT_MAX;
    for (int &transition : transitions[position])
    {
        minSteps = min(minSteps, 1 + maximumNumberOfStepsNeededToReachEndStartingWithMmaximize(transitions, transition, n, memoMinimize, memoMaximize));
    }

    memoMinimize[position] = minSteps;

    return minSteps;
}

static void runTestCase()
{
    int n;
    cin >> n;
    int m;
    cin >> m;

    int r;
    cin >> r;
    int b;
    cin >> b;

    vector<vector<int>> transitions(n);
    for (int i = 0; i < m; i++)
    {
        int u;
        cin >> u;
        int v;
        cin >> v;

        transitions.at(u).push_back(v);
    }

    vector<int> memoMinimize(n, -1);
    vector<int> memoMaximize(n, -1);

    // Holmes: Begins with red meeple and needs red meeple to finish first to win.
    // Moriarty: Begins with black meeple and needs black meeple to finish first to win.

    // Holmes will always try to minimize the number of steps left from the current position for the red meeple, while Moriarty will try to maximize it. Vice versa for the black meeple.

    // First play only with the black meeple and count the minimum number of steps needed to reach the end.
    int minBlack = minimumNumberOfStepsNeededToReachEndStartingWithMinimize(transitions, b, n, memoMinimize, memoMaximize);

    // Next play only with the red meeple and count the minimum number of steps needed to reach the end.
    int minRed = minimumNumberOfStepsNeededToReachEndStartingWithMinimize(transitions, r, n, memoMinimize, memoMaximize);

    // If the minimum number of steps the red meeple needs are lower than the minimum steps the black meeple needs, Holmes wins.
    bool moriartyWins = minBlack < minRed;

    if (minBlack == minRed)
    {
        moriartyWins = (minBlack % 2) == 0;
    }

    cout << moriartyWins << "\n";
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
