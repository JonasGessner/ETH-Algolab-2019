//
//  build_the_sum.cpp
//  Algorithms Lab
//
//  Created by Jonas Gessner
//  Copyright © 2019 Jonas Gessner. All rights reserved.
//

#include <iostream>

using namespace std;

static void runTestCase() {
    int n; cin >> n;
    int sum = 0;
    for (int i = 0; i < n; i++) {
        int x; cin >> x;
        sum += x;
    }

    cout << sum << "\n";
}

int main(int argc, const char * argv[]) {
    ios_base::sync_with_stdio(false);
    
    int t; cin >> t;
    
    for (int i = 0; i < t; i++) {
        runTestCase();
    }
    
    return 0;
}
