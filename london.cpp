//
//  london.cpp
//  Algorithms Lab
//
//  Created by Jonas Gessner
//  Copyright © 2019 Jonas Gessner. All rights reserved.
//

#include <iostream>
#include <vector>
#include <unordered_set>
#include <unordered_map>

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
    // make a vertex for every letter between a and z
    // add an edge to every letter edge with capacity = number of occurrences in target word for every letter
    // pair up corresponding front and back letters as vertex. Only respect pairs that contain relevant letters. Connect the letter vertices of the letters of this letter pair to this pair. Only create 1 vertex per unique pair and make the incoming and outgoing edges per pair have the capacity equal to the number of occurrences of this letter pair.
    // connect each letter pair vertices with capacity equal to the number of occurrences of this letter pair to the target
    int h, w;
    cin >> h >> w;
    string word;
    cin >> word;

    uint32_t wordLetters = 0;
    vector<int> letterFrequencies(26, 0);

    for (int i = 0; i < word.size(); i++)
    {
        const char letter = word.at(i);
        int letterIndex = letter - 'A';

        wordLetters |= 1 << letterIndex;
        letterFrequencies.at(letterIndex)++;
    }

    vector<vector<int>> frontLetters(h, vector<int>(w));
    vector<vector<int>> backLetters(h, vector<int>(w));

    for (int i = 0; i < h; i++)
    { // front page
        string line;
        cin >> line;
        int j = 0;
        for (const char c : line)
        {
            int charIndex = c - 'A';
            frontLetters.at(i).at(j) = charIndex;
            
            j++;
        }
    }

    for (int i = 0; i < h; i++)
    { // back page
        string line;
        cin >> line;
        int j = 0;
        for (const char c : line)
        {
            int charIndex = c - 'A';
            backLetters.at(i).at((w - j - 1)) = charIndex;
            
            j++;
        }
    }

    graph G(2 + 26); // source and target (0 and 1) and 1 vertex per letter (should be 26).
    edge_adder adder(G);

    const int source = 0;
    const int target = 1;

    const int letterVertexOffset = 2;
    const int letterPairVertexOffset = 26 + letterVertexOffset;

    unordered_set<uint64_t> letterPairs;
    unordered_map<uint64_t, int> pairCount;

    int pairVertices = 0;

    for (int i = 0; i < h; i++)
    {
        for (int j = 0; j < w; j++)
        {
            int frontLetter = frontLetters.at(i).at(j);
            int backLetter = backLetters.at(i).at(j);

            bool frontIsInWord = ((wordLetters >> frontLetter) & 1);
            bool backIsInWord = ((wordLetters >> backLetter) & 1);

            uint64_t pairMap = 0;

            if (frontIsInWord)
            {
                pairMap |= (1ULL << frontLetter);
            }

            if (backIsInWord && frontLetter != backLetter)
            {
                pairMap |= (1ULL << (backLetter + 26));
            }

            if (frontIsInWord || backIsInWord)
            {
                letterPairs.insert(pairMap);
                pairCount[pairMap] = (pairCount.count(pairMap) != 0) ? (pairCount.at(pairMap) + 1) : 1;
            }
        }
    }

    int pairVertexIndex = letterPairVertexOffset;
    for (const uint64_t &letterMap : letterPairs)
    {
        uint64_t shift = letterMap;
        int count = pairCount.at(letterMap);

        for (int i = 0; i < (2 * 26) && shift > 0; i++, (shift = (shift >> 1)))
        {
            if (shift & 1)
            {
                int letter = (i > 25) ? (i - 26) : i;
                adder.add_edge(letterVertexOffset + letter, pairVertexIndex, count);
            }
        }

        adder.add_edge(pairVertexIndex, target, count);
        pairVertexIndex++;
    }

    for (int i = 0; i < letterFrequencies.size(); i++)
    {
        int val = letterFrequencies.at(i);

        if (val > 0)
        {
            adder.add_edge(source, letterVertexOffset + i, val); // Add the incoming capacity needed for this letter
        }
    }

    int flow = boost::push_relabel_max_flow(G, source, target);

    if (flow == word.size())
    {
        cout << "Yes\n";
    }
    else
    {
        cout << "No\n";
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
