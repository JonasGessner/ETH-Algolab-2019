//
//  shopping.cpp
//  Algorithms Lab
//
//  Created by Jonas Gessner
//  Copyright © 2019 Jonas Gessner. All rights reserved.
//

#include <iostream>

// BGL include #include<boost / graph / adjacency_list.hpp>
#include <boost/graph/adjacency_list.hpp>
// BGL flow include *NEW* #include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>

using namespace std;

typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property, boost::property<boost::edge_capacity_t, long, boost::property<boost::edge_residual_capacity_t, long, boost::property<boost::edge_reverse_t, traits::edge_descriptor>>>> graph;

typedef traits::vertex_descriptor vertex_desc;
typedef traits::edge_descriptor edge_desc;

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

static void runTestCase()
{
    int n, m, s;
    cin >> n >> m >> s;

    graph G(n);
    edge_adder adder(G);

    int source = boost::add_vertex(G);
    int target = boost::add_vertex(G);

    adder.add_edge(0, target, s); // Flow from home to target, allowing the capacity of 1 trip per store.

    for (int i = 0; i < s; i++)
    {
        int storeVertex;
        cin >> storeVertex;

        adder.add_edge(source, storeVertex, 1); // Flow from source to store with capacity 1, for 1 trip per store (edges can be reused for the to and from trips for 1 store).
    }

    for (int i = 0; i < m; i++)
    {
        int u, v;
        cin >> u >> v;

        // Add an udirected edge with capacity 1
        adder.add_edge(u, v, 1);
        adder.add_edge(v, u, 1);
    }

    int flow = boost::push_relabel_max_flow(G, source, target);

    if (flow == s)
    {
        cout << "yes\n";
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
