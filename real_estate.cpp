//
//  real_estate.cpp
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
    void add_edge(int from, int to, long capacity, long cost)
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
    }
};

using namespace std;

static void runTestCase()
{
    int n, m, s;
    cin >> n >> m >> s;

    graph G(s + n + m);
    edge_adder adder(G);

    const int source = boost::add_vertex(G);
    const int target = boost::add_vertex(G);

    const int propertyOffset = s;
    const int buyersOffset = propertyOffset + m;

    const long bigAssFlow = numeric_limits<long>::max() / 2;

    for (int i = 0; i < s; i++)
    {
        int stateLimit;
        cin >> stateLimit;

        adder.add_edge(source, i, stateLimit, 0);
    }

    // Fucking states are not 0-indexed
    for (int i = 0; i < m; i++)
    {
        int state;
        cin >> state;

        adder.add_edge(state - 1, propertyOffset + i, 1, 0);
    }

    for (int i = 0; i < n; i++)
    {
        adder.add_edge(buyersOffset + i, target, 1, 0);

        for (int j = 0; j < m; j++)
        {
            int bid;
            cin >> bid;

            adder.add_edge(propertyOffset + j, buyersOffset + i, 1, 100 - bid);
        }
    }

    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    auto rc_map = boost::get(boost::edge_residual_capacity, G);

    // Option 2: Min Cost Max Flow with successive_shortest_path_nonnegative_weights
    boost::successive_shortest_path_nonnegative_weights(G, source, target);
    int cost = boost::find_flow_cost(G);

    // Iterate over all edges leaving the source to sum up the flow values.
    int s_flow = 0;
    out_edge_it e, eend;
    for (boost::tie(e, eend) = boost::out_edges(boost::vertex(source, G), G); e != eend; ++e)
    {
        s_flow += c_map[*e] - rc_map[*e];
    }

    cout << s_flow << " " << -(cost - (s_flow * 100)) << "\n";
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
