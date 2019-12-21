//
//  antenna.cpp
//  Algorithms Lab
//
//  Created by Jonas Gessner
//  Copyright © 2019 Jonas Gessner. All rights reserved.
//

#include <iostream>
#include <CGAL/Exact_predicates_exact_constructions_kernel_with_sqrt.h>
#include <CGAL/Min_circle_2.h>
#include <CGAL/Min_circle_2_traits_2.h>
#include <vector>

typedef CGAL::Exact_predicates_exact_constructions_kernel_with_sqrt K;
typedef CGAL::Min_circle_2_traits_2<K> Traits;
typedef CGAL::Min_circle_2<Traits> Min_circle;

using namespace std;

static K::Point_2 readPoint()
{
    long x;
    cin >> x;
    long y;
    cin >> y;

    return K::Point_2(x, y);
}

double ceil_to_double(const K::FT &x)
{
    double a = std::floor(CGAL::to_double(x));
    while (a < x)
        a += 1;
    while (a - 1 >= x)
        a -= 1;
    return a;
}

static void runTestCase(int n)
{
    vector<K::Point_2> points(n);

    for (int i = 0; i < n; i++)
    {
        K::Point_2 point = readPoint();
        points[i] = point;
    }

    Min_circle mc(points.begin(), points.end(), true); // randomize order

    Traits::Circle c = mc.circle();

    auto radius = CGAL::sqrt(c.squared_radius());

    cout << fixed << setprecision(0) << ceil_to_double(radius) << "\n";
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
