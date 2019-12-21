//
//  bistro.cpp
//  Algorithms Lab
//
//  Created by Jonas Gessner
//  Copyright © 2019 Jonas Gessner. All rights reserved.
//

#include <iostream>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K> Triangulation;
typedef Triangulation::Finite_faces_iterator Face_iterator;
typedef Triangulation::Vertex_handle Vertex_handle;

using namespace std;

bool runTestCase()
{
    int n;
    cin >> n;

    if (n == 0)
    {
        return false;
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

    int m;
    cin >> m;

    for (int i = 0; i < m; i++)
    {
        int x, y;
        cin >> x >> y;

        const auto point = K::Point_2(x, y);

        const K::Point_2 nearest = (*t.nearest_vertex(point)).point();

        cout << (long long)CGAL::to_double(CGAL::squared_distance(nearest, point)) << "\n";
    }

    return true;
}

int main(int argc, const char *argv[])
{
    ios_base::sync_with_stdio(false);

    while (runTestCase())
    {
    }

    return 0;
}
