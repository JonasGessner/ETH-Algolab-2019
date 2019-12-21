//
//  clues.cpp
//  Algorithms Lab
//
//  Created by Jonas Gessner
//  Copyright © 2019 Jonas Gessner. All rights reserved.
//

#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <limits>
#include <queue>
#include <string>
#include <vector>

#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_face_base_2<K> Fb;
typedef CGAL::Triangulation_vertex_base_with_info_2<int, K> Vb;
typedef CGAL::Triangulation_data_structure_2<Vb, Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K, Tds> Triangulation;

typedef Triangulation::Edge_iterator Edge_iterator;
typedef Triangulation::Vertex_iterator Vertex_iterator;

typedef K::Point_2 Point;

// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/connected_components.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS>
    graph;
typedef boost::graph_traits<graph>::vertex_descriptor vertex_desc;
typedef boost::graph_traits<graph>::edge_descriptor edge_desc;
typedef graph::out_edge_iterator out_edge_it;

using namespace std;

typedef enum {
    ColorNone = 2,
    ColorA = 0,
    ColorB = 1
} Color;

void testcase() {
    int n, m, r;
    cin >> n >> m >> r;

    K::FT rSq = K::FT(r) * K::FT(r);

    vector<Point> points;
    points.reserve(n);

    for (int i = 0; i < n; i++) {
        int x, y;
        cin >> x >> y;
        points.push_back(Point(x, y));
    }

    Triangulation t;
    t.insert(points.begin(), points.end());

    int vertexIndex = 0;
    for (Vertex_iterator v = t.finite_vertices_begin();
         v != t.finite_vertices_end(); ++v, vertexIndex++) {
        v->info() = vertexIndex;
        points.at(vertexIndex) = v->point();
    }

    graph G(n);

    for (Edge_iterator e = t.finite_edges_begin(); e != t.finite_edges_end();
         ++e) {
        Triangulation::Vertex_handle v1 = e->first->vertex((e->second + 1) % 3);
        Triangulation::Vertex_handle v2 = e->first->vertex((e->second + 2) % 3);

        if (CGAL::squared_distance(v1->point(), v2->point()) <= rSq) {
            const int v1Index = v1->info();
            const int v2Index = v2->info();

            boost::add_edge(v1Index, v2Index, G);
        }
    }

    bool interference = false;

    // DFS to find vertex set S
    std::vector<int> vis(n, false); // visited flags

    // ONLY WORKS FOR FOR DFS, NOT BFS!! (?)
    std::stack<pair<int, Color>> Q;

    // Do DFS 2-coloring
    vector<Color> colors(n, ColorNone);
    vector<Point> aVertices;
    vector<Point> bVertices;
    vector<int> cc(n);

    int component = 0;

    for (int i = 0; i < n; i++, component++) {
        if (vis[i]) {
            continue;
        }

        Q.push({i, ColorA});
        aVertices.push_back(points.at(i));
        colors.at(i) = ColorA;
        cc.at(i) = component;
        vis[i] = true;

        while (!Q.empty()) {
            const auto u = Q.top();
            Q.pop();

            Color nextColor = u.second == ColorA ? ColorB : ColorA;

            out_edge_it ebeg, eend;
            for (boost::tie(ebeg, eend) = boost::out_edges(u.first, G); ebeg != eend;
                 ++ebeg) {
                const int v = boost::target(*ebeg, G);
                if (vis[v]) {
                    if (colors.at(v) != nextColor) {
                        interference = true;
                    }
                    continue;
                }

                if (nextColor == ColorA) {
                    aVertices.push_back(points.at(v));
                } else {
                    bVertices.push_back(points.at(v));
                }

                colors.at(v) = nextColor;
                cc.at(v) = component;
                vis[v] = true;
                Q.push({v, nextColor});
            }

            if (interference) {
                break;
            }
        }

        if (interference) {
            break;
        }
    }

    // Chek if 2-coloring is valid
    Triangulation aTriangulation;
    aTriangulation.insert(aVertices.begin(), aVertices.end());

    for (Edge_iterator e = aTriangulation.finite_edges_begin();
         e != aTriangulation.finite_edges_end(); ++e) {
        if (aTriangulation.segment(*e).squared_length() <= rSq) {
            interference = true;
            break;
        }
    }

    Triangulation bTriangulation;
    bTriangulation.insert(bVertices.begin(), bVertices.end());

    for (Edge_iterator e = bTriangulation.finite_edges_begin();
         !interference && e != bTriangulation.finite_edges_end(); ++e) {
        if (bTriangulation.segment(*e).squared_length() <= rSq) {
            interference = true;
            break;
        }
    }

    // vector<int> cc(n);
    // boost::connected_components(G,
    // boost::make_iterator_property_map(cc.begin(),
    // boost::get(boost::vertex_index, G)));

    for (int i = 0; i < m; i++) {
        int x, y;
        cin >> x >> y;
        Point p1(x, y);

        cin >> x >> y;
        Point p2(x, y);

        if (interference) {
            cout << "n";
            continue;
        }

        const Triangulation::Vertex_handle v1 = t.nearest_vertex(p1);
        const Triangulation::Vertex_handle v2 = t.nearest_vertex(p2);

        const auto distDirect = CGAL::squared_distance(p1, p2);

        if (distDirect <= rSq) {
            cout << "y";
            continue;
        } else if (cc.at(v1->info()) == cc.at(v2->info())) {
            const auto dist1 = CGAL::squared_distance(v1->point(), p1);
            const auto dist2 = CGAL::squared_distance(v2->point(), p2);

            if (dist1 <= rSq && dist2 <= rSq) {
                cout << "y";
                continue;
            }
        }

        cout << "n";
    }

    cout << "\n";
}

int main() {
    std::ios_base::sync_with_stdio(false);

    int t;
    std::cin >> t;
    for (int i = 0; i < t; ++i)
        testcase();
}
