//
//  cointossing.cpp
//  Algorithms Lab
//
//  Created by Jonas Gessner
//  Copyright © 2019 Jonas Gessner. All rights reserved.
//

#include <iostream>
#include <vector>

// BGL include #include <boost/graph/adjacency_list.hpp>
#include <boost/graph/adjacency_list.hpp>
// BGL flow include *NEW* #include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>

typedef enum
{
    RoundResultAWon = 1,
    RoundResultBWon = 2,
    RoundResultUnknown = 0
} RoundResult;

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
    int n, m;
    cin >> n >> m;

    vector<pair<int, int>> undecdidedGames; // Pairs of (a, b) of games between a and b where the result is unknown.
    vector<int> accountedScores(n, 0);      // The number of games each player definitely already won.

    for (int i = 0; i < m; i++)
    {
        int a, b, x;
        cin >> a >> b >> x;
        RoundResult res = (RoundResult)x;
        switch (res)
        {
        case RoundResultAWon:
            accountedScores.at(a)++;
            break;
        case RoundResultBWon:
            accountedScores.at(b)++;
            break;
        case RoundResultUnknown:
            undecdidedGames.push_back({a, b});
            break;
        }
    }

    graph G(n + undecdidedGames.size());
    edge_adder adder(G);

    const auto source = boost::add_vertex(G);
    const auto target = boost::add_vertex(G);

    long requiredMaxFlow = 0;

    bool nope = false;
    for (int i = 0; i < n; i++)
    {
        int score;
        cin >> score;

        int unaccountedScore = score - accountedScores.at(i);
        if (unaccountedScore < 0)
        {
            nope = true;
        }
        else if (unaccountedScore > 0)
        {
            requiredMaxFlow += (long)unaccountedScore;
            adder.add_edge(source, i, unaccountedScore);
        }
    }

    if (nope || requiredMaxFlow != undecdidedGames.size())
    {
        cout << "no\n";
        return;
    }

    for (int i = 0; i < undecdidedGames.size(); i++)
    {
        const pair<int, int> players = undecdidedGames.at(i);

        int gameVertex = i + n;

        adder.add_edge(players.first, gameVertex, 1);
        adder.add_edge(players.second, gameVertex, 1);

        adder.add_edge(gameVertex, target, 1);
    }

    long flow = boost::push_relabel_max_flow(G, source, target);

    if (flow == requiredMaxFlow)
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
