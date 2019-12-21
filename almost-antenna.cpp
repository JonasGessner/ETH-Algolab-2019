//
//  almost-antenna.cpp
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
#include <array>

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

static K::FT evalCircle(const vector<K::Point_2> &points) {
    Min_circle circle(points.begin(), points.end(), true); // randomize order
    Traits::Circle c = circle.circle();
    K::FT radiusSquared = c.squared_radius();
    return radiusSquared;
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
    
    if (mc.number_of_support_points() > 3 || mc.number_of_support_points() == 0) {
        Traits::Circle c = mc.circle();
        
        K::FT radiusSquared = c.squared_radius();
        
        cout << fixed << setprecision(0) << ceil_to_double(CGAL::sqrt(radiusSquared)) << "\n";
        
        return;
    }
    
    points.erase(find(points.begin(), points.end(), mc.support_point(0)));
    
    K::FT smallestSquareRadius = evalCircle(points);
    
    if (mc.number_of_support_points() > 1) {
        points.erase(find(points.begin(), points.end(), mc.support_point(1)));
        points.push_back(mc.support_point(0));
        
        K::FT radiusSquared = evalCircle(points);
        if (radiusSquared < smallestSquareRadius) {
            smallestSquareRadius = radiusSquared;
        }
        
        if (mc.number_of_support_points() > 2) {
            points.erase(find(points.begin(), points.end(), mc.support_point(2)));
            points.push_back(mc.support_point(1));
            
            K::FT radiusSquared = evalCircle(points);
            if (radiusSquared < smallestSquareRadius) {
                smallestSquareRadius = radiusSquared;
            }
        }
    }
    
    auto smallestRadius = CGAL::sqrt(smallestSquareRadius);
    
    cout << fixed << setprecision(0) << ceil_to_double(smallestRadius) << "\n";
}

int main(int argc, const char *argv[])
{
    ios_base::sync_with_stdio(false);
    
    int n;
    cin >> n;
    
    while (n > 0)
    {
        runTestCase(n);
        if (cin.eof()) {
            break;
        }
        cin >> n;
    }
    
    return 0;
}
