//
//  germs.cpp
//  Algorithms Lab
//
//  Created by Jonas Gessner
//  Copyright © 2019 Jonas Gessner. All rights reserved.
//

#include <iostream>
#include <algorithm>

#include <CGAL/Exact_predicates_exact_constructions_kernel_with_sqrt.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

typedef CGAL::Exact_predicates_exact_constructions_kernel_with_sqrt Ksqrt;
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K> Triangulation;
typedef Triangulation::Edge_iterator Edge_iterator;

using namespace std;

double ceil_to_double(const Ksqrt::FT &x)
{
    double a = std::floor(CGAL::to_double(x));
    while (a < x)
        a += 1;
    while (a - 1 >= x)
        a -= 1;
    return a;
}

bool runTestCase()
{
    int n;
    cin >> n;

    if (n == 0)
    {
        return false;
    }

    int l, b, r, tt;
    cin >> l;
    cin >> b;
    cin >> r;
    cin >> tt;

    vector<K::Point_2> points(n);
    for (int i = 0; i < n; i++)
    {
        int x, y;
        cin >> x >> y;

        points.at(i) = K::Point_2(x, y);
    }

    Triangulation t;
    t.insert(points.begin(), points.end());

    vector<K::FT> dieMoments;
    dieMoments.reserve(n);

    for (auto v = t.finite_vertices_begin(); v != t.finite_vertices_end(); v++)
    {
        const auto vertex = v->point(); // For each vertex (bacteria)

        // This is the closest edge
        K::FT closestEdge = K::FT(min(min(vertex.x() - l, r - vertex.x()), min(vertex.y() - b, tt - vertex.y()))) * 2;

        // Keep track of the first object (edge or other bacteria) that is touched
        K::FT closest = closestEdge * closestEdge;

        Triangulation::Edge_circulator c = t.incident_edges(v);

        if (!c.is_empty())
        {
            do // Find the closest neighbouring vertex. The closest neighbouring vertex will cause this vertex to die. The neighbouring vertex might have already died when it collides with this one so can't conclude anything on the other vertex
            {
                const int edgeIndex = c->second;
                const auto face = c->first;

                const auto p1Vertex = face->vertex((edgeIndex + 1) % 3);
                const auto p2Vertex = face->vertex((edgeIndex + 2) % 3);

                if (p1Vertex == t.infinite_vertex() || p2Vertex == t.infinite_vertex() || p1Vertex == p2Vertex)
                {
                    continue;
                }

                const auto p1 = p1Vertex->point();
                const auto p2 = p2Vertex->point();

                const K::FT distance = CGAL::squared_distance(p1, p2);

                if (distance < closest && distance >= 1)
                {
                    closest = distance;
                }
            } while (++c != t.incident_edges(v));
        }

        dieMoments.push_back(closest);
    }

    sort(dieMoments.begin(), dieMoments.end());

    auto output = [](const K::FT &val) {
        Ksqrt::FT timeUntilCollision = CGAL::sqrt(CGAL::sqrt(Ksqrt::FT(val)) / 2 - 0.5);
        double time = ceil_to_double(timeUntilCollision);

        cout << setprecision(0) << fixed << time;
    };

    int afterMidIndex = n / 2;

    output(dieMoments.at(0));
    cout << " ";
    output(dieMoments.at(afterMidIndex));
    cout << " ";
    output(dieMoments.at(n - 1));
    cout << "\n";

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
