//
//  dominoes.cpp
//  Algorithms Lab
//
//  Created by Jonas Gessner
//  Copyright © 2019 Jonas Gessner. All rights reserved.
//

#include <iostream>
#include <vector>

using namespace std;

static void runTestCase() {
    int n; cin >> n;

    vector<int> dominos;
    dominos.reserve(n);

    for (int i = 0; i < n; i++) {
        int x; cin >> x;
        dominos.push_back(x);
    }

    int i = 0;
    while (i < n) {
        int h = dominos[i];

        int thisDomino = i;

        int bestNextPosition = thisDomino;
        int bestNextDistanceOverI = h;

        i++;

        for (int j = 1; j < h && i < n; j++, i++) {
            int thisH = dominos[i];
            bestNextDistanceOverI--;

            if (thisH > bestNextDistanceOverI) {
                bestNextPosition = i;
                bestNextDistanceOverI = thisH;
            }
        }

        if (thisDomino == bestNextPosition) {
            break;
        }

        i = bestNextPosition;
    }

    cout << i << "\n";
}

int main(int argc, const char * argv[]) {
    ios_base::sync_with_stdio(false);
    
    int t; cin >> t;
    
    for (int i = 0; i < t; i++) {
        runTestCase();
    }
    
    return 0;
}
