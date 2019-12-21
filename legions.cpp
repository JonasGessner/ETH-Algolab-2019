//
//  legions.cpp
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
#include <vector>

#include <CGAL/Gmpz.h>
#include <CGAL/QP_functions.h>
#include <CGAL/QP_models.h>

// choose input type (input coefficients must fit)
typedef long IT;
// choose exact type for solver (CGAL::Gmpz or CGAL::Gmpq)
typedef CGAL::Gmpz ET;

// program and solution types
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

typedef struct {
    IT a;
    IT b;
    IT c;

    IT v;

    IT distanceDenominator;
} Legion;

Legion makeLegion(int a, int b, int c, int v) {
    Legion l;
    l.a = a;
    l.b = b;
    l.c = c;

    l.v = v;

    l.distanceDenominator = sqrt(l.a * l.a + l.b * l.b);

    return l;
}

typedef struct {
    IT x;
    IT y;
} Point;

// double floor_to_double(const ET &x) {
//     double a = std::floor(CGAL::to_double(x));
//     while (a > x)
//         a -= 1;
//     while (a + 1 <= x)
//         a += 1;
//     return a;
// }

// int signedLegionDist(const Legion &legion, const Point &point) {
//     return (point.x * legion.a + point.y * legion.b + legion.c) / legion.distanceDenominator;
// }

int legionSignature(const Legion &legion, const Point &point) {
    return (point.x * legion.a + point.y * legion.b + legion.c) >= 0 ? 1 : -1;
}

using namespace std;

void testcase() {
    int startX, startY, n;
    cin >> startX >> startY >> n;

    Point start;
    start.x = startX;
    start.y = startY;

    Program lp(CGAL::SMALLER, false, 0, false, 0);

    const int t = 0;
    const int x = 1;
    const int y = 2;

    lp.set_c0(0);
    lp.set_c(t, -1);

    for (int i = 0; i < n; i++) {
        int a, b, c, v;

        cin >> a >> b >> c >> v;

        Legion l = makeLegion(a, b, c, v);

        int sig = legionSignature(l, start);

        lp.set_a(t, i, l.distanceDenominator * v);
        lp.set_a(x, i, -a * sig);
        lp.set_a(y, i, -b * sig);
        lp.set_b(i, c * sig);
    }

    // t <= 0
    lp.set_a(t, n, -1);
    lp.set_b(n, 0);

    // solve the program, using ET as the exact type
    Solution s = CGAL::solve_linear_program(lp, ET());
    assert(s.solves_linear_program(lp));

    ET v = s.objective_value_numerator() / s.objective_value_denominator();

    // output solution
    cout << (long)std::floor(CGAL::to_double(-v)) << "\n";
}

int main() {
    std::ios_base::sync_with_stdio(false);

    int t;
    std::cin >> t;
    for (int i = 0; i < t; ++i)
        testcase();
}
