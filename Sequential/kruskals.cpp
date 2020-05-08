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

typedef struct {    // struct representing one edge , similar to my primm's but with weight in it as well
    int v1;
    int v2;
    int weight;
} edge;

int num_vertices;
int num_edges;
edge* E;

void addEdge(int v1, int v2, int w, int i);
void kruskals();
// This comparison of edges via https://pdfs.semanticscholar.org/14be/5e7a0feeafaf7fc9c9339dee3d8c837b115c.pdf,
// which will allow me to quicksort
int compare_edges(const void *a, const void *b) {
    edge *e1 = (edge *)a;
    edge *e2 = (edge *)b;
    if (e1->weight < e2->weight) {
        return -1;
    } else if (e1->weight > e2->weight) {
        return 1;
    } else {
        return 0; // Else they are equal 
    }
}



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



void addEdge(int v1, int v2, int w, int i) {
    edge e = { v1, v2, w};
    E[i] = e;
}

void kruskals() {

    // Sort Edges by weight, increasing
    // sort(E.begin(), E.end());
    qsort(E, num_edges, sizeof(edge), compare_edges);
    DisjointSets DS(num_vertices);
    // vector<pair<int, pair<int, int> > >::iterator it;
    for (int i = 0;i < num_edges -1 ; i++) {
        int v1 = E[i].v1;
        int v2 = E[i].v2;
        int w = E[i].weight;
        int set_v1 = DS.find(v1);
        int set_v2 = DS.find(v2);
        if (set_v1 != set_v2) {
            cout << v1 << " - " << v2 << " - " << w << endl;
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
    int nEdges = 0;
    while (!file.eof()) {
        file >> id1 >> id2 >> w;
        maxId = max(maxId, stoi(id1));
        maxId = max(maxId, stoi(id2));
        nEdges++;
    }
    cout << "MAX:" << maxId;
    file.clear();
    file.seekg(0);

    // initialize graph
    num_vertices  =  maxId +1;
    num_edges = nEdges+1;
    E = new edge[num_edges];


    // Add edges
    string i1, i2, i3;
    int i = 0;
    while (!file.eof()) {
        file >> i1;
        file >> i2;
        file >> i3;
        addEdge(stoi(i1), stoi(i2), stoi(i3), i);
        i++;
    }
    file.close();
    cout << "Beginning Kruskals MST \n";
    kruskals();
    return 0;
}

