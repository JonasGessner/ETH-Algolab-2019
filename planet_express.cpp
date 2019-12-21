//
//  planet_express.cpp
//  Algorithms Lab
//
//  Created by Jonas Gessner
//  Copyright © 2019 Jonas Gessner. All rights reserved.
//

#include <iostream>
#include <vector>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/strong_components.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <limits>

using namespace std;

typedef boost::property<boost::edge_weight_t, int> EdgeWeightProperty;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS,
                              boost::no_property, EdgeWeightProperty>
    weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor edge_desc;
typedef boost::graph_traits<weighted_graph>::vertex_descriptor vertex_desc;

vector<int> dijkstra_dist(const weighted_graph &G, int s)
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
    int n, m, k, t;
    cin >> n >> m >> k >> t;

    vector<int> teleportationNetwork(t);

    weighted_graph G(n);

    for (int i = 0; i < t; i++)
    {
        int x;
        cin >> x;
        teleportationNetwork.at(i) = x;
    }

    for (int i = 0; i < m; i++)
    {
        int u, v, w;
        cin >> u >> v >> w;

        boost::add_edge(v, u, EdgeWeightProperty(w * 2), G); // We reverse all edges! Because instead of looking for paths from warehouses to vertex n - 1, we look at shortest paths from vertex n - 1 to all others, picking the warehouse that is closest!
        // We duplicate all edge weights so that we can avoid adding a clique to connect teleportable vertices and can instead add a star
    }

    // scc_map[i]: index of SCC containing i-th vertex
    vector<int> scc_map(n); // exterior property map
    // nscc: total number of SCCs
    int nscc = boost::strong_components(G,
                                        boost::make_iterator_property_map(scc_map.begin(), boost::get(boost::vertex_index, G)));

    vector<vector<int>> teleportationSCCs(nscc);

    for (const int &teleportVertex : teleportationNetwork)
    {
        const int &sccIndex = scc_map.at(teleportVertex);         // Which SCC is this teleportation vertex part of?
        teleportationSCCs.at(sccIndex).push_back(teleportVertex); // Save that this teleport vertex in in this SCC
    }

    int currentLastVertex = n;

    for (const vector<int> &teleportationSCC : teleportationSCCs)
    { // These are teleportation network vertices that all lie within the same SCC => They can teleport ot each other! Connect all these vertices through a central vertex (forming a star) and edge weights = number of the teleportation vertices in this SCC - 1
        int size = teleportationSCC.size();
        if (size > 1)
        {
            currentLastVertex++;

            for (int i = 0; i < size; i++)
            {
                const int &atI = teleportationSCC.at(i);

                // Now the teleportable vertices are connected through a star, centered by `currentLastVertex` and distance 2*size. This is why we duplicated all edge weights.
                boost::add_edge(atI, currentLastVertex, EdgeWeightProperty(size - 1), G);
                boost::add_edge(currentLastVertex, atI, EdgeWeightProperty(size - 1), G);
            }
        }
    }

    const vector<int> distances = dijkstra_dist(G, n - 1);

    int minDist = numeric_limits<int>::max();

    for (int i = 0; i < k; i++)
    {
        minDist = min(minDist, distances[i]);
    }

    minDist /= 2;

    if (minDist <= 1000000)
    {
        cout << minDist << "\n";
    }
    else
    {
        cout << "no\n";
    }
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
