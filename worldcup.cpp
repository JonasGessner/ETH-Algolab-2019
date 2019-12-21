//
//  worldcup.cpp
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

#include <CGAL/Gmpz.h>
#include <CGAL/QP_functions.h>
#include <CGAL/QP_models.h>

#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K> Triangulation;

// choose input type (input coefficients must fit)
typedef long IT;
// choose exact type for solver (CGAL::Gmpz or CGAL::Gmpq)
typedef CGAL::Gmpz ET;

// program and solution types
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

typedef K::Point_2 Point;
typedef K::Segment_2 Segment;
// typedef K::Circle_2 Circle;
typedef struct {
    Point p;
    K::FT rSq;
} Circle;

typedef struct {
    Point p;
    int supply;
    int alcoholContent;
} Warehouse;

typedef struct {
    Point p;
    int demand;
    int alcoholLimit;
} Stadium;

using namespace std;

double floor_to_double(const CGAL::Quotient<ET> &x) {
    double a = std::floor(CGAL::to_double(x));
    while (a > x)
        a -= 1;
    while (a + 1 <= x)
        a += 1;
    return a;
}

void testcase() {
    int n, m, c;
    cin >> n >> m >> c;

    vector<Point> triagPoints;
    triagPoints.reserve(n + m);

    vector<Warehouse> warehouses;
    warehouses.reserve(n);
    for (int i = 0; i < n; i++) { // warehouses
        int x, y, s, a;           // supply, alcohol content in percent
        cin >> x >> y >> s >> a;

        Warehouse w;
        w.alcoholContent = a;
        w.supply = s;
        w.p = Point(x, y);

        triagPoints.push_back(w.p);

        warehouses.push_back(w);
    }

    vector<Stadium> stadiums;
    stadiums.reserve(m);
    for (int i = 0; i < m; i++) { // stadiums
        int x, y, d, u;           // demand, upper absolute alcohol volume
        cin >> x >> y >> d >> u;

        Stadium s;
        s.alcoholLimit = u;
        s.demand = d;
        s.p = Point(x, y);

        triagPoints.push_back(s.p);

        stadiums.push_back(s);
    }

    Triangulation triang;
    triang.insert(triagPoints.begin(), triagPoints.end());

    vector<vector<int>> revenues(n, vector<int>(m, 0));

    for (int i = 0; i < n; i++) { // revenues
        for (int j = 0; j < m; j++) {
            int r;
            cin >> r;

            revenues.at(i).at(j) = r;
        }
    }

    vector<Circle> contours;
    for (int i = 0; i < c; i++) { // contour lines
        int x, y, r;
        cin >> x >> y >> r;
        const Point p = Point(x, y);

        const auto rSq = K::FT(r) * K::FT(r);

        const auto nearest = triang.nearest_vertex(p);

        if (CGAL::squared_distance(nearest->point(), p) > rSq) {
            continue;
        }

        contours.push_back({p, rSq});
    }

    Program lp(CGAL::SMALLER, true, 0, false, 0);

    const auto var = [&](const int i, const int j) {
        return m * i + j;
    };

    for (int i = 0; i < n; i++) { // revenues
        for (int j = 0; j < m; j++) {
            int t = 0;

            for (const Circle &circ : contours) {
                bool inc = false;
                const auto warehouse = warehouses.at(i);
                if (CGAL::squared_distance(warehouse.p, circ.p) <= circ.rSq) {
                    inc = true;
                }

                const auto stadium = stadiums.at(j);
                if (CGAL::squared_distance(stadium.p, circ.p) <= circ.rSq) {
                    inc = !inc;
                }

                if (inc) {
                    t++;
                }
            }

            int r = revenues.at(i).at(j);
            lp.set_c(var(i, j), -(100 * r - t));
        }
    }

    int constraint = 0;

    for (int i = 0; i < n; i++) { // Supply limit
        for (int j = 0; j < m; j++) {
            lp.set_a(var(i, j), constraint, 1);
        }

        lp.set_b(constraint, warehouses.at(i).supply);

        constraint++;
    }

    for (int j = 0; j < m; j++) { // Demand lower limit
        for (int i = 0; i < n; i++) {
            lp.set_a(var(i, j), constraint, 1);
        }

        lp.set_b(constraint, stadiums.at(j).demand);

        constraint++;
    }

    for (int j = 0; j < m; j++) { // Demand upper limit
        for (int i = 0; i < n; i++) {
            lp.set_a(var(i, j), constraint, -1);
        }

        lp.set_b(constraint, -stadiums.at(j).demand);

        constraint++;
    }

    for (int j = 0; j < m; j++) { // Alcohol limit
        for (int i = 0; i < n; i++) {
            lp.set_a(var(i, j), constraint, warehouses.at(i).alcoholContent);
        }

        lp.set_b(constraint, stadiums.at(j).alcoholLimit * 100);

        constraint++;
    }

    Solution s = CGAL::solve_nonnegative_linear_program(lp, ET());

    if (s.is_infeasible()) {
        cout << "RIOT!\n";
    } else {
        const auto v = s.objective_value();
        cout << (long)floor_to_double(-v / 100) << "\n";
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);

    int t;
    std::cin >> t;
    for (int i = 0; i < t; ++i)
        testcase();
}