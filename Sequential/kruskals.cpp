/* Simon Warchol, CS 205 Project, Spring 2020
Kruskal's algorithm, adapted from https://www.geeksforgeeks.org/prims-mst-for-adjacency-list-representation-greedy-algo-6/
 and https://www.geeksforgeeks.org/kruskals-minimum-spanning-tree-using-stl-in-c/*/

#include <iostream>
#include <queue>
#include <fstream>
#include <list>
#include <algorithm>

using namespace std;

// Build Graph from adjacency matrix (represented as a list)
class Graph {
    int num_vertices;
    vector<pair<int, pair<int, int> > > E;

public:
    Graph(int num_vertices);

    // Adds an edge between vertex v1 and vertex v1 w/ weight w
    void addEdge(int v1, int v2, int w);

    void kruskals();
};

//This code straight from https://www.geeksforgeeks.org/kruskals-minimum-spanning-tree-using-stl-in-c/
struct DisjointSets {
    int *parent, *rank;
    int num_vertices;

    DisjointSets(int num_vertices) {
        this->num_vertices = num_vertices;
        parent = new int[num_vertices + 1];
        rank = new int[num_vertices + 1];

        // Initialize all vertices in different sets w/ rank 0 and self as parent
        for (int i = 0; i <= num_vertices; i++) {
            rank[i] = 0;
            parent[i] = i;
        }
    }

    // Recursively find the parent of element
    int find(int v) {
        if (v != parent[v])
            parent[v] = find(parent[v]);
        return parent[v];
    }

    // Merge two sets, set the higher rank set as parent
    void merge(int x, int y) {
        x = find(x), y = find(y);
        if (rank[x] > rank[y])
            parent[y] = x;
        else
            parent[x] = y;
        if (rank[x] == rank[y])
            rank[y]++;
    }
};

Graph::Graph(int num_vertices) {
    this->num_vertices = num_vertices;
}

void Graph::addEdge(int v1, int v2, int w) {
    E.push_back(make_pair(w, make_pair(v1, v2)));
}

void Graph::kruskals() {

    // Sort Edges by weight, increasing
    sort(E.begin(), E.end());
    DisjointSets DS(num_vertices);
    vector<pair<int, pair<int, int> > >::iterator it;
    for (it = E.begin(); it != E.end(); it++) {
        int v1 = it->second.first;
        int v2 = it->second.second;
        int set_v1 = DS.find(v1);
        int set_v2 = DS.find(v2);
        if (set_v1 != set_v2) {
            cout << v1 << " - " << v2 << endl;
            DS.merge(set_v1, set_v2);
        }

    }
}


int main(int argc, char *argv[]) {
    if (argc != 2) {
        cout << "Specify an input file" << endl;
        exit(1);
    }

    ifstream file(argv[1], ifstream::binary);
    if (!file.is_open()) {
        cout << "Unable to open file " << argv[1] << endl;
        exit(2);
    }
    // Count number of IDs
    int maxId = 0;
    string id1, id2, w;
//    Find the max vertex id (exclude weights)
    while (!file.eof()) {
        file >> id1 >> id2 >> w;
        maxId = max(maxId, stoi(id1));
        maxId = max(maxId, stoi(id2));
    }
    cout << "MAX:" << maxId;
    file.clear();
    file.seekg(0);

    // initialize graph
    Graph G(maxId + 1);

    // Add edges
    string i1, i2, i3;
    while (!file.eof()) {
        file >> i1;
        file >> i2;
        file >> i3;
        G.addEdge(stoi(i1), stoi(i2), stoi(i3));
    }
    file.close();
    cout << "Beginning Kruskals MST from vertex 0 \n";
    G.kruskals();
    return 0;
}

