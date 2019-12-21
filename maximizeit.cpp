//
//  maximizeit.cpp
//  Algorithms Lab
//
//  Created by Jonas Gessner
//  Copyright © 2019 Jonas Gessner. All rights reserved.
//

#include <iostream>

#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

using namespace std;

// choose input type (input coefficients must fit)
typedef int IT;
// choose exact type for solver (CGAL::Gmpz or CGAL::Gmpq)
typedef CGAL::Gmpq ET;

// program and solution types
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

void printResult(const Solution &s, bool maximizing)
{
    if (s.is_unbounded())
    {
        cout << "unbounded";
    }
    else if (s.is_infeasible())
    {
        cout << "no";
    }
    else
    {
        auto opt = s.objective_value_numerator() / s.objective_value_denominator();

        if (!maximizing)
        {
            cout << ceil(opt.to_double());
        }
        else
        {
            cout << floor(-opt.to_double());
        }
    }

    cout << "\n";
}

void lpA(int a, int b)
{
    // create an LP with Ax <= b, lower bound 0 and no upper bounds
    Program lp(CGAL::SMALLER, true, 0, false, 0);

    // set the coefficients of A and b
    const int X = 0;
    const int Y = 1;

    lp.set_a(X, 0, 1);
    lp.set_a(Y, 0, 1);
    lp.set_b(0, 4); //  x + y  <= 4

    lp.set_a(X, 1, 4);
    lp.set_a(Y, 1, 2);
    lp.set_b(1, a * b); // 4x + 2y <= a * b

    lp.set_a(X, 2, -1);
    lp.set_a(Y, 2, 1);
    lp.set_b(2, 1); // -x + y <= 1

    // objective function
    lp.set_c(X, a);  // -a * x
    lp.set_c(Y, -b); // b * y
    lp.set_c0(0);

    // solve the program, using ET as the exact type
    Solution s = CGAL::solve_linear_program(lp, ET());
    assert(s.solves_linear_program(lp));

    printResult(s, true);
}

void lpB(int a, int b)
{
    Program lp(CGAL::LARGER, false, 0, true, 0);

    // set the coefficients of A and b
    const int X = 0;
    const int Y = 1;
    const int Z = 2;

    lp.set_a(X, 0, 1);
    lp.set_a(Y, 0, 1);
    lp.set_b(0, -4); //  -x - y  <= 4

    lp.set_a(X, 1, 4);
    lp.set_a(Y, 1, 2);
    lp.set_a(Z, 1, 1);
    lp.set_b(1, -a * b); // 4x + 2y <= a * b

    lp.set_a(X, 2, -1);
    lp.set_a(Y, 2, 1);
    lp.set_b(2, -1); // -x + y <= 1

    // objective function
    lp.set_c(X, a); // a * x
    lp.set_c(Y, b); // b * y
    lp.set_c(Z, 1); // z
    lp.set_c0(0);

    // solve the program, using ET as the exact type
    Solution s = CGAL::solve_linear_program(lp, ET());
    assert(s.solves_linear_program(lp));

    printResult(s, false);
}

static void runTestCase()
{
    int p, a, b;
    cin >> p;

    if (p == 0)
    {
        exit(0);
    }

    cin >> a;
    cin >> b;

    if (p == 1)
    {
        lpA(a, b);
    }
    else
    {
        lpB(a, b);
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
