//
//  motorcycles.cpp
//  Algorithms Lab
//
//  Created by Jonas Gessner
//  Copyright © 2019 Jonas Gessner. All rights reserved.
//

#include <iostream>
#include <vector>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <algorithm>

typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef K::Point_2 Point;
typedef K::Segment_2 Segment;
typedef K::Ray_2 Ray;

using namespace std;

typedef struct
{
    K::FT yStart;
    K::Sign slopeSign;
    K::FT absoluteSlope;
    int index;
} Biker;

inline Biker makeBiker(const K::FT &yStart, const K::Sign &slopeSign, const K::FT &absoluteSlope, const int &index)
{
    Biker b;
    b.yStart = yStart;
    b.slopeSign = slopeSign;
    b.absoluteSlope = absoluteSlope;
    b.index = index;

    return b;
}

bool bikersAscendingByIndex(const Biker &lhs, const Biker &rhs)
{
    return lhs.index < rhs.index;
}

bool bikersDescendingByYStart(const Biker &lhs, const Biker &rhs)
{
    return lhs.yStart > rhs.yStart;
}

static void runTestCase()
{
    int n;
    cin >> n;

    vector<Biker> rays(n);

    for (int i = 0; i < n; i++)
    {
        long y0, x1, y1;
        cin >> y0 >> x1 >> y1;

        auto p1 = Point(0, y0);
        auto p2 = Point(x1, y1);

        auto slope = (p2.y() - p1.y()) / p2.x();
        K::Sign sign = CGAL::sign(slope);
        auto absSlope = CGAL::abs(slope);

        Biker biker = makeBiker(y0, sign, absSlope, i);

        rays.at(i) = biker;
    }

    sort(rays.begin(), rays.end(), bikersDescendingByYStart);

    vector<Biker> res;

    for (int i = 0; i < n; i++)
    {
        const Biker current = rays.at(i);

        if (current.slopeSign == CGAL::NEGATIVE)
        {
            if (res.empty() || -current.absoluteSlope <= res.back().absoluteSlope * res.back().slopeSign) // previous infinite is sloping higher or equal than current — no intersection, add to res.
            {
                res.push_back(current);
            }
            else
            { // Previous infinite is facing down and at a greater slope than current. Delete
                for (int j = res.size() - 1; j >= 0; j--)
                {
                    const Biker prev = res.at(j);

                    if (prev.slopeSign * prev.absoluteSlope < -current.absoluteSlope)
                    {
                        res.erase(res.begin() + j);
                    }
                    else
                    {
                        break;
                    }
                }

                res.push_back(current);
            }
        }
        else
        {
            if (res.empty() || current.absoluteSlope <= res.back().absoluteSlope * res.back().slopeSign) // previous infinite is sloping higher or equal than current — no intersection, add to res.
            {
                res.push_back(current);
            }
            else
            { // There is an intersection between current and the previous infinite ray.
                bool add = true;

                for (int j = res.size() - 1; j >= 0; j--)
                {
                    const Biker prev = res.at(j);

                    if (current.absoluteSlope > prev.absoluteSlope)
                    {
                        add = false;
                        break; // Previous infinite ray has a smaller absolute slope. Current ray will be deleted
                    }
                    else if (current.absoluteSlope <= res.back().absoluteSlope * res.back().slopeSign) // previous infinite is sloping higher or equal than current — no intersection, add to res.
                    {
                        break;
                    }
                    else
                    { // There is an intersection and the previous infinite gets eliminated because it has a higher or equal (and is coming from the left) absolute slope.
                        res.erase(res.begin() + j);
                    }
                }

                if (add)
                {
                    res.push_back(current);
                }
            }
        }
    }

    sort(res.begin(), res.end(), bikersAscendingByIndex);

    for (const Biker &biker : res)
    {
        cout << biker.index << " ";
    }

    cout << "\n";
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
