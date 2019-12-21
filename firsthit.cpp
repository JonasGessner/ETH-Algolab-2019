//
//  firsthit.cpp
//  Algorithms Lab
//
//  Created by Jonas Gessner
//  Copyright © 2019 Jonas Gessner. All rights reserved.
//

#include <iostream>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Min_circle_2.h>
#include <CGAL/Min_circle_2_traits_2.h>
#include <CGAL/Bbox_2.h>
#include <vector>
#include <algorithm>

typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef K::Point_2 Point;
typedef K::Segment_2 Segment;

typedef CGAL::Min_circle_2_traits_2<K> MinCircleTraits;
typedef CGAL::Min_circle_2<MinCircleTraits> MinCircle;

using namespace std;

static K::Point_2 readPoint()
{
    long x;
    cin >> x;
    long y;
    cin >> y;

    return K::Point_2(x, y);
}

double floor_to_double(const K::FT &x)
{
    double a = std::floor(CGAL::to_double(x));
    while (a > x)
        a -= 1;
    while (a + 1 <= x)
        a += 1;
    return a;
}

static void runTestCase(int n)
{
    K::Point_2 rayStart = readPoint();
    K::Point_2 rayDirection = readPoint();

    K::Ray_2 ray(rayStart, rayDirection);

    vector<K::Segment_2> segments(n);

    for (int i = 0; i < n; i++)
    {
        K::Point_2 p1 = readPoint();
        K::Point_2 p2 = readPoint();

        K::Segment_2 segment = K::Segment_2(p1, p2);
        segments[i] = segment;
    }

    random_shuffle(segments.begin(), segments.end()); // Input may be given in adversarial way

    bool hasAny = false;
    K::Segment_2 connection; // Segment from ray start to current closets point on the ray

    for (int i = 0; i < n; i++)
    {
        K::Segment_2 segment = segments[i];

        if (hasAny ? CGAL::do_intersect(connection, segment) : CGAL::do_intersect(ray, segment))
        {
            auto o = CGAL::intersection(ray, segment);
            if (const Point *op = boost::get<Point>(&*o))
            {
                connection = K::Segment_2(rayStart, *op);
                hasAny = true;
            }
            else if (const Segment *os = boost::get<Segment>(&*o))
            {
                connection = K::Segment_2(rayStart, os->start());

                if (CGAL::do_intersect(connection, os->end()))
                {
                    connection = K::Segment_2(rayStart, os->end());
                }
                hasAny = true;
            }
            else
            {
                cout << "wtf";
            }
        }
    }

    if (!hasAny)
    {
        cout << "no"
             << "\n";
    }
    else
    {
        cout << fixed << setprecision(0) << floor_to_double(connection.end().x()) << " " << floor_to_double(connection.end().y()) << "\n";
    }
}

int main(int argc, const char *argv[])
{
    ios_base::sync_with_stdio(false);

    int n;
    cin >> n;

    while (n > 0)
    {
        runTestCase(n);
        cin >> n;
    }

    return 0;
}
