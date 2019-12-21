//
//  search_snippets.cpp
//  Algorithms Lab
//
//  Created by Jonas Gessner
//  Copyright © 2019 Jonas Gessner. All rights reserved.
//

#include <iostream>
#include <vector>
#include <map>
#include <unordered_map>
#include <climits>

using namespace std;

static void runTestCase()
{
    int n;
    cin >> n;

    if (n == 0)
    {
        cout << "0\n";
        return;
    }
    else if (n == 1)
    {
        cout << "1\n";
        return;
    }

    vector<int> frequencies(n);

    int totalNumberOfQueryWordOcurrencies = 0;
    for (int i = 0; i < n; i++)
    {
        int x;
        cin >> x;
        frequencies[i] = x;
        totalNumberOfQueryWordOcurrencies += x;
    }

    map<int, int, less<int>> wordAtPosition;

    for (int i = 0; i < n; i++)
    {
        int f = frequencies[i];
        for (int j = 0; j < f; j++)
        {
            int pos;
            cin >> pos;
            wordAtPosition[pos] = i;
        }
    }

    int distinctWords = 1;
    vector<int> frequenciesInWindow(n, 0);

    auto iIterator = wordAtPosition.begin();
    auto jIterator = wordAtPosition.begin();

    frequenciesInWindow[iIterator->second] = 1;

    int smallestInterval = INT_MAX;

    int j = 0;

    while ((distinctWords < n && j < (totalNumberOfQueryWordOcurrencies - 1)) || (distinctWords >= n))
    {
        if (distinctWords < n && j < (totalNumberOfQueryWordOcurrencies - 1))
        {
            j++;
            jIterator++;
            int nextWord = jIterator->second;
            int oldFrequency = frequenciesInWindow[nextWord];
            if (oldFrequency == 0)
            {
                distinctWords++;
            }
            frequenciesInWindow[nextWord] = (oldFrequency + 1);
        }

        if (distinctWords >= n)
        {
            if (distinctWords == n)
            {
                smallestInterval = min(jIterator->first - iIterator->first + 1, smallestInterval);
            }

            int deletedWord = iIterator->second;
            int oldFrequency = frequenciesInWindow[deletedWord];
            if (oldFrequency == 1)
            {
                distinctWords--;
            }
            frequenciesInWindow[deletedWord] = (oldFrequency - 1);
            iIterator++;
        }
    }

    cout << smallestInterval << "\n";
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
