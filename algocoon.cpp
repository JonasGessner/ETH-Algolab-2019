//
//  algocoon.cpp
//  Algorithms Lab
//
//  Created by Jonas Gessner
//  Copyright © 2019 Jonas Gessner. All rights reserved.
//

#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <queue>

// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/tuple/tuple.hpp>

// BGL graph definitions
// =====================
// Graph Type with nested interior edge properties for Flow Algorithms
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
                              boost::property<boost::edge_capacity_t, long,
                                              boost::property<boost::edge_residual_capacity_t, long,
                                                              boost::property<boost::edge_reverse_t, traits::edge_descriptor>>>>
    graph;
// Interior Property Maps
typedef boost::graph_traits<graph>::edge_descriptor edge_desc;
typedef boost::graph_traits<graph>::out_edge_iterator out_edge_it;

// Custom Edge Adder Class, that holds the references
// to the graph, capacity map and reverse edge map
// ===================================================
class edge_adder
{
    graph &G;

public:
    explicit edge_adder(graph &G) : G(G) {}

    void add_edge(int from, int to, long capacity)
    {
        auto c_map = boost::get(boost::edge_capacity, G);
        auto r_map = boost::get(boost::edge_reverse, G);
        const edge_desc e = boost::add_edge(from, to, G).first;
        const edge_desc rev_e = boost::add_edge(to, from, G).first;
        c_map[e] = capacity;
        c_map[rev_e] = 0; // reverse edge has no capacity!
        r_map[e] = rev_e;
        r_map[rev_e] = e;
    }
};

using namespace std;

static void runTestCase()
{
    int n, m;
    cin >> n >> m;

    graph G(n);
    edge_adder adder(G);

    // Vertices are figures
    // Edges are limbs extending into other figure
    vector<unordered_map<int, int>> edges(n);
    unordered_set<int> outgoingEdges;

    for (int i = 0; i < m; i++)
    {
        int u, v, c;
        cin >> u >> v >> c;

        edges.at(u)[v] += c;
        outgoingEdges.insert(u);
    }

    for (const int u : outgoingEdges)
    {
        for (const pair<int, int> &vc : edges.at(u))
        {
            const int v = vc.first;
            const int c = vc.second;

            adder.add_edge(u, v, c);
        }
    }

    // pick one vertex. This vertex is either in S or T
    // pick another different vertex. Since S and T can not be empty, at least one other vertex must exist that is in the other set
    // Try the flows between all such pairs, both ways (one way says the vertex is in S, other says in T)

    int minFlow = numeric_limits<int>::max();
    int source = 0;
    int target = 1;

    for (int i = 1; i < n; i++)
    {
        int outFlow = boost::push_relabel_max_flow(G, 0, i);

        if (outFlow < minFlow)
        {
            source = 0;
            target = i;
            minFlow = outFlow;
        }

        int inFlow = boost::push_relabel_max_flow(G, i, 0);

        if (inFlow < minFlow)
        {
            source = i;
            target = 0;
            minFlow = inFlow;
        }
    }

    auto rc_map = boost::get(boost::edge_residual_capacity, G);

    // Find min cut. This is the min number of outgoing capacity, i.e. minimum number of limbs that need to be cut

    // Find a min cut via maxflow
    int flow = boost::push_relabel_max_flow(G, source, target);
    std::cout << flow << "\n";

    // BFS to find vertex set S
    std::vector<int> vis(n, false); // visited flags
    std::queue<int> Q;              // BFS queue (from std:: not boost::)
    vis[source] = true;             // Mark the source as visited
    Q.push(source);
    while (!Q.empty())
    {
        const int u = Q.front();
        Q.pop();
        out_edge_it ebeg, eend;
        for (boost::tie(ebeg, eend) = boost::out_edges(u, G); ebeg != eend; ++ebeg)
        {
            const int v = boost::target(*ebeg, G);
            // Only follow edges with spare capacity
            if (rc_map[*ebeg] == 0 || vis[v])
                continue;
            vis[v] = true;
            Q.push(v);
        }
    }

    string out = "";
    int count = 0;
    // Output S
    for (int i = 0; i < n; ++i)
    {
        if (vis[i])
        {
            out.append(" " + to_string(i));
            count++;
        }
    }

    cout << count << out << "\n";
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
