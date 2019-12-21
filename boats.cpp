//
//  boats.cpp
//  Algorithms Lab
//
//  Created by Jonas Gessner
//  Copyright © 2019 Jonas Gessner. All rights reserved.
//

#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>

typedef struct
{
    int ringLocation;
    int length;

    int eft; // earliest finish time
    int est; // earliest start time
    int lft; // latest finish time
    int lst; // latest start time
} Boat;

Boat makeBoat(const int &ringLocation, const int &length)
{
    Boat b;
    b.ringLocation = ringLocation;
    b.length = length;

    b.est = ringLocation - length;
    b.lft = ringLocation + length;
    b.eft = ringLocation;
    b.lst = ringLocation;

    return b;
}

using namespace std;

static void runTestCase()
{
    int n;
    cin >> n;

    vector<Boat> boats(n);

    for (int i = 0; i < n; i++)
    {
        int length;
        cin >> length;
        int location;
        cin >> location;

        boats.at(i) = makeBoat(location, length);
    }

    sort(boats.begin(), boats.end(), [](const Boat &lhs, const Boat &rhs) {
        return lhs.eft < rhs.eft || (lhs.eft == rhs.eft && lhs.est > rhs.est);
    });

    int currentLocation = numeric_limits<int>::min();

    int boatCount = 0;

    for (int i = 0; i < n; i++)
    {
        const Boat &current = boats.at(i);

        // Latest start time of current boat is in the past
        if (current.lst < currentLocation)
        {
            continue;
        }

        // earliest start time of boat is not in the past. Boat can be scheduled at earliest possible time
        if (current.est >= currentLocation)
        {
            boatCount++;
            currentLocation = current.eft;
        }
        else
        {
            // Boat can not be scheduled at earliest possible time! Search the following boats up to and including thr first boat where its est is not in the past and pick the one with the earliest eft given the current time.

            int currentBestEFT = currentLocation + current.length;
            int currentBestEFTI = i;

            for (int j = i + 1; j < n; j++)
            {
                const Boat &next = boats.at(j);

                if (next.est >= currentLocation)
                {
                    if (next.eft < currentBestEFT)
                    {
                        currentBestEFT = min(currentBestEFT, next.eft);
                        currentBestEFTI = j;
                    }
                    break;
                }
                else if (next.lst >= currentLocation)
                {
                    int eftGivenCurrentLocation = max(currentLocation, next.est) + next.length;
                    if (eftGivenCurrentLocation < currentBestEFT)
                    {
                        currentBestEFT = eftGivenCurrentLocation;
                        currentBestEFTI = j;
                    }
                }
            }

            currentLocation = currentBestEFT;
            i = currentBestEFTI;
            boatCount++;
        }
    }

    cout << boatCount << "\n";
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
