//
//  hit.cpp
//  Algorithms Lab
//
//  Created by Jonas Gessner
//  Copyright © 2019 Jonas Gessner. All rights reserved.
//

#include <iostream>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel epec;
typedef CGAL::Exact_predicates_exact_constructions_kernel epic;

using namespace std;

static epec::Point_2 readPoint()
{
    long x;
    cin >> x;
    long y;
    cin >> y;

    return epec::Point_2(x, y);
}

static void runTestCase(int n)
{
    epec::Point_2 rayStart = readPoint();
    epec::Point_2 rayDirection = readPoint();

    epec::Ray_2 ray(rayStart, rayDirection);

    //vector<epec::Segment_2> lineSegments(n);

    bool flush = false;
    for (int i = 0; i < n; i++)
    {
        epec::Point_2 p1 = readPoint();
        epec::Point_2 p2 = readPoint();

        if (!flush)
        {
            epec::Segment_2 segment = epec::Segment_2(p1, p2);

            if (CGAL::do_intersect(ray, segment))
            {
                cout << "yes"
                     << "\n";
                flush = true;
            }
        }
        //lineSegments[i] =
    }

    if (!flush)
    {
        cout << "no"
             << "\n";
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
