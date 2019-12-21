//
//  strikesback.cpp
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

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_with_info_2<int, K> Vb;
typedef CGAL::Triangulation_face_base_2<K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb, Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K, Tds> Triangulation;

// example: decide whether two point sets R and B can be separated by a nonvertical line
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

// choose input type (input coefficients must fit)
typedef K::FT IT;
// choose exact type for solver (CGAL::Gmpz or CGAL::Gmpq)
typedef CGAL::Gmpq ET;

// program and solution types
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

using namespace std;

typedef struct
{
	K::Point_2 p;
	K::FT d;
} Asteroid;

void testcase()
{
	int a, s, b, e;
	cin >> a >> s >> b >> e;

	vector<Asteroid> asteroids;
	asteroids.reserve(a);

	Program lp(CGAL::LARGER, true, 0, true, e);
	lp.set_c0(0);

	for (int i = 0; i < a; i++)
	{
		int x, y, d;
		cin >> x >> y >> d;
		asteroids.push_back({K::Point_2(x, y), K::FT(d)});

		lp.set_b(1 + i, K::FT(d));
	}

	vector<K::Point_2> shots;
	shots.reserve(s);

	for (int i = 0; i < s; i++)
	{
		int x, y;
		cin >> x >> y;
		shots.push_back(K::Point_2(x, y));
		lp.set_c(i, 1);
		lp.set_a(i, 0, 1);
	}

	lp.set_r(0, CGAL::SMALLER);
	lp.set_b(0, e);

	vector<K::Point_2> hunters;
	hunters.reserve(b);

	for (int i = 0; i < b; i++)
	{
		int x, y;
		cin >> x >> y;
		hunters.push_back(K::Point_2(x, y));
	}

	Triangulation hunterTriangulation;
	hunterTriangulation.insert(hunters.begin(), hunters.end());

	Triangulation shotTriangulation;
	shotTriangulation.insert(shots.begin(), shots.end());

	int shot = 0;
	for (Triangulation::Vertex_iterator v = shotTriangulation.finite_vertices_begin(); v != shotTriangulation.finite_vertices_end(); ++v)
	{
		if (b == 0)
		{
			for (int i = 0; i < a; i++)
			{
				const K::FT distToAsteroid = CGAL::squared_distance(asteroids.at(i).p, v->point());

				lp.set_a(shot, 1 + i, K::FT(1) / max(distToAsteroid, K::FT(1)));
			}
		}
		else
		{
			const auto h = hunterTriangulation.nearest_vertex(v->point());
			const K::FT hunterDist = CGAL::squared_distance(h->point(), v->point());

			for (int i = 0; i < a; i++)
			{
				const K::FT distToAsteroid = CGAL::squared_distance(asteroids.at(i).p, v->point());

				if (distToAsteroid < hunterDist)
				{
					lp.set_a(shot, 1 + i, K::FT(1) / max(distToAsteroid, K::FT(1)));
				}
			}
		}

		shot++;
	}

	Solution sol = CGAL::solve_linear_program(lp, ET());
	assert(sol.solves_linear_program(lp));

	cout << (sol.is_infeasible() ? "n" : "y") << "\n";
}

int main()
{
	std::ios_base::sync_with_stdio(false);

	int t;
	std::cin >> t;
	for (int i = 0; i < t; ++i)
		testcase();
}
