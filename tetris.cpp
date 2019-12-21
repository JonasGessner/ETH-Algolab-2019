//
//  tetris.cpp
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
    int w, n;
    cin >> w >> n;

    graph G(2 * (w + 1)); // We need a vertex (split into 2 to limit vertex flow) for every width position. This includes 0 and w and hence we need w + 1 vertices.
    edge_adder adder(G);

    for (int i = 0; i <= w; i++)
    {
        int cap = (i == 0 || i == w) ? n : 1;

        adder.add_edge(2 * i, (2 * i) + 1, cap); // connect the incoming and outgoing vertices with capacity 1, limiting the vertex capacity to 1. Unless it is the first or the last vertex
    }

    for (int i = 0; i < n; i++)
    {
        int e1, e2;
        cin >> e1 >> e2;
        int left = min(e1, e2);
        int right = max(e1, e2);

        adder.add_edge((2 * left) + 1, 2 * right, 1); // Add an edge from the outgoing vertex at `left` to the incoming vertex at `right`
    }

    int flow = boost::push_relabel_max_flow(G, 0, (2 * (w + 1)) - 1);

    cout << flow << "\n";
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
