//
//  suez.cpp
//  Algorithms Lab
//
//  Created by Jonas Gessner
//  Copyright © 2019 Jonas Gessner. All rights reserved.
//

#include <iostream>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>
#include <algorithm>

// choose input type (input coefficients must fit)
typedef int IT;
// choose exact type for solver (CGAL::Gmpz or CGAL::Gmpq)
typedef CGAL::Gmpq ET;

// program and solution types
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

struct Point
{
    int x;
    int y;
    int index;
    Point(const int _x, const int _y, const int _index) : x(_x), y(_y), index(_index){};
    
public:
    bool pointIsBoundedInXByPoint(const Point &other, const CGAL::Gmpq &whRatio) const
    {
        int xDelta = abs(other.x - x);
        int yDelta = abs(other.y - y);
        
        if (yDelta == 0)
        {
            return true;
        }
        
        CGAL::Gmpq whRatioDiff = CGAL::Gmpq(xDelta) / CGAL::Gmpq(yDelta);
        
        if (whRatioDiff >= whRatio)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    
    bool pointIsBoundedInYByPoint(const Point &other, const CGAL::Gmpq &whRatio) const
    {
        int xDelta = abs(other.x - x);
        int yDelta = abs(other.y - y);
        
        if (yDelta == 0)
        {
            return xDelta == 0;
        }
        
        CGAL::Gmpq whRatioDiff = CGAL::Gmpq(xDelta) / CGAL::Gmpq(yDelta);
        
        if (whRatioDiff <= whRatio)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
};

bool pointXAscending(const Point &lhs, const Point &rhs)
{
    return lhs.x < rhs.x;
}

bool pointYAscending(const Point &lhs, const Point &rhs)
{
    return lhs.y < rhs.y;
}

using namespace std;

double ceil_to_double(const ET &x)
{
    double a = std::ceil(CGAL::to_double(x));
    while (a < x)
        a += 1;
    while (a - 1 >= x)
        a -= 1;
    return a;
}

static void runTestCase()
{
    int n, m, h, w;
    cin >> n >> m >> h >> w;
    
    const CGAL::Gmpq whRatio = CGAL::Gmpq(w) / CGAL::Gmpq(h);
    
    Program lp(CGAL::SMALLER, true, 1, false, 0);
    lp.set_c0(0);
    
    // variables: scale factor for each new poster
    // target function: sum of all areas (simply sum of all variables, since they all multiply the same area (w*h) that factor can be ignored)
    // constraints: for each new poster:
    // find closest x nail to the left
    // find closest x nail to the right
    // find closest y nail to the top
    // find closest y nail to the bottom:
    // a * (w / 2) + a_other * (w / 2) <= x - x_old
    // a * w + a_other * w <= 2*(x - x_old)
    // bzw
    // a * w <= 2 * (x - x_old) - w
    // etc
    
    vector<Point> newPoints;
    newPoints.reserve(n);
    
    for (int i = 0; i < n; i++)
    { // new posters
        int x, y;
        cin >> x >> y;
        
        lp.set_c(i, -(2 * w + 2 * h));
        
        newPoints.push_back(Point(x, y, i));
    }
    
    vector<Point> oldPointsXSorted;
    oldPointsXSorted.reserve(m);
    vector<Point> oldPointsYSorted;
    oldPointsYSorted.reserve(m);
    
    for (int i = 0; i < m; i++)
    { // old posters
        int x, y;
        cin >> x >> y;
        
        oldPointsXSorted.push_back(Point(x, y, 0));
        oldPointsYSorted.push_back(Point(x, y, 0));
    }
    
    sort(oldPointsXSorted.begin(), oldPointsXSorted.end(), pointXAscending);
    sort(oldPointsYSorted.begin(), oldPointsYSorted.end(), pointYAscending);
    
    // Binary search does little to improve running time. Pretty useless. Simple for loop will also do the trick.
    const auto selectSuccessor = [&](const vector<Point> &xHaystack, const vector<Point> &yHaystack, const Point &point, bool y) -> const Point * {
        const vector<Point> &haystack = y ? yHaystack : xHaystack;
        
        int bound = upper_bound(haystack.begin(), haystack.end(), point, (y ? pointYAscending : pointXAscending)) - haystack.begin();
        
        while (bound < haystack.size())
        {
            const Point &found = haystack.at(bound);
            if (y ? point.pointIsBoundedInYByPoint(found, whRatio) : point.pointIsBoundedInXByPoint(found, whRatio))
            {
                return &found;
            }
            else
            {
                bound++;
            }
        }
        
        return NULL;
    };
    
    const auto selectPredecessor = [&](const vector<Point> &xHaystack, const vector<Point> &yHaystack, const Point &point, bool y) -> const Point * {
        const vector<Point> &haystack = y ? yHaystack : xHaystack;
        
        int bound = lower_bound(haystack.begin(), haystack.end(), point, (y ? pointYAscending : pointXAscending)) - haystack.begin();
        
        while (bound > 0 && bound <= haystack.size())
        {
            const Point &found = haystack.at(bound - 1);
            if (y ? point.pointIsBoundedInYByPoint(found, whRatio) : point.pointIsBoundedInXByPoint(found, whRatio))
            {
                return &found;
            }
            else
            {
                bound--;
            }
        }
        
        return NULL;
    };
    
    const auto selectBoundingPoints = [&](const vector<Point> &xHaystack, const vector<Point> &yHaystack, const Point &point) -> pair<Point *, Point *> {
        const Point *xPredecessor = selectPredecessor(xHaystack, yHaystack, point, false);
        const Point *yPredecessor = selectPredecessor(xHaystack, yHaystack, point, true);
        
        const Point *xSuccessor = selectSuccessor(xHaystack, yHaystack, point, false);
        const Point *ySuccessor = selectSuccessor(xHaystack, yHaystack, point, true);
        
        Point *xBound = NULL;
        Point *yBound = NULL;
        
        const auto useXBound = [&](Point *p) {
            if (p == NULL)
            {
                return;
            }
            if (xBound == NULL || abs(p->x - point.x) < abs(xBound->x - point.x))
            {
                xBound = p;
            }
        };
        
        const auto useYBound = [&](Point *p) {
            if (p == NULL)
            {
                return;
            }
            if (yBound == NULL || abs(p->y - point.y) < abs(yBound->y - point.y))
            {
                yBound = p;
            }
        };
        
        useXBound((Point *)xPredecessor);
        useXBound((Point *)xSuccessor);
        
        useYBound((Point *)yPredecessor);
        useYBound((Point *)ySuccessor);
        
        return {xBound, yBound};
    };
    
    int constraint = 0;
    for (int i = 0; i < n; i++)
    {
        const Point &newPoint = newPoints.at(i);
        
        const pair<Point *, Point *> oldBounds = selectBoundingPoints(oldPointsXSorted, oldPointsYSorted, newPoint);
        if (oldBounds.first != NULL)
        {
            const Point oldXBound = *oldBounds.first;
            lp.set_a(newPoint.index, constraint, w);
            lp.set_b(constraint, 2 * abs(oldXBound.x - newPoint.x) - w);
            constraint++;
        }
        
        if (oldBounds.second != NULL)
        {
            const Point oldYBound = *oldBounds.second;
            lp.set_a(newPoint.index, constraint, h);
            lp.set_b(constraint, 2 * abs(oldYBound.y - newPoint.y) - h);
            constraint++;
        }
        
        const auto useXBound = [&](const Point &p) {
            lp.set_a(newPoint.index, constraint, w);
            lp.set_a(p.index, constraint, w);
            lp.set_b(constraint, 2 * abs(p.x - newPoint.x));
            constraint++;
        };
        
        const auto useYBound = [&](const Point &p) {
            lp.set_a(newPoint.index, constraint, h);
            lp.set_a(p.index, constraint, h);
            lp.set_b(constraint, 2 * abs(p.y - newPoint.y));
            constraint++;
        };
        
        for (int j = i + 1; j < n; j++)
        {
            const Point &check = newPoints.at(j);
            if (check.index == newPoint.index)
            {
                continue;
            }
            
            if (newPoint.pointIsBoundedInXByPoint(check, whRatio))
            {
                useXBound(check);
            }
            else
            {
                useYBound(check);
            }
        }
    }
    
    Solution s = CGAL::solve_linear_program(lp, ET());
    
    cout << (long long)ceil_to_double(-s.objective_value_numerator() / s.objective_value_denominator()) << "\n";
}

int main(int argc, const char *argv[])
{
    ios_base::sync_with_stdio(false);
    
    int t;
    cin >> t;
    
    for (int i = 0; i < t; i++)
    {
        runTestCase();
    }
    
    return 0;
}
