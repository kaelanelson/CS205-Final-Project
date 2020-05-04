/* Simon Warchol, CS 205 Project, Spring 2020
Prim's algorithm, adapted from https://www.geeksforgeeks.org/prims-mst-for-adjacency-list-representation-greedy-algo-6/ */

#include <iostream>
#include <queue>
#include <fstream>
#include <list>
#include <algorithm>

using namespace std;

// Build Graph from adjacency matrix (represented as a list)
class Graph {
    int num_vertices;
    list<pair<int, int> > *A;

public:
    Graph(int num_vertices);

    // Adds an edge between vertex v1 and vertex v1 w/ weight w
    void addEdge(int v1, int v2, int w);

    void prims();

};

Graph::Graph(int num_vertices) {
    this->num_vertices = num_vertices;
    A = new list<pair<int, int> >[num_vertices];
}

void Graph::addEdge(int v1, int v2, int w) {
    A[v1].push_back(make_pair(v2, w));
    A[v2].push_back(make_pair(v1, w));
}

//Adapted from https://www.geeksforgeeks.org/prims-algorithm-using-priority_queue-stl/
void Graph::prims(int start_vertex) {
    // create a queue,
    priority_queue<pair<int, int>, vector<pair<int, int> >, greater<pair<int, int> > > Q;

    vector<int> key(num_vertices, INT_MAX);

    vector<int> parent(num_vertices, -1);
    // create visited list
    bool *visited = new bool[num_vertices];
    for (int i = 0; i < num_vertices; i++)
        visited[i] = false;

    // assign index to start at
    int s = start_vertex;

    // mark s as visited and put s into Q
    visited[s] = true;
    Q.push(make_pair(0, s));

    // list<int>::iterator i;
    while (!Q.empty()) {
        // pop off head of Q
        int v1 = Q.top().second; // second will be the source
        Q.pop();
        visited[v1] = true;

        // mark and enqueue all unvisited neighbor nodes of s
        list<pair<int, int> >::iterator i;
        for (i = A[v1].begin(); i != A[v1].end(); ++i) {
            int v2 = (*i).first;
            int w = (*i).second;
            // if this weight is smaller and we still haven't visited the vertex
            if (!visited[v2] && key[v2] > w) {
                key[v2] = w;
                Q.push(make_pair(key[v2], v2));
                parent[v2] = v1;
            }
        }
    }
    for (int i = 1; i < num_vertices; ++i)
        printf("%d - %d\n", parent[i], i);
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
    cout << "Beginning Prims MST from vertex 0 \n";
    G.prims(0);
    return 0;
}

