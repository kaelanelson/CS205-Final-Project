

#include <iostream>
#include <queue>
#include <fstream>
#include <list>
#include <algorithm>
#include <array>
#include<numeric>

using namespace std;

// Build Graph from adjacency matrix (represented as a list)

int num_vertices;
int **A;

void addEdge(int v1, int v2, int w);

vector<int> prims(int s);

void ClosenessCentrality(int maxId);


void addEdge(int v1, int v2, int w) {
    A[v1][v2] = w;
    A[v2][v1] = w;
}

// Adapted from https://www.programiz.com/dsa/prim-algorithm
vector<int> prims(int s) {

    int numEdges = 0; //init
    bool *selected = new bool[num_vertices];
    for (int v = 0; v < num_vertices; v++) {
        selected[v] = false;
    }
    // choose sth vertex and make it true
    selected[s] = true;

    int l = 0;
    int row, col;
    vector<int> row_ls(num_vertices);
    vector<int> col_ls(num_vertices);
    // cout << "Edge" << " : " << "Weight\n";
    while (numEdges < num_vertices - 1) {
        int min = INT8_MAX;
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
        // cout << row << " - " << col << " :  " << A[row][col] << "\n";
        selected[col] = true;
        numEdges++;
        row_ls[l] = row;
        col_ls[l] = col;
        l++;
    }

    // create p: a list containing the path lengths from node s to all other nodes
    vector<int> p(num_vertices);
    int summ = 0;
    for(int k=0; k < num_vertices; k++){

        int cr = row_ls[k];
        int cc = col_ls[k];
        
        if(k == 0){ // if current node is the first node, add weight
            p[k] = A[cr][cc];
        }

        else if(cr == row_ls[0]){ // if current node is same as first node, add weight
            p[k] = A[cr][cc];
        }

        else if(cr == col_ls[k-1]){ 
            // else, the current node is connected to the previous node -> sum up weights from path leading up to it
            int pc = col_ls[k-1];
            summ = summ + A[cr][pc];
            p[k] = summ;
            }
        }

    return p;
}

void ClosenessCentrality(int maxId){

    // initialize list
    vector<float> closeness_centrality(num_vertices, -1);


    list<int>::iterator i;
    for(int i=1; i < num_vertices; i++){
        // sum min path starting at node i
        vector<int> sp = prims(i);

         // add up path distances to neighboring nodes
        int tot_sp = accumulate(sp.begin(), sp.end(), 0);

        // calculate closeness
        // not normalized 
        if(tot_sp>0 and num_vertices>1){
            closeness_centrality[i] = ((float)num_vertices - 1)/(float)tot_sp; 
            }

        else{
            closeness_centrality[i] = 0.0;
            }
        }

    // print for each node's closeness measurement;
    for (int i = 1; i < num_vertices; ++i)
        cout << i << " : " << closeness_centrality[i] << "\n";

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

    cout << "\n Closeness Centrality \n";
    ClosenessCentrality(maxId);

    return 0;
}