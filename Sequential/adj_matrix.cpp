/* Kyra Ballard, CS 205 Project, Spring 2020
sequential adjacency matrix implementation */

#include <iostream>
#include <fstream>
#include <algorithm>

using namespace std;

int numVertices;
int **adjMatrix;

void addEdge(int i, int j);

void printMatrix();

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

    /* Count number of IDs */
    int maxId = 0;
    string id;
    while (!file.eof()) {
        file >> id;
        maxId = max(maxId, stoi(id));
    }
    file.clear();
    file.seekg(0);

    /* Initialize adjacency matrix */
    numVertices = maxId+1;
    adjMatrix = new int *[numVertices];
    for (int i = 0; i < numVertices; i++) {
        adjMatrix[i] = new int[numVertices];
        for (int j = 0; j < numVertices; j++)
            adjMatrix[i][j] = 0;
    }

    /* Add edges to matrix */
    string i1, i2;
    while (!file.eof()) {
        file >> i1;
        file >> i2;
//        since array is 0 indexed, but data starts w/ 1
        addEdge(stoi(i1), stoi(i2));
    }
    file.close();

    printMatrix();

    for (int i = 0; i < numVertices; i++)
        delete[] adjMatrix[i];
    delete[] adjMatrix;

    return 0;
}

void addEdge(int i, int j) {
    adjMatrix[i][j] = 1;
    adjMatrix[j][i] = 1;
}

void printMatrix() {
    for (int i = 0; i < numVertices; i++) {
        cout << i << " : ";
        for (int j = 0; j < numVertices; j++)
            cout << adjMatrix[i][j] << " ";
        cout << "\n";
    }
}
