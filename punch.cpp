//
//  punch.cpp
//  Algorithms Lab
//
//  Created by Jonas Gessner
//  Copyright © 2019 Jonas Gessner. All rights reserved.
//

#include <iostream>
#include <vector>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <limits>
#include <unordered_set>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;

using namespace std;

typedef struct
{
    int price;
    int volume;
} Beverage;

Beverage makeBeverage(const int &price, const int &volume)
{
    Beverage b;
    b.price = price;
    b.volume = volume;
    return b;
}

tuple<int, uint64_t, uint64_t, int> cheapestWayToGetToVolume(const int target, const vector<Beverage> &beverages, vector<tuple<int, uint64_t, uint64_t, int>> &memo)
{
    if (target <= 0)
    {
        return {0, 0, 0, 0};
    }

    auto &memoVal = memo.at(target);

    if (get<0>(memoVal) != -1)
    {
        return memoVal;
    }

    int bestCost = numeric_limits<int>::max();
    int bestCostMaxCount = 0;
    uint64_t bestCostSelection0 = 0;
    uint64_t bestCostSelection1 = 0;

    for (int i = 0; i < beverages.size(); i++)
    {
        const Beverage &beverage = beverages.at(i);

        if (beverage.price > bestCost)
        {
            continue;
        }

        auto sub = cheapestWayToGetToVolume(target - beverage.volume, beverages, memo);
        int cost = beverage.price + get<0>(sub);

        uint64_t select1 = get<1>(sub);
        uint64_t select0 = get<2>(sub);
        int count = get<3>(sub);

        if (i > 63)
        {
            if (((select1 >> (i - 64)) & 1) == 0)
            {
                select1 |= 1ULL << (i - 64);
                count++;
            }
        }
        else
        {
            if (((select0 >> i) & 1) == 0)
            {
                select0 |= 1ULL << i;
                count++;
            }
        }

        if (cost < bestCost || (cost == bestCost && count > bestCostMaxCount))
        {
            bestCost = cost;
            bestCostMaxCount = count;
            bestCostSelection0 = select0;
            bestCostSelection1 = select1;
        }
    }

    const tuple<int, uint64_t, uint64_t, int> res = {bestCost, bestCostSelection1, bestCostSelection0, bestCostMaxCount};

    memo.at(target) = res;

    return res;
}

static void runTestCase()
{
    int n;
    int k;
    cin >> n >> k;

    vector<Beverage> beverages(n);

    for (int i = 0; i < n; i++)
    {
        int price;
        int volume;
        cin >> price >> volume;

        beverages.at(i) = makeBeverage(price, volume);
    }

    vector<tuple<int, uint64_t, uint64_t, int>> memo(k + 1, {-1, 0, 0, 0});

    auto cheapest = cheapestWayToGetToVolume(k, beverages, memo);

    cout << get<0>(cheapest) << " "
         << get<3>(cheapest) << "\n";
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
