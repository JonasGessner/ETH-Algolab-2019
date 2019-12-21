//
//  planks.cpp
//  Algorithms Lab
//
//  Created by Jonas Gessner
//  Copyright © 2019 Jonas Gessner. All rights reserved.
//

#include <iostream>
#include <vector>
#include <algorithm>
#include <boost/math/special_functions/binomial.hpp>
#include <unordered_set>
#include <set>

using namespace std;

static void runTestCase()
{
    int n;
    cin >> n;

    vector<int> lengths(n);

    long totalLength = 0;

    int longestLength = 0;

    for (int i = 0; i < n; i++)
    {
        int l;
        cin >> l;

        longestLength = max(longestLength, l);
        totalLength += l;

        lengths.at(i) = l;
    }

    // sort(lengths.begin(), lengths.end(), greater<int>());

    long sideLength = totalLength / 4;

    if (longestLength > sideLength)
    {
        cout << "0\n";
        return;
    }

    uint32_t nMask = (1 << n) - 1;

    unordered_set<uint32_t> halfSets;
    unordered_set<uint32_t> quaterSets;

    for (uint32_t i = 0; i < (1 << n); i++)
    {
        if (halfSets.find((~i) & nMask) != halfSets.end())
        {
            continue;
        }

        long sum = 0;
        int count = 0;
        for (int j = 0; j < n; j++)
        {
            if ((i >> j) & 1)
            {
                count++;
                sum += lengths.at(j);
                if (sum > sideLength * 2)
                {
                    break;
                }
            }
        }

        if (sum == sideLength)
        {
            quaterSets.insert(i);
        }

        if (count > 1 && sum == sideLength * 2)
        {
            halfSets.insert(i);
        }
    }

    long count = 0;

    for (const uint32_t &halfSet : halfSets)
    {
        int leftQuaterPairs = 0;
        int rightQuaterPairs = 0;

        for (const uint32_t &quater : quaterSets)
        {
            if ((quater & halfSet) == quater)
            {
                leftQuaterPairs++;
            }
            else if ((quater & (~halfSet)) == quater)
            {
                rightQuaterPairs++;
            }
        }

        count += (leftQuaterPairs / 2) * (rightQuaterPairs / 2);
    }

    cout << count / 3 << "\n";
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
