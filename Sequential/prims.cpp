/* Simon Warchol, CS 205 Project, Spring 2020
Prim's algorithm, adapted from https://www.geeksforgeeks.org/prims-mst-for-adjacency-list-representation-greedy-algo-6/ */

#include <iostream>
#include <queue>
#include <fstream>
#include <list>
#include <algorithm>
#include <array>
#include <cstdio>
#include <ctime>


using namespace std;

// Build Graph from adjacency matrix (represented as a list)

int num_vertices;
int **A;

void addEdge(int v1, int v2, int w);

void prims(int s);


void addEdge(int v1, int v2, int w) {
    A[v1][v2] = w;
    A[v2][v1] = w;
}

// Adapted from https://www.programiz.com/dsa/prim-algorithm
void prims(int s) {

    int numEdges = 0; //init
    bool *selected = new bool[num_vertices];
    for (int v = 0; v < num_vertices; v++) {
        selected[v] = false;
    }
    // choose sth vertex and make it true
    selected[s] = true;

    int row, col;
    cout << "Edge" << " : " << "Weight\n";
    while (numEdges < num_vertices - 1) {
        int min = INT32_MAX;
        row = 0;
        col = 0;
        for (int i = 0; i < num_vertices; i++) {
            if (selected[i]) {
                for (int j = 0; j < num_vertices; j++) {
                    if (!selected[j] && A[i][j] != 0) { // not in selected and there is an edge
                        if (min > A[i][j]) {
                            min = A[i][j];
                            row = i;
                            col = j;
                        }

                    }
                }
            }
        }
        cout << row << " - " << col << " :  " << A[row][col] << "\n";
        selected[col] = true;
        numEdges++;
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
    num_vertices = maxId + 1;
    A = new int *[num_vertices];

//  Initialize an adjacency matrix
    for (int i = 0; i < num_vertices; i++) {
        A[i] = new int[num_vertices];
        for (int j = 0; j < num_vertices; j++)
            A[i][j] = 0;
    }
    file.seekg(0);
    // Add edges
    string i1, i2, i3;
    while (!file.eof()) {
        file >> i1;
        file >> i2;
        file >> i3;
        addEdge(stoi(i1), stoi(i2), stoi(i3));
    }
    file.close();
    std::clock_t start;
    double duration;
    start = std::clock();
    cout << "Beginning Prims MST from vertex 0 \n";
    prims(0);
    duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
    cout<<"printf: "<< duration <<'\n';
    return 0;
}


