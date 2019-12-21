//
//  beach_bars.cpp
//  Algorithms Lab
//
//  Created by Jonas Gessner
//  Copyright © 2019 Jonas Gessner. All rights reserved.
//

#include <iostream>
#include <vector>
#include <algorithm>
#include <set>

using namespace std;

static void runTestCase()
{
    int n;
    cin >> n;
    vector<int> parasols(n);

    for (int i = 0; i < n; i++)
    {
        int x;
        cin >> x;
        parasols[i] = x;
    }

    sort(parasols.begin(), parasols.end());

    int startLocation = parasols[0];
    int endLocation = parasols.back();

    int i = 0; // Points to first index in parasols that is within the target range.
    int j = 0; // Points to first index in parasols that is still too far from the target range.
    // i == j <=> None fit in target range

    int fits = 0;

    set<int> fitting;
    vector<int> opts;
    int optCount = 0;
    int optFurthestDistance = 101;

    for (int currentLocation = startLocation; currentLocation <= endLocation; currentLocation++) {
        while (i < j && currentLocation - parasols[i] > 100) {
            fitting.erase(parasols[i]);
            i++;
            fits--;
        }

        while (j < parasols.size() && parasols[j] - currentLocation <= 100) {
            fitting.insert(parasols[j]);
            fits++;
            j++;
        }

        if (fits >= optCount) {
            int furthestDistance = 0;
            for (const int &x : fitting) {
                furthestDistance = max(furthestDistance, abs(currentLocation - x));
            }

            if (fits > optCount || furthestDistance < optFurthestDistance) {
                optCount = fits;
                optFurthestDistance = furthestDistance;
                opts.clear();
                opts.push_back(currentLocation);
            }
            else if (fits == optCount && furthestDistance == optFurthestDistance) {
                opts.push_back(currentLocation);
            }
        }
    }

    cout << optCount << " " << optFurthestDistance << "\n";

    for (const int &x : opts) {
        cout << x << " ";
    }

    cout << "\n";
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
