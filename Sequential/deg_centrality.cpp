/* Kyra Ballard, CS 205 Project, Spring 2020
 sequential degree centrality implementation */

#include <iostream>
#include <fstream>
#include <algorithm>
#include <ctime>

using namespace std;

int numVertices;
int **adjMatrix;

void addEdge(int i, int j) {
    adjMatrix[i][j] = 1;
    adjMatrix[j][i] = 1;
}

int main(int argc, char *argv[]) {
    
    ifstream file("all.edges", ifstream::binary); 
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
        file >> i1 >> i2;
        addEdge(stoi(i1), stoi(i2));
    }
    file.close();
    
    clock_t start = clock();
    int sum;
    for (int i = 0; i < numVertices; i++) {
        sum = 0;
        for (int j = 0; j < numVertices; j++){
            if(adjMatrix[i][j]==1)
                sum++;
        }
    }
    clock_t end = clock();
    double duration = (end - start) / (double) CLOCKS_PER_SEC;
    cout << "duration = " << duration << endl;
    
    for (int i = 0; i < numVertices; i++)
        delete[] adjMatrix[i];
    delete[] adjMatrix;
    
    return 0;
}
