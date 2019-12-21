//
//  deck_of_cards.cpp
//  Algorithms Lab
//
//  Created by Jonas Gessner
//  Copyright © 2019 Jonas Gessner. All rights reserved.
//

#include <iostream>
#include <vector>
#include <climits>

int cards[100000];

using namespace std;

static void runTestCase()
{
    int n;
    cin >> n;
    int k;
    cin >> k;

    for (int i = 0; i < n; i++)
    {
        int x;
        cin >> x;
        cards[i] = x;
    }

    int i = 0;
    int j = 0;
    int currentSum = cards[i];
    int currentDistance = k - currentSum;

    int closestAbsoluteDistance = INT_MAX;

    int bestI = 0;
    int bestJ = 0;

    while (j < (n - 1) && closestAbsoluteDistance > 0 && currentDistance != 0)
    {
        bool brk = false;
        while (j < (n - 1) && currentDistance > 0) // Diff is positive so we can add more elements
        {
            j++;
            int addVal = cards[j];
            if (addVal > k && addVal - k > closestAbsoluteDistance)
            {
                j++;
                i = j;

                currentSum = cards[i];
                currentDistance = k - currentSum;

                if (abs(currentDistance) < closestAbsoluteDistance)
                {
                    closestAbsoluteDistance = abs(currentDistance);
                    bestI = i;
                    bestJ = j;
                }
                brk = true;
                break;
            }
            currentSum += addVal;
            currentDistance -= addVal;

            if (abs(currentDistance) < closestAbsoluteDistance)
            {
                closestAbsoluteDistance = abs(currentDistance);
                bestI = i;
                bestJ = j;
            }
        }

        if (brk)
        {
            continue;
        }

        while (currentDistance < 0 && i < j) // Diff is negative so we overshot. Remove elements from the left
        {
            int subVal = cards[i];
            currentSum -= subVal;
            currentDistance += subVal;
            i++;

            if (abs(currentDistance) < closestAbsoluteDistance)
            {
                closestAbsoluteDistance = abs(currentDistance);
                bestI = i;
                bestJ = j;
            }
        }

        if (currentDistance < 0 && i == j)
        {
            i++;
            j++;

            currentSum = cards[i];
            currentDistance = k - currentSum;

            if (abs(currentDistance) < closestAbsoluteDistance)
            {
                closestAbsoluteDistance = abs(currentDistance);
                bestI = i;
                bestJ = j;
            }
        }
    }
    cout << bestI << " " << bestJ << "\n";
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
