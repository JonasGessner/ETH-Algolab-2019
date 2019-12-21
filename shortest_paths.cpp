//
//  shortest_paths.cpp
//  Algorithms Lab
//
//  Created by Jonas Gessner
//  Copyright © 2019 Jonas Gessner. All rights reserved.
//

#include <iostream>
#include <vector>
#include <limits>

using namespace std;

// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/floyd_warshall_shortest.hpp>

typedef boost::property<boost::edge_weight_t, int> EdgeWeightProperty;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS,
                              boost::no_property, EdgeWeightProperty>
    weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor edge_desc;
typedef boost::graph_traits<weighted_graph>::vertex_descriptor vertex_desc;

vector<int> dijkstra_dist_map(const weighted_graph &G, int s)
{
    int n = boost::num_vertices(G);
    std::vector<int> dist_map(n);

    boost::dijkstra_shortest_paths(G, s,
                                   boost::distance_map(boost::make_iterator_property_map(
                                       dist_map.begin(), boost::get(boost::vertex_index, G))));

    return dist_map;
}

static void runTestCase()
{
    int n, m, q;
    cin >> n >> m >> q;

    weighted_graph G(n);

    for (int i = 0; i < n; i++)
    {
        int noop;
        cin >> noop >> noop;
    }

    for (int i = 0; i < m; i++)
    {
        int u, v, w;
        cin >> u >> v >> w;

        boost::add_edge(u, v, EdgeWeightProperty(w), G);
    }

    vector<vector<int>> distMaps(n);

    for (int i = 0; i < q; i++)
    {
        int a, b;
        cin >> a >> b;

        if (distMaps.at(a).size() == 0)
        {
            distMaps.at(a) = dijkstra_dist_map(G, a);
        }

        int dist = distMaps.at(a).at(b);

        if (dist < numeric_limits<int>::max())
        {
            cout << dist << "\n";
        }
        else
        {
            cout << "unreachable\n";
        }
    }
}

int main(int argc, const char *argv[])
{
    ios_base::sync_with_stdio(false);

    runTestCase();

    return 0;
}
