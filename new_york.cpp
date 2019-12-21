//
//  new_york.cpp
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
#include <stack>
#include <string>
#include <vector>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/max_cardinality_matching.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS> graph;
typedef boost::graph_traits<graph>::vertex_descriptor vertex_desc;
typedef graph::out_edge_iterator out_edge_it;

using namespace std;

typedef struct {
    // int start;
    int maxTemp;
    int minTemp;
    int length;
} Trip;

bool dfs(const int u, const graph &G, const vector<int> &temps, const int m, const int k, Trip *trip, const vector<Trip> &foundTrips) {
    trip->length++;
    if (trip->length == 1) {
        trip->maxTemp = temps.at(u);
        trip->minTemp = temps.at(u);
    } else {
        trip->maxTemp = max(trip->maxTemp, temps.at(u));
        trip->minTemp = min(trip->minTemp, temps.at(u));
    }

    if (trip->maxTemp - trip->minTemp > k) {
        return false;
    }

    const Trip &found = foundTrips.at(u);

    if (found.length != -1) {
        if (found.length == -2) {                            // Heavy misuse of struct members incoming
            if (found.minTemp <= m - trip->length) {         // NOT actually the min temp
                trip->length = found.minTemp + trip->length; // This is all the information about how long this trip can be that we have.
                return false;
            }
        } else if (max(trip->maxTemp, found.maxTemp) - min(trip->minTemp, found.minTemp) <= k) {
            trip->maxTemp = max(trip->maxTemp, found.maxTemp);
            trip->minTemp = min(trip->minTemp, found.minTemp);
            trip->length = m;
            return true;
        }
    }

    if (trip->length == m) {
        return true;
    }

    Trip longestNo = *trip;

    out_edge_it ebeg, eend;
    for (boost::tie(ebeg, eend) = boost::out_edges(u, G); ebeg != eend; ++ebeg) {
        const int v = boost::target(*ebeg, G);

        Trip cpy = *trip;
        if (dfs(v, G, temps, m, k, &cpy, foundTrips)) {
            *trip = cpy;
            return true;
        } else if (cpy.length > longestNo.length) {
            longestNo = cpy;
        }
    }

    *trip = longestNo;

    return false;
}

void testcase() {
    int n, m, k;

    cin >> n >> m >> k;

    vector<int> temps(n);

    for (int i = 0; i < n; i++) {
        cin >> temps.at(i);
    }

    graph G(n);

    for (int i = 1; i < n; i++) {
        int u, v;
        cin >> u >> v;

        boost::add_edge(u, v, G);
    }

    Trip no;
    no.length = -1;

    vector<Trip> foundTrips(n, no);
    bool any = false;
    for (int i = 0; i < n; i++) {
        Trip trip;
        trip.length = 0;
        if (dfs(i, G, temps, m, k, &trip, foundTrips)) {
            cout << i << " ";
            any = true;
            foundTrips.at(i) = trip;
        } else {
            Trip t;
            t.length = -2;
            t.minTemp = trip.length;
            foundTrips.at(i) = t;
        }
    }

    if (!any) {
        cout << "Abort mission\n";
    } else {
        cout << "\n";
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);

    int t;
    std::cin >> t;
    for (int i = 0; i < t; ++i)
        testcase();
}