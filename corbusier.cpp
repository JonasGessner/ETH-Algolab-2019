//
//  corbusier.cpp
//  Algorithms Lab
//
//  Created by Jonas Gessner
//  Copyright © 2019 Jonas Gessner. All rights reserved.
//

#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <limits>
#include <string>
#include <unordered_set>
#include <vector>

using namespace std;

void testcase() {
    int n, ii, k;
    cin >> n >> ii >> k;

    bool found = false;
    vector<int> sumsFirst;
    sumsFirst.push_back(0);

    unordered_set<int> seen;

    for (int i = 0; i < n / 2; i++) {
        int disk;
        cin >> disk;

        if (found) {
            continue;
        }

        int sumsCount = sumsFirst.size();
        for (int j = 0; j < sumsCount; j++) {
            int newSum = (sumsFirst.at(j) + disk) % k;

            if (newSum == ii) {
                found = true;
                break;
            }

            if (seen.find(newSum) != seen.end()) {
                continue;
            }

            sumsFirst.push_back(newSum);
            seen.insert(newSum);
        }
    }

    sort(sumsFirst.begin(), sumsFirst.end());

    vector<int> sumsSecond;
    sumsSecond.push_back(0);

    unordered_set<int> seen2;

    for (int i = n / 2; i < n; i++) {
        int disk;
        cin >> disk;

        if (found) {
            continue;
        }

        int sumsCount = sumsSecond.size();
        for (int j = 0; j < sumsCount; j++) {
            int newSum = (sumsSecond.at(j) + disk) % k;

            if (newSum == ii) {
                found = true;
                break;
            }

            if (seen2.find(newSum) != seen2.end()) {
                continue;
            }

            int needed = (ii - newSum);
            if (needed < 0) {
                needed += k;
            }
            needed %= k;

            if (binary_search(sumsFirst.begin(), sumsFirst.end(), needed)) {
                found = true;
                break;
            }

            sumsSecond.push_back(newSum);
            seen2.insert(newSum);
        }
    }

    if (found) {
        cout << "yes\n";
        return;
    }

    cout << "no\n";
}

int main() {
    std::ios_base::sync_with_stdio(false);

    int t;
    std::cin >> t;
    for (int i = 0; i < t; ++i)
        testcase();
}
