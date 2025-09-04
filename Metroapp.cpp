#include <bits/stdc++.h>
using namespace std;

struct Edge {
    int to;
    double time;   // effective travel time (mins)
    double dist;   // physical distance (km)
};

struct State {
    int node;
    double time, dist;
    bool operator>(const State &other) const {
        if (fabs(time - other.time) < 1e-9) return dist > other.dist;  // distance as the second priority if time is same
        return time > other.time;    // time is the first priority
    }
};

vector<Edge> graph[100005];

// ---------- Constraint Models ----------
double roadTime(double distance, double avgSpeed, int numSignals, double signalDelay) {
    double travel = (distance / avgSpeed) * 60.0;  // hr → min
    return travel + numSignals * signalDelay;
}

double metroTime(double distance, double metroSpeed, double avgWait) {
    double travel = (distance / metroSpeed) * 60.0;
    return travel + avgWait;
}

// ---------- Dijkstra ----------
pair<double, double> dijkstra(int src, int dest, int n) {
    vector<double> minTime(n, 1e18);
    vector<double> minDist(n, 1e18);

    priority_queue<State, vector<State>, greater<State>> pq;
    pq.push({src, 0, 0});
    minTime[src] = 0;
    minDist[src] = 0;

    while (!pq.empty()) {
        auto cur = pq.top(); pq.pop();
        int u = cur.node;

        if (u == dest) return {cur.time, cur.dist};

        for (auto &e : graph[u]) {
            double newTime = cur.time + e.time;
            double newDist = cur.dist + e.dist;

            if (newTime < minTime[e.to] ||
               (fabs(newTime - minTime[e.to]) < 1e-9 && newDist < minDist[e.to])) {
                minTime[e.to] = newTime;
                minDist[e.to] = newDist;
                pq.push({e.to, newTime, newDist});
            }
        }
    }
    return {-1, -1}; // no path found
}

// ---------- Main ----------
int main() {
    int n = 5; // number of nodes
    int src = 0, dest = 4;

    // Road edge: 5 km, 30 km/h, 2 signals with 1.5 min avg delay
    double t1 = roadTime(5.0, 30.0, 2, 1.5);
    graph[0].push_back({1, t1, 5.0});

    // Road edge: 12 km, 40 km/h, 1 signal of 2 mins
    double t2 = roadTime(12.0, 40.0, 1, 2.0);
    graph[1].push_back({2, t2, 12.0});

    // Metro edge: 4 km, 40 km/h, avg wait 3 mins
    double t3 = metroTime(4.0, 40.0, 3.0);
    graph[0].push_back({3, t3, 4.0});

    // Metro edge: 10 km, 50 km/h, avg wait 4 mins
    double t4 = metroTime(10.0, 50.0, 4.0);
    graph[3].push_back({4, t4, 10.0});

    // Road edge: 2 km, 20 km/h, 0 signals
    double t5 = roadTime(2.0, 20.0, 0, 0.0);
    graph[2].push_back({4, t5, 2.0});

    auto ans = dijkstra(src, dest, n);

    if (ans.first < 0) cout << "No path found\n";
    else cout << "Minimum Time = " << ans.first 
              << " mins, Distance = " << ans.second << " km\n";

    return 0;
}
