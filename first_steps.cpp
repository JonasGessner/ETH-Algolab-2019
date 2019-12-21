//
//  first_steps.cpp
//  Algorithms Lab
//
//  Created by Jonas Gessner
//  Copyright © 2019 Jonas Gessner. All rights reserved.
//

#include <iostream>
#include <vector>

// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <boost/graph/breadth_first_search.hpp>
#include <boost/graph/depth_first_search.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

using namespace std;

typedef boost::property<boost::edge_weight_t, int> EdgeWeightProperty;

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
                              boost::no_property, EdgeWeightProperty>
    weighted_graph;

typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor edge_desc;
typedef boost::graph_traits<weighted_graph>::vertex_descriptor vertex_desc;

static void runTestCase()
{
  int n;
  int m;
  cin >> n >> m;

  weighted_graph G(n);

  for (int i = 0; i < m; i++)
  {
    int u;
    int v;
    int w;
    cin >> u >> v >> w;

    boost::add_edge(u, v, EdgeWeightProperty(w), G);
  }

  std::vector<edge_desc> mst; // vector to store MST edges (not a property map!)

  boost::kruskal_minimum_spanning_tree(G, std::back_inserter(mst));

  int totalWeight = 0;
  for (std::vector<edge_desc>::iterator it = mst.begin(); it != mst.end(); ++it)
  {
    totalWeight += boost::get(boost::edge_weight, G, *it);
  }

  std::vector<int> dist_map(n);

  boost::dijkstra_shortest_paths(G, 0,
                                 boost::distance_map(boost::make_iterator_property_map(
                                     dist_map.begin(), boost::get(boost::vertex_index, G))));

  cout
      << totalWeight << " " << *max_element(dist_map.begin(), dist_map.end()) << "\n";
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
