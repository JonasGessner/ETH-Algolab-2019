//
//  ant_challenge.cpp
//  Algorithms Lab
//
//  Created by Jonas Gessner
//  Copyright © 2019 Jonas Gessner. All rights reserved.
//

#include <iostream>
#include <vector>
#include <map>
#include <unordered_map>

// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>

using namespace std;

typedef boost::property<boost::edge_weight_t, int> EdgeWeightProperty;

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
                              boost::no_property, EdgeWeightProperty>
    weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor edge_desc;
typedef boost::graph_traits<weighted_graph>::vertex_descriptor vertex_desc;

int dijkstra_dist(const weighted_graph &G, int s, int t)
{
    int n = boost::num_vertices(G);
    std::vector<int> dist_map(n);

    boost::dijkstra_shortest_paths(G, s,
                                   boost::distance_map(boost::make_iterator_property_map(
                                       dist_map.begin(), boost::get(boost::vertex_index, G))));

    return dist_map[t];
}

static void runTestCase()
{
    int n, m, s, a, b;
    cin >> n >> m >> s >> a >> b;

    // G is a multigraph containing representing a union of all MSTs of the species' graphs (aka their private networks).
    weighted_graph G(n);

    vector<weighted_graph> graphs(s, weighted_graph(n));

    for (int i = 0; i < m; i++)
    {
        int u, v;
        cin >> u >> v;

        for (int j = 0; j < s; j++)
        {
            int x;
            cin >> x;

            boost::add_edge(u, v, EdgeWeightProperty(x), graphs.at(j));
        }
    }

    for (int i = 0; i < s; i++)
    {
        int trash;
        cin >> trash;

        vector<edge_desc> edgeMap;

        boost::kruskal_minimum_spanning_tree(graphs.at(i), back_inserter(edgeMap));

        for (const auto &edge : edgeMap)
        {
            int source = boost::source(edge, graphs.at(i));
            int target = boost::target(edge, graphs.at(i));
            int weight = boost::get(boost::edge_weight, graphs.at(i), edge);

            boost::add_edge(source, target, EdgeWeightProperty(weight), G);
        }
    }

    cout << dijkstra_dist(G, a, b) << "\n";
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
