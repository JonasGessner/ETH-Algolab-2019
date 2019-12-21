//
//  diet.cpp
//  Algorithms Lab
//
//  Created by Jonas Gessner
//  Copyright © 2019 Jonas Gessner. All rights reserved.
//

#include <iostream>

#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

// choose input type (input coefficients must fit)
typedef int IT;
// choose exact type for solver (CGAL::Gmpz or CGAL::Gmpq)
typedef CGAL::Gmpq ET;

// program and solution types
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

using namespace std;

static void runTestCase()
{
    int n, m;
    cin >> n >> m;

    if (n == m && n == 0)
    {
        exit(0);
    }

    Program lp(CGAL::SMALLER, true, 0, false, 0);

    // m variables
    // 2 * n constraints
    // objective: minimize x_0 * c_0 + ... + x_m-1 * c_m-1 (prices of foods * amounts of foods used)

    // constraints:
    // For each ingredient i:
    // x_0 * a_0_i + ... + x_m-1 * a_m-1_i <= min_i / >= max_i
    // constrain the amount of the ingredients to the max and min. The amount of ingredients is the amount of ingredients in each food * the amount of each food used

    for (int i = 0; i < n; i++)
    {
        int min, max;
        cin >> min >> max;

        lp.set_b(2 * i, max);

        // x_i >= min
        // <=> -x_i <= -min
        lp.set_b((2 * i) + 1, -min);
    }

    for (int i = 0; i < m; i++)
    {
        int price;
        cin >> price;

        lp.set_c(i, price);

        for (int j = 0; j < n; j++)
        {
            int amount;
            cin >> amount;

            lp.set_a(i, (2 * j), amount);
            lp.set_a(i, (2 * j) + 1, -amount);
        }
    }

    Solution s = CGAL::solve_linear_program(lp, ET());

    if (s.is_infeasible())
    {
        cout << "No such diet.\n";
    }
    else
    {
        cout << floor((s.objective_value_numerator() / s.objective_value_denominator()).to_double()) << "\n";
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
