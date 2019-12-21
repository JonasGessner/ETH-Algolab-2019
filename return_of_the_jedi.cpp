//
//  return_of_the_jedi.cpp
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
#include <queue>

using namespace std;

// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <boost/graph/prim_minimum_spanning_tree.hpp>
#include <boost/graph/lookup_edge.hpp>

typedef boost::property<boost::edge_weight_t, int> EdgeWeightProperty;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
							  boost::no_property, EdgeWeightProperty>
	weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor edge_desc;
typedef boost::graph_traits<weighted_graph>::vertex_descriptor vertex_desc;

void kruskal(const weighted_graph &G)
{
	std::vector<edge_desc> mst; // vector to store MST edges (not a property map!)

	boost::kruskal_minimum_spanning_tree(G, std::back_inserter(mst));

	for (std::vector<edge_desc>::iterator it = mst.begin(); it != mst.end(); ++it)
	{
		std::cout << boost::source(*it, G) << " " << boost::target(*it, G) << "\n";
	}
}

void testcase()
{
	int n, start;
	cin >> n >> start;

	int heaviestEdge = 0;
	weighted_graph G(n);
	for (int i = 0; i < n - 1; i++)
	{
		for (int j = i + 1; j < n; j++)
		{
			int weight;
			cin >> weight;
			heaviestEdge = max(heaviestEdge, weight);
			boost::add_edge(i, j, EdgeWeightProperty(weight), G);
		}
	}

	weight_map weights = boost::get(boost::edge_weight, G);

	vector<int> predecessorMap(n);
	vector<int> weightToPredecessorMap(n);

	boost::prim_minimum_spanning_tree(G,
									  boost::make_iterator_property_map(predecessorMap.begin(), boost::get(boost::vertex_index, G)),
									  boost::root_vertex(n - 1)
										  .distance_map(boost::make_iterator_property_map(weightToPredecessorMap.begin(), boost::get(boost::vertex_index, G))));

	weighted_graph MST(n);
	int initialMSTWeight = 0;
	for (int i = 0; i < n; i++)
	{
		int predecessor = predecessorMap.at(i);
		if (predecessor != i)
		{
			int w = weightToPredecessorMap.at(i);
			initialMSTWeight += w;
			boost::add_edge(i, predecessor, EdgeWeightProperty(w), MST);
		}
	}

	weight_map weightsMST = boost::get(boost::edge_weight, MST);

	// Try adding in every edge (u, v) in G that is not already part of the MST.
	// For each edge, find the path between u and v and remove the edge with maximal weight from that path, while adding in (u, v).
	// Find the edge where the added weight is minimized

	vector<vector<int>> MSTPathMaxEdgeWeight(n, vector<int>(n, 0));

	for (int u = 0; u < n; u++)
	{
		std::vector<int> vis(n, false); // visited flags
		std::queue<int> Q;				// BFS queue (from std:: not boost::)
		vis[u] = true;					// Mark the source as visited
		Q.push(u);
		while (!Q.empty())
		{
			bool found = false;
			const int a = Q.front();
			Q.pop();
			weighted_graph::out_edge_iterator ebeg1, eend1;
			for (boost::tie(ebeg1, eend1) = boost::out_edges(a, MST); ebeg1 != eend1; ++ebeg1)
			{
				const int b = boost::target(*ebeg1, MST);
				// Only follow edges with spare capacity
				if (vis[b])
				{
					continue;
				}

				MSTPathMaxEdgeWeight[u][b] = max(MSTPathMaxEdgeWeight[u][a], weightsMST[*ebeg1]);

				vis[b] = true;
				Q.push(b);
			}
		}
	}

	int minDiff = numeric_limits<int>::max();

	weighted_graph::edge_iterator ebeg, eend;
	for (tie(ebeg, eend) = boost::edges(G); ebeg != eend; ++ebeg)
	{
		const int u = boost::source(*ebeg, G);
		const int v = boost::target(*ebeg, G);

		if (predecessorMap[u] == v || predecessorMap[v] == u)
		{
			continue; // Is an MST edge
		}

		int weightOfEdge = weights[*ebeg];

		int diff = weightOfEdge - MSTPathMaxEdgeWeight[u][v]; // This is the difference in weight we get by removing the max weight incident edge and adding this edge

		minDiff = min(minDiff, diff);
	}

	cout << initialMSTWeight + minDiff << "\n";
}

int main()
{
	std::ios_base::sync_with_stdio(false);

	int t;
	std::cin >> t;
	for (int i = 0; i < t; ++i)
		testcase();
}
