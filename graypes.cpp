//
//  graypes.cpp
//  Algorithms Lab
//
//  Created by Jonas Gessner
//  Copyright © 2019 Jonas Gessner. All rights reserved.
//

#include <iostream>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel_with_sqrt.h>
#include <CGAL/Delaunay_triangulation_2.h>

typedef CGAL::Exact_predicates_exact_constructions_kernel_with_sqrt Ksqrt;
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K> Triangulation;
typedef Triangulation::Edge_iterator Edge_iterator;

using namespace std;

static void runTestCase()
{
    int n;
    cin >> n;

    if (n == 0)
    {
        exit(0);
    }

    vector<K::Point_2> points(n);
    for (int i = 0; i < n; i++)
    {
        int x, y;
        cin >> x >> y;

        points.at(i) = K::Point_2(x, y);
    }

    Triangulation t;
    t.insert(points.begin(), points.end());

    K::FT shortestEdge = -1;

    // Find shortest edge.
    for (Edge_iterator e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e)
    {
        const auto edge = t.segment(e);
        const K::FT length = edge.squared_length();

        if (shortestEdge == -1 || length < shortestEdge)
        {
            shortestEdge = length;
        }
    }

    Ksqrt::FT len = CGAL::sqrt(Ksqrt::FT(shortestEdge));

    cout << (int)ceil(CGAL::to_double(len * 50)) << "\n";
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
