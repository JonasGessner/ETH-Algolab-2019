//
//  even_matrices.cpp
//  Algorithms Lab
//
//  Created by Jonas Gessner
//  Copyright © 2019 Jonas Gessner. All rights reserved.
//

#include <iostream>
#include <vector>

using namespace std;

static void runTestCase()
{
    int n;
    cin >> n;
    vector<vector<bool>> rowPartialSums(n, vector<bool>(n)); // partial sums from zero along each row

    for (int i = 0; i < n; i++)
    {
        bool rowEven = true;
        for (int j = 0; j < n; j++)
        {
            bool x;
            cin >> x;

            rowEven = rowEven != x;

            rowPartialSums[i][j] = rowEven;
        }
    }

    int sum = 0;
    for (int i = 0; i < n; i++)
    {
        for (int j = i; j < n; j++)
        { // enumerate all column ranges
            bool even = true;
            int evens = 1;
            int odds = 0;
            for (int k = 0; k < n; k++)
            { // enumerate all rows. Build partial submatrix sums from the top row along each column range. Build these using the partial sums of each row.
                bool rowEven = rowPartialSums[k][j] == ((i > 0) ? rowPartialSums[k][i - 1] : true);
                even = even == rowEven;
                // we get an even submatrix along this column range by subtracting an even submatrix along this column range, starting from the top row from another even submtarix along this column range, starting from the top row (or odd and odd submatrices). (Like even pairs but with matrices Mx,y and Mx,z)
                // To count ALL even submatrices, we count all combinations of even, even and odd, odd submatrix combinations. This is essentially an iterative alg for ncr(oddCount, 2) + ncr(evenCount, 2) + oddCount.
                // When we encounter an even/odd submatrix that starts from zero, the number of additional even submatrices we can form is equal to the number of even/odd submatrices we found previous to finding the current one, since all pairs of the newly found matrix and the prevously found matrixes form a unique even submatrix.
                if (even)
                {
                    sum += evens;
                    evens++;
                }
                else
                {
                    sum += odds;
                    odds++;
                }
            }
        }
    }

    cout << sum << "\n";
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
