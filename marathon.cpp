//
//  marathon.cpp
//  Algorithms Lab
//
//  Created by Jonas Gessner
//  Copyright © 2019 Jonas Gessner. All rights reserved.
//

#include <iostream>

// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/cycle_canceling.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>

#include <boost/graph/dijkstra_shortest_paths.hpp>

// Graph Type with nested interior edge properties for Cost Flow Algorithms
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS>
    traits;
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
    std::pair<edge_desc, edge_desc> add_edge(int from, int to, long capacity, long cost)
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
        w_map[e] = cost;      // new assign cost
        w_map[rev_e] = -cost; // new negative cost

        return {e, rev_e};
    }
};

std::vector<int> dijkstra_dists(const graph &G, int s)
{
    int n = boost::num_vertices(G);
    std::vector<int> dist_map(n);

    boost::dijkstra_shortest_paths(G, s,
                                   boost::distance_map(boost::make_iterator_property_map(
                                       dist_map.begin(), boost::get(boost::vertex_index, G))));

    return dist_map;
}

using namespace std;

static void runTestCase()
{
    int n, m, source, target;
    cin >> n >> m >> source >> target;

    graph dijkstraGraph(n);
    auto dijkstraEdgeWeights = boost::get(boost::edge_weight, dijkstraGraph); // new!

    graph G(n);
    edge_adder adder(G);

    // Don't need costs really but it works now so wont change it.

    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    auto w_map = boost::get(boost::edge_weight, G); // new!
    auto rc_map = boost::get(boost::edge_residual_capacity, G);

    // configure graph

    for (int i = 0; i < m; i++)
    {
        int u, v, width, length;

        cin >> u >> v >> width >> length;

        if (u == v)
        {
            continue; // Edge couldn't possibly be on shortest path and if length is 0 it still couldn't augment the max flow
        }

        adder.add_edge(u, v, width, length);
        adder.add_edge(v, u, width, length);

        const edge_desc edge1 = boost::add_edge(u, v, dijkstraGraph).first;
        const edge_desc edge2 = boost::add_edge(v, u, dijkstraGraph).first;

        dijkstraEdgeWeights[edge1] = length;
        dijkstraEdgeWeights[edge2] = length;
    }

    // find shortest s-t path
    const vector<int> shortestPaths = dijkstra_dists(dijkstraGraph, source);
    const int shortestST = shortestPaths[target];

    // Remove all non shortest path edges
    for (int i = 0; i < n; i++)
    {
        const int minDist = shortestPaths[i];

        out_edge_it e, eend;
        for (boost::tie(e, eend) = boost::out_edges(boost::vertex(i, G), G); e != eend; ++e)
        {
            const int length = w_map[*e];

            const int minDistToOther = shortestPaths[e->m_target];

            if (minDist + length > min(minDistToOther, shortestST))
            { // Not shortest path edge. Make flow 0
                c_map[*e] = 0;
            }
        }
    }

    const long maxFlow = boost::push_relabel_max_flow(G, source, target);

    cout << maxFlow << "\n";
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
