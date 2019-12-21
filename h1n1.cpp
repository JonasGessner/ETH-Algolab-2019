//
//  h1n1.cpp
//  Algorithms Lab
//
//  Created by Jonas Gessner
//  Copyright © 2019 Jonas Gessner. All rights reserved.
//

#include <iostream>
#include <vector>
#include <unordered_set>
#include <limits>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_2<K> Vb;
typedef CGAL::Triangulation_face_base_with_info_2<long, K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb, Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K, Tds> Triangulation;
typedef Triangulation::Edge_iterator Edge_iterator;

// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <boost/graph/prim_minimum_spanning_tree.hpp>
#include <boost/graph/breadth_first_search.hpp>
#include <boost/graph/depth_first_search.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

using namespace std;

typedef boost::property<boost::edge_weight_t, long> EdgeWeightProperty;

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
                              boost::no_property, EdgeWeightProperty>
    weighted_graph;

typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor edge_desc;
typedef boost::graph_traits<weighted_graph>::vertex_descriptor vertex_desc;

using namespace std;

double floor_to_double(const K::FT &x)
{
    double a = std::floor(CGAL::to_double(x));
    while (a > x)
        a -= 1;
    while (a + 1 <= x)
        a += 1;
    return a;
}

bool runTestCase()
{
    int n;
    cin >> n;

    if (n == 0)
    {
        return false;
    }

    vector<K::Point_2> points(n);

    for (int i = 0; i < n; i++)
    {
        int x, y;
        cin >> x >> y;

        const K::Point_2 point(x, y);

        points.at(i) = point;
    }

    Triangulation t;
    t.insert(points.begin(), points.end());

    // Give every face an index
    long faceIndex = 0;

    for (auto f = t.finite_faces_begin(); f != t.finite_faces_end(); f++, faceIndex++)
    {
        f->info() = faceIndex;
    }

    // Reduce to widest path problem (https://en.wikipedia.org/wiki/Widest_path_problem)

    // "In an undirected graph, a widest path may be found as the path between the two vertices in the maximum spanning tree of the graph, and a minimax path may be found as the path between the two vertices in the minimum spanning tree."

    weighted_graph G(t.number_of_faces() + 1);
    const int infiniteVertex = 0;

    const long MAX_VAL = (numeric_limits<long>::max() / 2);

    for (auto f = t.finite_faces_begin(); f != t.finite_faces_end(); f++)
    {
        for (int i = 0; i < 3; i++)
        {
            const auto p1 = f->vertex((i + 1) % 3);
            const auto p2 = f->vertex((i + 2) % 3);

            const long width = (long)floor_to_double(CGAL::squared_distance(p1->point(), p2->point()) / 4); // Need to divide actual distance by two. This is the squared distance, so need to divide by 4 (2^2).

            const auto neighbour = f->neighbor(i);

            if (t.is_infinite(neighbour))
            {
                boost::add_edge(f->info() + 1, infiniteVertex, EdgeWeightProperty(MAX_VAL - width), G);
            }
            else if (neighbour->info() < f->info())
            {
                continue; // Neighbour has smaller face index, hence it is already connected to this face
            }
            else
            {
                boost::add_edge(f->info() + 1, neighbour->info() + 1, EdgeWeightProperty(MAX_VAL - width), G);
            }
        }
    }

    // Make max spanning tree
    // Use prim to get a useful predecessor map, and store weights to predecessors too

    vector<vertex_desc> predMap(t.number_of_faces() + 1);
    vector<long> weightToPredecessorMap(t.number_of_faces() + 1);

    // iterator property map adds indices to vertices
    boost::prim_minimum_spanning_tree(G, boost::make_iterator_property_map(predMap.begin(), boost::get(boost::vertex_index, G)),
                                      boost::root_vertex(infiniteVertex).distance_map(boost::make_iterator_property_map(weightToPredecessorMap.begin(), boost::get(boost::vertex_index, G))));

    int m;
    cin >> m;

    for (int i = 0; i < m; i++)
    {
        int x, y;
        cin >> x >> y;

        long distSquared;
        cin >> distSquared;

        const K::Point_2 point(x, y);

        const auto nearestVertex = t.nearest_vertex(point);

        const auto initialDistance = CGAL::squared_distance(nearestVertex->point(), point);

        if (distSquared <= initialDistance)
        {
            const auto initialFace = t.locate(point);

            if (t.is_infinite(initialFace))
            {
                cout << "y";
                continue;
            }

            const int initialFaceIndex = initialFace->info();

            vertex_desc current = initialFaceIndex + 1;

            while (current != infiniteVertex && (-(weightToPredecessorMap.at(current) - MAX_VAL)) >= distSquared)
            {
                current = predMap.at(current);
            }

            if (current == infiniteVertex)
            {
                cout << "y";
            }
            else
            {
                cout << "n";
            }
        }
        else
        {
            cout << "n";
        }
    }

    cout << "\n";

    return true;
}

int main(int argc, const char *argv[])
{
    ios_base::sync_with_stdio(false);

    while (runTestCase())
    {
    }

    return 0;
}
