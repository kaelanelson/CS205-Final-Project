/* Kyra Ballard / Simon Warchol, CS 205 Project, Spring 2020
sequential adjacency matrix implementation */

#include <iostream>
#include <fstream>
#include <algorithm>

using namespace std;


void addEdge(int i, int j, int **matrix);

void printMatrix(int numVertices);

int **createAdjMatrix(ifstream &file, int numVertices);

int **createAdjMatrix(ifstream &file, int numVertices) {
    /* Count number of IDs */

    int **matrix = new int *[numVertices];
    for (int i = 0; i < numVertices; i++) {
        matrix[i] = new int[numVertices];
        for (int j = 0; j < numVertices; j++)
            matrix[i][j] = 0;
    }

    /* Add edges to matrix */
    string i1, i2;
    while (!file.eof()) {
        file >> i1;
        file >> i2;
        addEdge(stoi(i1)-1, stoi(i2)-1, matrix);
    }
    return matrix;

}


void addEdge(int i, int j, int **matrix) {
    matrix[i][j] = 1;
    matrix[j][i] = 1;
}

void printMatrix(int numVertices, int** matrix) {
    for (int i = 0; i < numVertices; i++) {
        cout << i << " : ";
        for (int j = 0; j < numVertices; j++)
            cout << matrix[i][j] << " ";
        cout << "\n";
    }
}
