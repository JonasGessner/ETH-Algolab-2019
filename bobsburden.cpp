//
//  bobsburden.cpp
//  Algorithms Lab
//
//  Created by Jonas Gessner
//  Copyright © 2019 Jonas Gessner. All rights reserved.
//

#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <limits>
#include <string>
#include <vector>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

typedef boost::property<boost::edge_weight_t, int> EW;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS,
                              boost::no_property, EW>
    weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor edge_desc;
typedef boost::graph_traits<weighted_graph>::vertex_descriptor vertex_desc;

std::vector<int> dijkstra_dists(const weighted_graph &G, int s) {
    int n = boost::num_vertices(G);
    std::vector<int> dist_map(n);

    boost::dijkstra_shortest_paths(G, s,
                                   boost::distance_map(boost::make_iterator_property_map(
                                       dist_map.begin(), boost::get(boost::vertex_index, G))));

    return dist_map;
}

using namespace std;

void testcase() {
    int k;
    cin >> k;

    weighted_graph G((k * (k + 1)));

    const auto vertexIndex = [&](const int x, const int y) {
        if (x > y || x < 1 || y < 1 || y > k) {
            return -1;
        }

        return (((y - 1) * y) / 2) + x - 1;
    };

    vector<int> weights((k * (k + 1)) / 2);

    for (int y = 1; y <= k; y++) {
        for (int x = 1; x <= y; x++) {
            int weight;
            cin >> weight;

            int index = vertexIndex(x, y);

            weights.at(index) = weight;

            int left = vertexIndex(x - 1, y);
            int right = vertexIndex(x + 1, y);

            int upLeft = vertexIndex(x - 1, y - 1);
            int upRight = vertexIndex(x, y - 1);

            int downLeft = vertexIndex(x, y + 1);
            int downRight = vertexIndex(x + 1, y + 1);

            int inVertex = 2 * index;
            int outVertex = inVertex + 1;

            boost::add_edge(inVertex, outVertex, EW(weight), G);

            if (upLeft != -1) {
                boost::add_edge(outVertex, 2 * upLeft, EW(0), G);
            }
            if (upRight != -1) {
                boost::add_edge(outVertex, 2 * upRight, EW(0), G);
            }
            if (right != -1) {
                boost::add_edge(outVertex, 2 * right, EW(0), G);
            }
            if (left != -1) {
                boost::add_edge(outVertex, 2 * left, EW(0), G);
            }
            if (downLeft != -1) {
                boost::add_edge(outVertex, 2 * downLeft, EW(0), G);
            }
            if (downRight != -1) {
                boost::add_edge(outVertex, 2 * downRight, EW(0), G);
            }
        }
    }

    const int top = 0;
    const int lowerLeft = vertexIndex(1, k);
    const int lowerRight = vertexIndex(k, k);

    const auto topDists = dijkstra_dists(G, 2 * top);
    const auto lowerLeftDists = dijkstra_dists(G, 2 * lowerLeft);
    const auto lowerRightDists = dijkstra_dists(G, 2 * lowerRight);

    int best = numeric_limits<int>::max();
    for (int y = 1; y <= k; y++) {
        for (int x = 1; x <= y; x++) {
            int index = vertexIndex(x, y);
            int weight = weights.at(index);

            int sum = topDists.at(2 * index) + lowerLeftDists.at(2 * index) + lowerRightDists.at(2 * index) + weight;

            best = min(best, sum);
        }
    }

    cout << best << "\n";
}

int main() {
    std::ios_base::sync_with_stdio(false);

    int t;
    std::cin >> t;
    for (int i = 0; i < t; ++i)
        testcase();
}
