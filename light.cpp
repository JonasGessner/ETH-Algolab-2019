//
//  light.cpp
//  Algorithms Lab
//
//  Created by Jonas Gessner
//  Copyright © 2019 Jonas Gessner. All rights reserved.
//

#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>

using namespace std;

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K> Triangulation;
typedef Triangulation::Edge_iterator Edge_iterator;

void testcase()
{
	int n, m;
	cin >> m >> n;

	std::vector<pair<K::Point_2, long>> people;
	people.reserve(m);
	for (std::size_t i = 0; i < m; ++i)
	{
		int x, y;
		long r;
		std::cin >> x >> y >> r;
		people.push_back({K::Point_2(x, y), r});
	}

	long h;
	cin >> h;

	std::vector<K::Point_2> lights;
	lights.reserve(n);
	for (std::size_t i = 0; i < n; ++i)
	{
		int x, y;
		std::cin >> x >> y;
		lights.push_back(K::Point_2(x, y));
	}

	// construct triangulation
	Triangulation t;
	t.insert(lights.begin(), lights.end());

	vector<int> winnersSon;

	vector<int> hits(m, -1);
	int maxHit = 0;

	for (int i = 0; i < m; i++)
	{
		const auto person = people.at(i);
		long minDIstForWin = (h + person.second) * (h + person.second);

		const auto nearest = t.nearest_vertex(person.first);
		const auto nearestPoint = nearest->point();
		const K::FT dist = CGAL::squared_distance(nearestPoint, person.first);
		if (dist >= minDIstForWin)
		{
			winnersSon.push_back(i);
		}
		else
		{
			for (int j = 0; j < n; j++)
			{
				if (CGAL::squared_distance(lights.at(j), person.first) < minDIstForWin)
				{
					hits.at(i) = j;
					maxHit = max(maxHit, j);
					break;
				}
			}
		}
	}

	if (winnersSon.empty())
	{
		int i = 0;
		for (int shit : hits)
		{
			if (shit == maxHit)
			{
				cout << i << " ";
			}

			i++;
		}
	}

	for (int w : winnersSon)
	{
		cout << w << " ";
	}
	cout << "\n";
}

int main()
{
	std::ios_base::sync_with_stdio(false);

	int t;
	std::cin >> t;
	for (int i = 0; i < t; ++i)
		testcase();
}
