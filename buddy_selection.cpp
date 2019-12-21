//
//  buddy_selection.cpp
//  Algorithms Lab
//
//  Created by Jonas Gessner
//  Copyright © 2019 Jonas Gessner. All rights reserved.
//

#include <iostream>
#include <unordered_set>
#include <vector>

// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/max_cardinality_matching.hpp>

using namespace std;

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS> graph;
typedef boost::graph_traits<graph>::vertex_descriptor vertex_desc;

int maximum_matching_size(const graph &G)
{
    int n = boost::num_vertices(G);
    std::vector<vertex_desc> mate_map(n); // exterior property map
    const vertex_desc NULL_VERTEX = boost::graph_traits<graph>::null_vertex();

    boost::edmonds_maximum_cardinality_matching(G,
                                                boost::make_iterator_property_map(mate_map.begin(), boost::get(boost::vertex_index, G)));
    int matching_size = boost::matching_size(G,
                                             boost::make_iterator_property_map(mate_map.begin(), boost::get(boost::vertex_index, G)));

    return matching_size;
}

static void runTestCase()
{
    int n, c, f;
    cin >> n >> c >> f;

    vector<unordered_set<string>> characteristics(n);

    for (int i = 0; i < n; i++)
    {
        characteristics.at(i).reserve(c);

        for (int j = 0; j < c; j++)
        {
            string characteristic;
            cin >> characteristic;

            characteristics.at(i).insert(characteristic);
        }
    }

    graph G(n);

    vector<string> intersectionResult(c);

    for (int i = 0; i < (n - 1); i++)
    {
        for (int j = (i + 1); j < n; j++)
        {
            int count = 0;
            for (string element : characteristics.at(i))
            {
                if (characteristics.at(j).find(element) != characteristics.at(j).end())
                {
                    count++;
                }
            }
            if (count > f)
            {
                boost::add_edge(i, j, G);
            }
        }
    }

    if (maximum_matching_size(G) == (n / 2))
    {
        cout << "not optimal\n";
    }
    else
    {
        cout << "optimal\n";
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
