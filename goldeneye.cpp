//
//  goldeneye.cpp
//  Algorithms Lab
//
//  Created by Jonas Gessner
//  Copyright © 2019 Jonas Gessner. All rights reserved.
//

#include <iostream>
#include <vector>

#include <algorithm>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

#include <CGAL/Triangulation_face_base_with_info_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_with_info_2<int, K> Vb;
typedef CGAL::Triangulation_face_base_2<K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb, Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K, Tds> Triangulation;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;

typedef Triangulation::Edge_iterator Edge_iterator;

typedef K::Point_2 Point;
typedef K::Segment_2 Segment;
typedef K::Circle_2 Circle;

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/strong_components.hpp>
#include <boost/graph/connected_components.hpp>
#include <boost/graph/incremental_components.hpp>
#include <boost/pending/disjoint_sets.hpp>

typedef boost::property<boost::edge_weight_t, long> EdgeWeightProperty;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
                              boost::no_property, EdgeWeightProperty>
    weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor edge_desc;
typedef boost::graph_traits<weighted_graph>::out_edge_iterator out_edge_it;
typedef boost::graph_traits<weighted_graph>::vertex_descriptor vertex_desc;
typedef boost::graph_traits<weighted_graph>::vertices_size_type vertex_index;

using namespace std;

typedef struct
{
    int u;
    int v;
    long dist;
} Edge;

static void runTestCase()
{
    int n, m;
    cin >> n >> m;

    long long p;
    cin >> p;

    vector<Point> jammers;
    jammers.reserve(n);

    for (int i = 0; i < n; i++)
    {
        int x, y;
        cin >> x >> y;

        jammers.push_back(Point(x, y));
    }

    Triangulation t;
    t.insert(jammers.begin(), jammers.end());

    int vertexIndex = 0;
    for (auto v = t.finite_vertices_begin(); v != t.finite_vertices_end(); v++, vertexIndex++)
    {
        v->info() = vertexIndex;
    }

    weighted_graph G(n);

    for (auto v = t.finite_vertices_begin(); v != t.finite_vertices_end(); v++)
    {
        auto incident = t.incident_vertices(v);

        do
        {
            if (t.is_infinite(incident))
            {
                continue;
            }

            if (incident->info() < v->info())
            {
                continue; // Neighbour has smaller index, is already connected to self
            }

            const auto distToIncident = (long)CGAL::to_double(CGAL::squared_distance(v->point(), incident->point()));

            boost::add_edge(v->info(), incident->info(), EdgeWeightProperty(distToIncident), G);
        } while (++incident != t.incident_vertices(v));
    }

    const weight_map edgeWeights = boost::get(boost::edge_weight, G);

    std::vector<edge_desc> mst; // vector to store MST edges (not a property map!)
    boost::kruskal_minimum_spanning_tree(G, std::back_inserter(mst));

    weighted_graph MSTInitial(n);
    weighted_graph MSTFitAll(n);
    weighted_graph MSTFitInitial(n);

    vector<Edge> longerEdgesInitial;

    for (std::vector<edge_desc>::iterator it = mst.begin(); it != mst.end(); ++it)
    {
        const auto bro = edgeWeights[*it];

        Edge e;
        e.u = boost::source(*it, G);
        e.v = boost::target(*it, G);
        e.dist = bro;

        longerEdgesInitial.push_back(e);

        if (bro <= p)
        {
            boost::add_edge(boost::source(*it, G), boost::target(*it, G), EdgeWeightProperty(bro), MSTInitial);
        }
    }

    sort(longerEdgesInitial.begin(), longerEdgesInitial.end(), [&](const Edge &lhs, const Edge &rhs) { return lhs.dist > rhs.dist; });
    vector<Edge> longerEdgesAll(longerEdgesInitial);

    // scc_map[i]: index of SCC containing i-th vertex
    std::vector<int> scc_map_initial(n); // exterior property map
    // nscc: total number of SCCs
    int nscc = boost::connected_components(MSTInitial, boost::make_iterator_property_map(scc_map_initial.begin(), boost::get(boost::vertex_index, MSTInitial)));

    std::vector<vertex_index> rankInitial(n);
    std::vector<vertex_desc> parentInitial(n);

    typedef vertex_index *Rank;
    typedef vertex_desc *Parent;

    boost::disjoint_sets<Rank, Parent> dsInitial(&rankInitial[0], &parentInitial[0]);

    boost::initialize_incremental_components(MSTFitInitial, dsInitial);
    boost::incremental_components(MSTFitInitial, dsInitial);

    std::vector<vertex_index> rankAll(n);
    std::vector<vertex_desc> parentAll(n);

    boost::disjoint_sets<Rank, Parent> dsAll(&rankAll[0], &parentAll[0]);

    boost::initialize_incremental_components(MSTFitAll, dsAll);
    boost::incremental_components(MSTFitAll, dsAll);

    long neededForAll = 0;
    long neededForFits = 0;

    for (int i = 0; i < m; i++)
    {
        int x1, y1;
        cin >> x1 >> y1;

        int x2, y2;
        cin >> x2 >> y2;

        const Point p1(x1, y1);
        const Point p2(x2, y2);

        const auto startVertex = t.nearest_vertex(p1);
        const auto endVertex = t.nearest_vertex(p2);

        long neededDist = CGAL::to_double(CGAL::squared_distance(startVertex->point(), p1)) * 4;
        neededDist = max(neededDist, (long)CGAL::to_double(CGAL::squared_distance(endVertex->point(), p2) * 4));

        if (neededDist <= p && scc_map_initial[startVertex->info()] == scc_map_initial[endVertex->info()])
        {
            while (dsInitial.find_set(startVertex->info()) != dsInitial.find_set(endVertex->info()))
            {
                long aa;
                do
                {
                    const Edge next = longerEdgesInitial.back();
                    longerEdgesInitial.pop_back();
                    aa = next.dist;
                    dsInitial.union_set(next.u, next.v);
                } while (!longerEdgesInitial.empty() && longerEdgesInitial.back().dist == aa);

                neededForFits = max(neededForFits, aa);
            }

            neededForFits = max(neededForFits, neededDist);
            neededForAll = max(neededForAll, neededForFits);

            cout << "y";
        }
        else
        {
            while (dsAll.find_set(startVertex->info()) != dsAll.find_set(endVertex->info()))
            {
                long aa;
                do
                {
                    const Edge next = longerEdgesAll.back();
                    longerEdgesAll.pop_back();
                    aa = next.dist;
                    dsAll.union_set(next.u, next.v);
                } while (!longerEdgesAll.empty() && longerEdgesAll.back().dist == aa);

                neededForAll = max(neededForAll, aa);
            }

            neededForAll = max(neededForAll, neededDist);

            cout << "n";
        }
    }

    cout << "\n";
    cout << neededForAll << "\n";
    cout << neededForFits << "\n";
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
