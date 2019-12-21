//
//  sith.cpp
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
#include <string>
#include <vector>

#include <boost/graph/adjacency_list.hpp>

#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Triangulation_face_base_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <boost/pending/disjoint_sets.hpp>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_with_info_2<int, K> Vb;
typedef CGAL::Triangulation_face_base_2<K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb, Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K, Tds> Triangulation;

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS> graph;
typedef boost::graph_traits<graph>::vertex_descriptor vertex_desc;

using namespace std;

void testcase() {
    int n, r;

    cin >> n >> r;

    K::FT rSq = K::FT(r) * K::FT(r);

    vector<pair<K::Point_2, int>> pts;
    pts.reserve(n);

    for (int i = 0; i < n; i++) {
        int x, y;
        cin >> x >> y;

        pts.push_back({K::Point_2(x, y), n - i - 1});
    }

    const auto check = [&](const int size) {
        Triangulation t;
        t.insert(pts.begin() + (n - size), pts.end());

        boost::disjoint_sets_with_storage<> uf(size);

        for (Triangulation::Edge_iterator e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e) {
            if (t.segment(*e).squared_length() <= rSq) {
                Triangulation::Vertex_handle v1 = e->first->vertex((e->second + 1) % 3);
                Triangulation::Vertex_handle v2 = e->first->vertex((e->second + 2) % 3);

                uf.link(v1->info(), v2->info());
            }
        }

        int maxSize = 0;

        vector<int> sizes(size);
        for (int j = 0; j < size; j++) {
            int s = uf.find_set(j);
            sizes.at(s)++;
            if (sizes.at(s) > maxSize) {
                maxSize = sizes.at(s);
            }
        }

        if (maxSize > (n - size - 1)) {
            return true;
        }

        return false;
    };

    int l = (n / 2);
    int rr = n - 1;

    while (l < rr) {
        int mid = (l + rr) / 2;

        bool ok = check(mid);

        if (!ok) {
            l = mid + 1;
        } else {
            rr = mid;
        }
    }

    cout << (n - l) << endl;
}

int main() {
    std::ios_base::sync_with_stdio(false);

    int t;
    std::cin >> t;
    for (int i = 0; i < t; ++i)
        testcase();
}
