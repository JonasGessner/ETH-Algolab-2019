//
//  canteen.cpp
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
                              boost::property<boost::edge_capacity_t, int,
                                              boost::property<boost::edge_residual_capacity_t, int,
                                                              boost::property<boost::edge_reverse_t, traits::edge_descriptor,
                                                                              boost::property<boost::edge_weight_t, int>>>>>
    graph; // new! weightmap corresponds to costs

typedef boost::graph_traits<graph>::edge_descriptor edge_desc;
typedef boost::graph_traits<graph>::out_edge_iterator out_edge_it; // Iterator

// Custom edge adder class
class edge_adder
{
    graph &G;

public:
    explicit edge_adder(graph &G) : G(G) {}
    void add_edge(int from, int to, int capacity, int cost)
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
    int n;
    cin >> n;

    graph G(n);
    edge_adder adder(G);

    const int source = boost::add_vertex(G);
    const int target = boost::add_vertex(G);

    for (int i = 0; i < n; i++)
    {
        int mealAmount, mealCost;

        cin >> mealAmount >> mealCost;

        adder.add_edge(source, i, mealAmount, mealCost); // connect source
    }

    long consumerCount = 0;

    for (int i = 0; i < n; i++)
    {
        int eatersAmount, mealPrice;

        cin >> eatersAmount >> mealPrice;

        consumerCount += eatersAmount;

        adder.add_edge(i, target, eatersAmount, 20 - mealPrice); // connect to target, set prices. Offset prices to make them non-negative
    }

    for (int i = 0; i < (n - 1); i++)
    {
        int storageCapacity, storagePrice;

        cin >> storageCapacity >> storagePrice;

        adder.add_edge(i, i + 1, storageCapacity, storagePrice); // store food for 1 night
    }

    auto c_map = boost::get(boost::edge_capacity, G);
    auto rc_map = boost::get(boost::edge_residual_capacity, G);

    boost::successive_shortest_path_nonnegative_weights(G, source, target);
    int cost = boost::find_flow_cost(G);

    // Iterate over all edges leaving the source to sum up the flow values.
    int flow = 0;
    out_edge_it e, eend;
    for (boost::tie(e, eend) = boost::out_edges(boost::vertex(source, G), G); e != eend; ++e)
    {
        flow += c_map[*e] - rc_map[*e];
    }

    if (flow == consumerCount)
    {
        cout << "possible " << flow << " " << -(cost - flow * 20) << "\n"; // subtract the price offset added above
    }
    else
    {
        cout << "impossible " << flow << " " << -(cost - flow * 20) << "\n";
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
