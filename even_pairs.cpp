//
//  even_pairs.cpp
//  Algorithms Lab
//
//  Created by Jonas Gessner
//  Copyright © 2019 Jonas Gessner. All rights reserved.
//

#include <iostream>
#include <vector>

using namespace std;

static void runTestCase() {
    int n;
    cin >> n;
    vector<int> numbers;
    numbers.reserve(n);

    for (int i = 0; i < n; i++) {
        int x;
        cin >> x;
        numbers.push_back(x);
    }

    int evenCount = 0;
    int oddCount = 0;

    int count = 0;

    bool even = true;

    for (int i = 0; i < n; i++) {
        bool thisEven = (numbers[i] % 2) == 0;
        even = even == thisEven;
        // For every even partial sum, we cam create 1 even contigious subsequence from it, plus an additional number equal to the number of even paertial sums we already encountered (since the differences with will be even).
        if (even) {
            evenCount++;
            count += evenCount;
        } else {
            // For every odd partial sum, we cam create he number of odd paertial sums we already encountered (since the differences with will be odd).
            count += oddCount;
            oddCount++;
        }
    }

    cout << count << "\n";
}

int main(int argc, const char *argv[]) {
    ios_base::sync_with_stdio(false);

    int t;
    cin >> t;

    for (int i = 0; i < t; i++) {
        runTestCase();
    }

    return 0;
}
