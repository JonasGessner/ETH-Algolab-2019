//
//  radiation.cpp
//  Algorithms Lab
//
//  Created by Jonas Gessner
//  Copyright © 2019 Jonas Gessner. All rights reserved.
//

#include <iostream>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpq.h>
#include <CGAL/Gmpz.h>

using namespace std;

// choose input type (input coefficients must fit)
typedef CGAL::Gmpz IT;
// choose exact type for solver (CGAL::Gmpz or CGAL::Gmpq)
typedef CGAL::Gmpz ET;

// program and solution types
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

using namespace std;

typedef struct
{
    int x;
    int y;
    int z;
} Point;

inline Point makePoint(const int x, const int y, const int z)
{
    Point p;
    p.x = x;
    p.y = y;
    p.z = z;

    return p;
}

inline bool canFit(Program *reuse, int reuseDegree, int reuseTermCount, const vector<Point> &negativePoints, const vector<Point> &positivePoints, int d, Program *out, int *termCountOut, int *dimensionOut)
{
    Program lp;
    int termCount;

    bool isReusing = reuse != NULL && reuseDegree <= d;

    if (isReusing)
    {
        lp = *reuse;
        termCount = reuseTermCount;
    }
    else
    {
        lp = Program(CGAL::SMALLER, false, 0, false, 0);

        const int error = 0;

        int constraint = 0;
        for (const Point &p : negativePoints)
        {
            lp.set_a(error, constraint, 1); // fitting error

            lp.set_b(constraint, 0);
            constraint++;
        }

        for (const Point &p : positivePoints)
        {
            lp.set_a(error, constraint, -1); // fitting error

            lp.set_b(constraint, 0);
            lp.set_r(constraint, CGAL::LARGER);
            constraint++;
        }

        lp.set_c(error, -1); // Maximize the minimum fitting error! This must be greater than zero
        lp.set_l(error, true, 0.0);
        lp.set_u(error, true, 1.0);

        // variable 0 is the fitting error
        // therefore offset variables by 1
        termCount = 1;
    }

    // Generate all terms of degree <= d
    for (int x = 0; x <= d; x++)
    {
        for (int y = d - x; y >= 0; y--)
        {
            for (int z = d - x - y; z >= 0; z--)
            {
                if (!isReusing || d > reuseDegree) // If we are reusing an LP only add terms of higher degree than the degree of the reused LP
                {
                    // Add variables and constraints
                    int constraint = 0;
                    for (const Point &p : negativePoints)
                    {
                        double factor = pow(p.x, x) * pow(p.y, y) * pow(p.z, z);

                        lp.set_a(termCount, constraint, factor);

                        constraint++;
                    }

                    for (const Point &p : positivePoints)
                    {
                        double factor = pow(p.x, x) * pow(p.y, y) * pow(p.z, z);

                        lp.set_a(termCount, constraint, factor);

                        constraint++;
                    }

                    termCount++;
                }
            }
        }
    }

    if (out != NULL)
    {
        *out = lp;
    }
    if (termCountOut != NULL)
    {
        *termCountOut = termCount;
    }
    if (dimensionOut != NULL)
    {
        *dimensionOut = d;
    }

    CGAL::Quadratic_program_options options;
    options.set_pricing_strategy(CGAL::QP_BLAND);
    Solution s = CGAL::solve_linear_program(lp, ET(), options);

    return !s.is_infeasible() && (s.is_unbounded() || s.objective_value_numerator() != 0);
}

static void runTestCase()
{
    int h, t;
    cin >> h >> t;

    vector<Point> healthyPoints(h);

    for (int i = 0; i < h; i++)
    {
        int x, y, z;
        cin >> x >> y >> z;

        healthyPoints.at(i) = makePoint(x, y, z);
    }

    vector<Point> tumorPoints(t);

    for (int i = 0; i < t; i++)
    {
        int x, y, z;
        cin >> x >> y >> z;

        tumorPoints.at(i) = makePoint(x, y, z);
    }

    // Do an exponential search and reuse the LP.

    int maxD = 31;
    int bound = 0;
    Program *reuseProgram = NULL;
    int reuseTermCount = 0;
    int reuseDimension = 0;

    while (bound < maxD && !canFit(reuseProgram, reuseDimension, reuseTermCount, healthyPoints, tumorPoints, bound, reuseProgram, &reuseTermCount, &reuseDimension))
    {
        if (bound == 0)
        {
            bound = 1;
        }
        else
        {
            bound *= 2;
        }
    }

    // Do a binary search (this is just a part of the exponential search algorithm) and reuse the LP if possible.

    int l = bound / 2;
    int r = min(bound + 1, maxD);

    while (l < r)
    {
        int mid = (l + r) / 2;

        if (canFit(reuseProgram, reuseDimension, reuseTermCount, healthyPoints, tumorPoints, mid, reuseProgram, &reuseTermCount, &reuseDimension))
        {
            r = mid;
        }
        else
        {
            l = mid + 1;
        }
    }

    if (l <= 30)
    {
        cout << l << "\n";
    }
    else
    {
        cout << "Impossible!\n";
    }
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
