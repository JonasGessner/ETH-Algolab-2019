//
//  carsharing.cpp
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

using namespace std;

// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/cycle_canceling.hpp>
#include <boost/graph/find_flow_cost.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>

// Graph Type with nested interior edge properties for Cost Flow Algorithms
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
                              boost::property<boost::edge_capacity_t, long,
                                              boost::property<boost::edge_residual_capacity_t, long,
                                                              boost::property<boost::edge_reverse_t, traits::edge_descriptor,
                                                                              boost::property<boost::edge_weight_t, long>>>>>
    graph; // new! weightmap corresponds to costs

typedef boost::graph_traits<graph>::edge_descriptor edge_desc;
typedef boost::graph_traits<graph>::out_edge_iterator out_edge_it; // Iterator

// Custom edge adder class
class edge_adder {
    graph &G;

public:
    explicit edge_adder(graph &G) : G(G) {}
    void add_edge(int from, int to, long capacity, long cost) {
        auto c_map = boost::get(boost::edge_capacity, G);
        auto r_map = boost::get(boost::edge_reverse, G);
        auto w_map = boost::get(boost::edge_weight, G); // new!
        const edge_desc e = boost::add_edge(from, to, G).first;
        const edge_desc rev_e = boost::add_edge(to, from, G).first;
        c_map[e] = capacity;
        c_map[rev_e] = 0; // reverse edge has no capacity!
        r_map[e] = rev_e;
        r_map[rev_e] = e;
        w_map[e] = cost;      // new assign cost
        w_map[rev_e] = -cost; // new negative cost
    }
};

typedef struct {
    int start;
    int end;

    int startT;
    int endT;

    int profit;

    int startVertex;
    int endVertex;
} Trip;

typedef struct {
    int startTime;
    int vertex;
} StationVertex;

void testcase() {
    int n, s;

    cin >> n >> s;

    const long maxT = 100000;
    const long bigAssCapacity = 100 * 100;

    vector<int> initialCars(s);

    long flow = 0;

    vector<vector<StationVertex>> stationVertices(s); // [stationIndex: (timestamp, vertexIndex)] (for sorting)
    vector<unordered_map<int, int>> vertices(s);      // [stationIndex: [timestamp: vertexIndex]] (for fast access for a specific index)
    int vertexIndex = 0;

    const auto registerVertex = [&](const int station, const int time) -> int {
        vertices.at(station)[time] = vertexIndex;
        stationVertices.at(station).push_back({time, vertexIndex});
        vertexIndex++;
        return vertexIndex - 1;
    };

    for (int i = 0; i < s; i++) {
        int capacity;
        cin >> capacity;

        initialCars.at(i) = capacity;
        flow += capacity;

        registerVertex(i, 0);
        registerVertex(i, maxT);
    }

    // Achieve constant flow (equals to number of all cars). Then achieve predictable max cost (flow * maxT * 100). The profit is the result of max cost - actual cost

    vector<Trip> trips;
    trips.reserve(n);

    for (int i = 0; i < n; i++) {
        int startStation, endStation, startTime, endTime, profit;
        cin >> startStation >> endStation >> startTime >> endTime >> profit;

        // Did a matlab programmer write the exercise?
        startStation--;
        endStation--;

        Trip t;
        t.start = startStation;
        t.end = endStation;
        t.startT = startTime;
        t.endT = endTime;
        t.profit = profit;

        const auto foundStart = vertices.at(startStation).find(startTime);

        if (foundStart != vertices.at(startStation).end()) {
            t.startVertex = foundStart->second;
        } else {
            t.startVertex = registerVertex(startStation, startTime);
        }

        const auto foundEnd = vertices.at(endStation).find(endTime);

        if (foundEnd != vertices.at(endStation).end()) {
            t.endVertex = foundEnd->second;
        } else {
            t.endVertex = registerVertex(endStation, endTime);
        }

        trips.push_back(t);
    }

    graph G(vertexIndex);
    edge_adder adder(G);

    const auto source = boost::add_vertex(G);
    const auto target = boost::add_vertex(G);

    for (int i = 0; i < s; i++) { // For each station, connect all vertices sequentially from source to target. Source limits the capacity according to the initial car count.
        sort(stationVertices.at(i).begin(), stationVertices.at(i).end(), [](const StationVertex &lhs, const StationVertex &rhs) {
            return lhs.startTime < rhs.startTime;
        });

        StationVertex current = stationVertices.at(i).at(0);
        adder.add_edge(source, current.vertex, initialCars.at(i), 0);

        for (int j = 1; j < stationVertices.at(i).size(); j++) {
            StationVertex next = stationVertices.at(i).at(j);

            adder.add_edge(current.vertex, next.vertex, bigAssCapacity, 100 * (next.startTime - current.startTime));

            current = next;
        }

        adder.add_edge(current.vertex, target, bigAssCapacity, 0);
    }

    for (const Trip &trip : trips) { // Connect trip vertices
        adder.add_edge(trip.startVertex, trip.endVertex, 1, 100 * (trip.endT - trip.startT) - trip.profit);
    }

    boost::successive_shortest_path_nonnegative_weights(G, source, target);
    long cost2 = boost::find_flow_cost(G);

    cout << maxT * 100 * flow - cost2 << "\n";
}

int main() {
    std::ios_base::sync_with_stdio(false);

    int t;
    std::cin >> t;
    for (int i = 0; i < t; ++i)
        testcase();
}
