//
//  cantonal_courier.cpp
//  Algorithms Lab
//
//  Created by Jonas Gessner
//  Copyright © 2019 Jonas Gessner. All rights reserved.
//

#include <iostream>
#include <vector>

// BGL include
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>

using namespace std;

// Graph Type with nested interior edge properties for flow algorithms
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
                              boost::property<boost::edge_capacity_t, long,
                                              boost::property<boost::edge_residual_capacity_t, long,
                                                              boost::property<boost::edge_reverse_t, traits::edge_descriptor>>>>
    graph;

typedef traits::vertex_descriptor vertex_desc;
typedef traits::edge_descriptor edge_desc;

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

void runTestCase()
{
    int z, j;
    cin >> z >> j;

    graph G(z + j);
    edge_adder adder(G);

    const vertex_desc source = boost::add_vertex(G);
    const vertex_desc target = boost::add_vertex(G);

    for (int i = 0; i < z; i++)
    {
        // connect each zone with target and set capacity to the cost of this zone
        int cost;
        cin >> cost;
        adder.add_edge(i + j, target, cost);
    }

    int maxProfit = 0;
    for (int i = 0; i < j; i++)
    {
        // create edge from source to each job with capacity = reward for this job
        // also add rewards to maximum reward
        int profit;
        cin >> profit;
        maxProfit += profit;
        adder.add_edge(source, i, profit);
    }

    for (int i = 0; i < j; i++)
    {
        // now connect each jobs with the zone it needs to pass
        // the capacity of this edge does not matter because it is limited by the input from the source and output of the zones
        int ticketsNeeded;
        cin >> ticketsNeeded;

        for (int k = 0; k < ticketsNeeded; k++)
        {
            int zone;
            cin >> zone;
            adder.add_edge(i, zone + j, 10000); // Just any capacity over 5000
        }
    }

    long max = boost::push_relabel_max_flow(G, source, target);
    cout << maxProfit - minUnachievableProfit << "\n";
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
