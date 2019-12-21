//
//  tourofgaul.cpp
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
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/cycle_canceling.hpp>
#include <boost/graph/find_flow_cost.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>

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
class edge_adder {
    graph &G;

public:
    explicit edge_adder(graph &G) : G(G) {}
    edge_desc add_edge(int from, int to, long capacity, long cost) {
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

        return e;
    }
};

using namespace std;

typedef struct {
    int a;
    int b;
    int d;
} Food;

void testcase() {
    int n, m;
    cin >> n >> m;

    const int edgeCostBase = 128;

    graph G(n);
    edge_adder adder(G);

    auto cost_map = boost::get(boost::edge_weight, G);
    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    auto rc_map = boost::get(boost::edge_residual_capacity, G);

    const auto realSource = boost::add_vertex(G);
    const auto source = boost::add_vertex(G);
    const auto target = boost::add_vertex(G);

    const edge_desc srcEdge = adder.add_edge(realSource, source, numeric_limits<int>::max() / 2 - 1, 0);

    int lastC = 0;
    for (int i = 0; i < n; i++) {
        // Achieve a uniform cost of edgeCostBase/flow unit/trip. This cost is only lowered by transporting food, by the reward for each transported food.

        int c = 0;
        if (i < (n - 1)) {
            cin >> c;

            // Ensure flow is always maximum along path between cities
            if (c > lastC) {
                adder.add_edge(source, i, c - lastC, edgeCostBase * i); // Allow additional flow. This a shortcut from the source. Set the cost to the missed cost of the trips that are skipped
            }

            adder.add_edge(i, i + 1, c, edgeCostBase);
        }

        if (i > 0) {
            if (c < lastC || i == (n - 1)) {
                adder.add_edge(i, target, lastC - c, edgeCostBase * ((n - 1) - i)); // Allow "venting" flow. Same here, add all the skipped costs
            }
        }

        lastC = c;
    }

    for (int i = 0; i < m; i++) {
        Food f;
        cin >> f.a >> f.b >> f.d;

        int cost = ((f.b - f.a) * edgeCostBase) - f.d;

        bool found = false;

        out_edge_it e, eend;
        for (boost::tie(e, eend) = boost::out_edges(boost::vertex(f.a, G), G); e != eend; ++e) {
            if (boost::target(*e, G) == f.b) {
                if (cost_map[*e] == cost) {
                    c_map[*e]++;
                    found = true;
                    break;
                }
            }
        }

        if (found) {
            continue;
        }

        adder.add_edge(f.a, f.b, 1, cost);
    }

    boost::successive_shortest_path_nonnegative_weights(G, realSource, target);
    int cost2 = boost::find_flow_cost(G);

    int s_flow = c_map[srcEdge] - rc_map[srcEdge];

    cout << ((n - 1) * edgeCostBase * s_flow) - cost2 << "\n";
}

int main() {
    std::ios_base::sync_with_stdio(false);

    int t;
    std::cin >> t;
    for (int i = 0; i < t; ++i)
        testcase();
}
