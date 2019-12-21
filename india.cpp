//
//  india.cpp
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

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/cycle_canceling.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>

// Graph Type with nested interior edge properties for Cost Flow Algorithms
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
							  boost::property<boost::edge_capacity_t, long,
											  boost::property<boost::edge_residual_capacity_t, long,
															  boost::property<boost::edge_reverse_t, traits::edge_descriptor,
																			  boost::property<boost::edge_weight_t, long>>>>>
	graph; // new! weightmap corresponds to costs

typedef boost::graph_traits<graph>::edge_descriptor edge_desc;
typedef boost::graph_traits<graph>::out_edge_iterator out_edge_it; // Iterator

// Custom edge adder class
class edge_adder
{
	graph &G;

public:
	explicit edge_adder(graph &G) : G(G) {}
	edge_desc add_edge(int from, int to, long capacity, long cost)
	{
		auto c_map = boost::get(boost::edge_capacity, G);
		auto r_map = boost::get(boost::edge_reverse, G);
		auto w_map = boost::get(boost::edge_weight, G); // new!
		const edge_desc e = boost::add_edge(from, to, G).first;
		const edge_desc rev_e = boost::add_edge(to, from, G).first;
		c_map[e] = capacity;
		c_map[rev_e] = 0; // reverse edge has no capacity!
		r_map[e] = rev_e;
		r_map[rev_e] = e;
		w_map[e] = cost;	  // new assign cost
		w_map[rev_e] = -cost; // new negative cost

		return e;
	}
};

using namespace std;

void testcase()
{
	int n, m, cash, start, target;
	cin >> n >> m >> cash >> start >> target;

	graph G(n);
	edge_adder adder(G);

	auto c_map = boost::get(boost::edge_capacity, G);
	auto r_map = boost::get(boost::edge_reverse, G);
	auto rc_map = boost::get(boost::edge_residual_capacity, G);

	int cheapest = numeric_limits<int>::max();
	for (int i = 0; i < m; i++)
	{
		int u, v, cost, cap;
		cin >> u >> v >> cost >> cap;
		adder.add_edge(u, v, cap, cost);

		cheapest = min(cheapest, cost);
	}

	const int source = boost::add_vertex(G);

	long r = cash / cheapest;
	long l = 0;

	int cost;

	edge_desc limiterEdge = adder.add_edge(source, start, 0, 0);

	long smallFlow = 0;
	while (l < r)
	{
		long mid = (l + r) / 2 + (l + r) % 2;

		c_map[limiterEdge] = mid;

		boost::successive_shortest_path_nonnegative_weights(G, source, target);
		cost = boost::find_flow_cost(G);

		out_edge_it e, eend;

		long currentFlow = 0;
		for (boost::tie(e, eend) = boost::out_edges(boost::vertex(source, G), G); e != eend; ++e)
		{
			currentFlow += c_map[*e] - rc_map[*e];
		}

		if (cost > cash)
		{
			r = mid - 1;
		}
		else
		{
			smallFlow = currentFlow;
			l = mid;
		}
	}

	cout << smallFlow << "\n";
}

int main()
{
	std::ios_base::sync_with_stdio(false);

	int t;
	std::cin >> t;
	for (int i = 0; i < t; ++i)
		testcase();
}
