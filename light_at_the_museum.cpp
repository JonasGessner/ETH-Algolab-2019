//
//  light_at_the_museum.cpp
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

using namespace std;

vector<uint32_t> configurationsThatIlluminateRoom(const vector<int> &targetBrightnesses, const vector<int> &currentBrightnesses, const vector<vector<int>> &modifiers, int room, int n, int m)
{
    vector<uint32_t> res;

    // split & list
    int limit1 = (1 << (n / 2));
    int limit2 = (1 << ((n / 2) + (n % 2)));
    int secondIndexOffset = n / 2;

    // The modifiers for the specified room by all configurations of the first half of switches
    vector<pair<uint32_t, int>> firstModifiers(limit1);

    for (uint32_t i = 0; i < limit1; i++) // for every light switch configuration
    {
        int sum = 0;

        for (int k = 0; k < n; k++) // For every switch
        {
            if (((i >> k) & 1) == 1) // If this switch is on in the configuration
            {
                sum += modifiers.at(room).at(k); // toggle light k in room `room`.
            }
        }

        firstModifiers.at(i) = {i, sum};
    }

    sort(firstModifiers.begin(), firstModifiers.end(), [](const pair<uint32_t, int> &lhs, const pair<uint32_t, int> &rhs) {
        return lhs.second < rhs.second;
    });

    for (uint32_t i = 0; i < limit2; i++) // for every light switch configuration
    {
        int sum = 0;

        for (int k = 0; k < n; k++) // For every switch
        {
            if (((i >> k) & 1) == 1) // If this switch is on in the configuration
            {
                sum += modifiers.at(room).at(k + secondIndexOffset); // toggle light k + secondIndexOffset in room `room`.
            }
        }

        const int neededBrightneddFromFirst = targetBrightnesses.at(room) - currentBrightnesses.at(room) - sum;

        const pair<uint32_t, int> p = {0, neededBrightneddFromFirst};

        const auto lowerBound = lower_bound(firstModifiers.begin(), firstModifiers.end(), p, [](const pair<uint32_t, int> &lhs, const pair<uint32_t, int> &rhs) {
            return lhs.second < rhs.second;
        });

        const auto upperBound = upper_bound(firstModifiers.begin(), firstModifiers.end(), p, [](const pair<uint32_t, int> &lhs, const pair<uint32_t, int> &rhs) {
            return lhs.second < rhs.second;
        });

        for (auto it = lowerBound; it < upperBound; it++)
        {
            const uint32_t firstConfig = (*it).first;
            const uint32_t fullConfig = firstConfig | (i << secondIndexOffset);

            res.push_back(fullConfig);
        }
    }

    return res;
}

vector<uint32_t> configurationsThatIlluminateRoom(const vector<int> &targetBrightnesses, const vector<int> &currentBrightnesses, const vector<vector<int>> &modifiers, vector<uint32_t> validConfigs, int room, int n, int m)
{
    vector<uint32_t> res;

    for (const uint32_t &config : validConfigs) // for every light switch configuration
    {
        int sum = 0;

        for (int k = 0; k < n; k++) // For every switch
        {
            if (((config >> k) & 1) == 1) // If this switch is on in the configuration
            {
                sum += modifiers.at(room).at(k); // toggle light k in room `room`.
            }
        }

        if (targetBrightnesses.at(room) == currentBrightnesses.at(room) + sum)
        {
            res.push_back(config);
        }
    }

    return res;
}

static void runTestCase()
{
    int n;
    int m;
    cin >> n >> m;

    vector<int> targetBrightnesses(m);                // target brightnesses
    vector<int> currentBrightnesses(m);               // brightnesses in initial state
    vector<vector<int>> modifiers(m, vector<int>(n)); // brightness modifiers for each switch for each room from initial (untoggled) state

    for (int i = 0; i < m; i++)
    {
        int b;
        cin >> b;
        targetBrightnesses.at(i) = b;
    }

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            int on;
            cin >> on;
            int off;
            cin >> off;

            currentBrightnesses.at(j) += on;

            int modifier = -on + off; // toggling this switch turns off the lights that are initially on and turns on those that are initially off

            modifiers.at(j).at(i) = modifier;
        }
    }

    vector<uint32_t> configs = configurationsThatIlluminateRoom(targetBrightnesses, currentBrightnesses, modifiers, 0, n, m);
    int i = 1;
    while (i < m && !configs.empty())
    {
        configs = configurationsThatIlluminateRoom(targetBrightnesses, currentBrightnesses, modifiers, configs, i, n, m);
        i++;
    }

    if (i == m && !configs.empty())
    {
        int minSwitches = numeric_limits<int>::max();

        for (const auto &config : configs)
        {
            minSwitches = min(minSwitches, __builtin_popcount(config)); // __builtin_popcount: hamming weight
        }

        cout << minSwitches << "\n";
    }
    else
    {
        cout << "impossible\n";
    }
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
