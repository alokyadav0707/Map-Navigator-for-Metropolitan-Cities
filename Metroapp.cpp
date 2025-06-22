#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <climits>
using namespace std;

// Structure to represent a node in the priority queue
struct Node {
    int vertex;
    int cost;
    int time;

    bool operator<(const Node& other) const {
        return cost > other.cost; // Min-heap based on cost
    }
};

class MapNavigator {
private:
    int numVertices;
    vector<vector<pair<int, pair<int, int>>>> adjList; // {neighbor, {distance, time}}

public:
    MapNavigator(int vertices) : numVertices(vertices) {
        adjList.resize(vertices);
    }

    void addEdge(int u, int v, int distance, int time) {
        adjList[u].emplace_back(v, make_pair(distance, time));
        adjList[v].emplace_back(u, make_pair(distance, time)); // For undirected graph
    }

    void findOptimalRoute(int start, int end) {
        priority_queue<Node> pq;
        vector<int> distance(numVertices, INT_MAX);
        vector<int> travelTime(numVertices, INT_MAX);
        vector<int> parent(numVertices, -1);

        distance[start] = 0;
        travelTime[start] = 0;
        pq.push({start, 0, 0});

        while (!pq.empty()) {
            Node current = pq.top();
            pq.pop();

            int u = current.vertex;
            for (const auto& neighbor : adjList[u]) {
                int v = neighbor.first;
                int weight = neighbor.second.first;
                int time = neighbor.second.second;

                if (distance[u] + weight < distance[v] || 
                    (distance[u] + weight == distance[v] && travelTime[u] + time < travelTime[v])) {

                    distance[v] = distance[u] + weight;
                    travelTime[v] = travelTime[u] + time;
                    parent[v] = u;
                    pq.push({v, distance[v], travelTime[v]});
                }
            }
        }

        if (distance[end] == INT_MAX) {
            cout << "No route exists from " << start << " to " << end << ".\n";
        } else {
            cout << "Optimal Route from " << start << " to " << end << ":\n";
            printPath(parent, end);
            cout << "\nTotal Distance: " << distance[end] << " units\n";
            cout << "Total Time: " << travelTime[end] << " minutes\n";
        }
    }

    void printPath(const vector<int>& parent, int vertex) {
        if (parent[vertex] == -1) {
            cout << vertex;
            return;
        }
        printPath(parent, parent[vertex]);
        cout << " -> " << vertex;
    }
};

int main() {
    int vertices, edges;
    cout << "Enter number of intersections (vertices) and roads (edges): ";
    cin >> vertices >> edges;

    MapNavigator navigator(vertices);

    cout << "Enter the roads as (start, end, distance, time):\n";
    for (int i = 0; i < edges; ++i) {
        int u, v, distance, time;
        cin >> u >> v >> distance >> time;
        navigator.addEdge(u, v, distance, time);
    }

    int start, end;
    cout << "Enter starting and ending intersections: ";
    cin >> start >> end;

    navigator.findOptimalRoute(start, end);

    return 0;
}
