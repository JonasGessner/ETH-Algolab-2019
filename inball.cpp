//
//  inball.cpp
//  Algorithms Lab
//
//  Created by Jonas Gessner
//  Copyright © 2019 Jonas Gessner. All rights reserved.
//

#include <iostream>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>
#include <CGAL/Sqrt_extension.h>

// choose input type (input coefficients must fit)
typedef CGAL::Gmpq IT;
// choose exact type for solver (CGAL::Gmpz or CGAL::Gmpq)
typedef CGAL::Gmpq ET;

// program and solution types
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

using namespace std;

double floor_to_double(const ET &x)
{
    double a = std::floor(CGAL::to_double(x));
    while (a > x)
        a -= 1;
    while (a + 1 <= x)
        a += 1;
    return a;
}

static void runTestCase()
{
    int n, d;
    cin >> n;
    if (n == 0)
    {
        exit(0);
    }
    cin >> d;

    Program lp(CGAL::SMALLER, false, 0, false, 0);

    const int R = d;

    vector<IT> inputs(d);
    for (int i = 0; i < n; i++)
    {
        CGAL::Gmpz normCalc = 0;

        for (int j = 0; j < d; j++)
        {
            int val;
            cin >> val;
            normCalc += CGAL::Gmpz(val) * CGAL::Gmpz(val);
            inputs.at(j) = IT(val);
        }

        CGAL::Gmpz norm = CGAL::sqrt(normCalc);

        for (int j = 0; j < d; j++)
        {
            lp.set_a(j, i, inputs.at(j) / IT(norm));
        }

        // a_i is the normal of the hyperplane. The distance from a point P to the hyperplane is -(a_i^T x (dot product) - b_i) / norm_2(a_i) (under the assumtion that b_i is larger), a non linear term
        // If we normalize a_i and scale b_i accordingly then norm_2(a_i) becomes 1 and the term becomes linear. So we calculate the norm, scale the factors accordingly, and then the distance from x to the hypersphere is simply -(a_i'^T x (dot product) - b_i').

        lp.set_a(R, i, 1);

        int b;
        cin >> b;
        lp.set_b(i, IT(b) / IT(norm)); // Careful about type mixing! Better to first cast before doing something like division!
    }

    // set r >= 0
    lp.set_a(R, n, -1);
    lp.set_b(n, 0);

    lp.set_c(R, -1);

    Solution s = CGAL::solve_linear_program(lp, ET());
    assert(s.solves_linear_program(lp));

    if (s.is_infeasible())
    {
        cout << "none\n";
    }
    else if (s.is_unbounded())
    {
        cout << "inf\n";
    }
    else
    {
        auto opt = -((s.objective_value_numerator() / s.objective_value_denominator()));
        cout
            << floor_to_double(opt) << "\n";
    }
}

int main(int argc, const char *argv[])
{
    ios_base::sync_with_stdio(false);

    while (true)
    {
        runTestCase();
    }

    return 0;
}
