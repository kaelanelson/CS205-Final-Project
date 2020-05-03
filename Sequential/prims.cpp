/* Simon Warchol, CS 205 Project, Spring 2020
Prim's MST*/

#include <iostream>
#include <fstream>
#include <list>
#include <algorithm>
#include "adj_matrix.h"

using namespace std;

// Build Graph from adjacency matrix (represented as a list)
int **adjMatrix;

void primsAlgorithm(int numVertices);


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
    int maxId = 0;
    string id;
    while (!file.eof()) {
        file >> id;
        maxId = max(maxId, stoi(id));
    }
    file.clear();
    file.seekg(0);

    /* Initialize adjacency matrix */
    int numVertices = maxId;
    adjMatrix = createAdjMatrix(file, numVertices);


    file.close();
    printMatrix(numVertices, adjMatrix);
    primsAlgorithm(numVertices);
}

#include <iostream>
#include <cstring>

using namespace std;

// Adapted from https://www.programiz.com/dsa/prim-algorithm
void primsAlgorithm(int numVertices) {

    int numEdges = 0; //init
    bool *selected = new bool[numVertices];
    for (int v = 0; v< numVertices; v++){
        selected[v] = false;
    }
    // choose 0th vertex and make it true
    selected[0] = true;

    int row, col;
    cout << "Edge" << " : " << "Weight\n";
    while (numEdges < numVertices - 1) {
        int min = INT_MAX;
        row = 0;
        col = 0;
        for (int i = 0; i < numVertices; i++) {
            if (selected[i]) {
                for (int j = 0; j < numVertices; j++) {
                    if (!selected[j] && adjMatrix[i][j] != 0) { // not in selected and there is an edge
                        if (min > adjMatrix[i][j]) {
                            min = adjMatrix[i][j];
                            row = i;
                            col = j;
                        }

                    }
                }
            }
        }
        cout << row << " - " << col << " :  " << adjMatrix[row][col]<<"\n";
        selected[col] = true;
        numEdges++;
    }

}