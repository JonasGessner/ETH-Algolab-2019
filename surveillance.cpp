//
//  surveillance.cpp
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

#include <boost/graph/strong_components.hpp>

// BGL include
#include <boost/graph/adjacency_list.hpp>

// BGL flow include *NEW*
#include <boost/graph/push_relabel_max_flow.hpp>

// Graph Type with nested interior edge properties for flow algorithms
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
							  boost::property<boost::edge_capacity_t, long,
											  boost::property<boost::edge_residual_capacity_t, long,
															  boost::property<boost::edge_reverse_t, traits::edge_descriptor>>>>
	graph;

typedef traits::vertex_descriptor vertex_desc;
typedef traits::edge_descriptor edge_desc;

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS> findGraph;
typedef boost::graph_traits<findGraph>::out_edge_iterator out_edge_it;

// Custom edge adder class, highly recommended
class edge_adder
{
	graph &G;

public:
	explicit edge_adder(graph &G) : G(G) {}

	void add_edge(int from, int to, long capacity)
	{
		auto c_map = boost::get(boost::edge_capacity, G);
		auto r_map = boost::get(boost::edge_reverse, G);
		const auto e = boost::add_edge(from, to, G).first;
		const auto rev_e = boost::add_edge(to, from, G).first;
		c_map[e] = capacity;
		c_map[rev_e] = 0; // reverse edge has no capacity!
		r_map[e] = rev_e;
		r_map[rev_e] = e;
	}
};

void testcase()
{
	int n, m, k, l;
	cin >> n >> m >> k >> l;

	graph returnGraph(n + l + k);
	edge_adder returnAdder(returnGraph);
	const int returnSource = boost::add_vertex(returnGraph);
	const int returnTarget = boost::add_vertex(returnGraph);

	findGraph findGraph(n);

	const int flowInStationsOffset = n;
	const int photosLimiterOffset = n + k;

	vector<int> stations(k);
	for (int i = 0; i < k; i++)
	{
		int x;
		cin >> x;
		stations.at(i) = x;
		returnAdder.add_edge(x, returnTarget, 1);						 // One picture per station
		returnAdder.add_edge(returnSource, flowInStationsOffset + i, 1); // Station at index i is station x
	}

	unordered_map<int, vector<int>> photos;
	for (int i = 0; i < l; i++)
	{
		int x;
		cin >> x;

		returnAdder.add_edge(photosLimiterOffset + i, x, 1); // Photo and index i is photo x. Photo can only be taken once
		if (photos.find(x) != photos.end())
		{
			photos.at(x).push_back(i);
		}
		else
		{
			vector<int> bro;
			bro.push_back(i);
			photos[x] = bro;
		}
	}

	for (int i = 0; i < m; i++)
	{
		int u, v;
		cin >> u >> v;

		returnAdder.add_edge(u, v, 1);
		boost::add_edge(u, v, findGraph);
	}

	for (int i = 0; i < k; i++)
	{
		const int station = stations.at(i);
		// cout << "Station " << station << ":";

		// Vertex is also a photo vertex. Find the indices of the photos at this vertex and add an edge to the flow limiter for these photos
		if (photos.find(station) != photos.end())
		{
			for (const int photoIndex : photos.at(station))
			{
				// cout << " - " << station;
				returnAdder.add_edge(flowInStationsOffset + i, photosLimiterOffset + photoIndex, 1); // Flow from this station to the limiter for this photo
			}
		}

		std::vector<int> vis(n, false); // visited flags
		std::queue<int> Q;				// BFS queue (from std:: not boost::)
		vis[station] = true;			// Mark the source as visited
		Q.push(station);
		while (!Q.empty())
		{
			const int u = Q.front();
			Q.pop();
			out_edge_it ebeg, eend;
			for (boost::tie(ebeg, eend) = boost::out_edges(u, findGraph); ebeg != eend; ++ebeg)
			{
				const int v = boost::target(*ebeg, findGraph);
				if (vis[v])
				{
					continue;
				}
				if (photos.find(v) != photos.end())
				{
					for (const int photoIndex : photos.at(v))
					{
						// cout << " " << v;
						returnAdder.add_edge(flowInStationsOffset + i, photosLimiterOffset + photoIndex, 1); // Flow from this station to the limiter for this photo
					}
				}
				vis[v] = true;
				Q.push(v);
			}
		}
		// cout << "\n";
	}

	int flow = boost::push_relabel_max_flow(returnGraph, returnSource, returnTarget);

	cout << flow << "\n";
}

int main()
{
	std::ios_base::sync_with_stdio(false);

	int t;
	std::cin >> t;
	for (int i = 0; i < t; ++i)
		testcase();
}
