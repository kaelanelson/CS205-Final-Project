/* Kaela Nelson, CS 205 Project, Spring 2020
Prim's algorithm, adapted from https://www.geeksforgeeks.org/prims-mst-for-adjacency-list-representation-greedy-algo-6/ 
Closeness Centrality, adapted from */

#include <iostream>
#include <fstream>
#include <list>
#include <mpi.h>
#include <algorithm>
#include <array>
#include <numeric>
#include <queue>

using namespace std;

// Build Graph from adjacency matrix (represented as a list)

int num_vertices;
int *A;
int offset, blocksize, size, nper, row_offset;
int *min_edge;
typedef struct { int v1; int v2; } Edge;


void addEdge(int v1, int v2, int w);
vector<int> prims(int s, int rank);
void ClosenessCentrality(int maxId, int rank);
void distributeAdjacencyMatrix(int rank);


void addEdge(int v1, int v2, int w) {
    A[v1 * num_vertices + v2] = w;
    A[v2 * num_vertices + v1] = w;
}

// Adapted from https://www.programiz.com/dsa/prim-algorithm
vector<int> prims(int s, int rank) {
    MPI_Barrier(MPI_COMM_WORLD);

    int numEdges = 0; //init
    int *selected = new int[num_vertices];
    for (int v = 0; v < num_vertices; v++) {
        selected[v] = 0;
    }
    // choose sth vertex and make it true
    selected[s] = 1;

    int r, c;
    Edge edge;
    int l=0;
    vector<int> row_ls(num_vertices);
    vector<int> col_ls(num_vertices);
    int *min_pair = new int[2];
    // if(rank==0){
        // cout << "Edge" << " : " << "Weight (num_vertices)"<<num_vertices<<"\n";
    // }
    // This way of reducing to find the min via https://github.com/elahehrashedi/MPI_Prime_MST/blob/master/PrimMPI.c
    struct { int min; int rank; } minRow, row;
    while (numEdges < num_vertices-1) {
        int min = INT32_MAX;
        r = 0;
        c = 0;
        for (int i = row_offset; i < (nper+row_offset); i++) {
            if (selected[i] == 1) {
                for (int j = 0; j < num_vertices; j++) {
                    if (selected[j] == 0  && A[i * num_vertices + j] != 0) { // not in selected and there is an edge
                        if (min > A[i * num_vertices + j]) {
                            min = A[i * num_vertices + j];
                            r = i;
                            c = j;
                        }

                    }
                }
            }
        }
        row.min = min;
        row.rank = rank; // the rank of min in row
        // This reduction via // This way of reducing to find the min via https://github.com/elahehrashedi/MPI_Prime_MST/blob/master/PrimMPI.c
        MPI_Allreduce(&row, &minRow, 1, MPI_2INT, MPI_MINLOC, MPI_COMM_WORLD);
        edge.v1 = r;
        edge.v2 = c;
        MPI_Bcast(&edge, 1, MPI_2INT, minRow.rank, MPI_COMM_WORLD);
        MPI_Bcast(&min, 1, MPI_INT, minRow.rank, MPI_COMM_WORLD);
        // if (rank==0){
            // cout << "Adding Edge: "<<edge.v1 << " - " << edge.v2 << " :  " << A[edge.v1 * num_vertices + edge.v2] << "\n";
        // }
        selected[edge.v1] = 1;
        selected[edge.v2] = 1;

        if (rank==0){
        row_ls[l] = edge.v1;
        col_ls[l] = edge.v2;
        l++;
        }
        numEdges++;

    }

    // create p: a list containing the path lengths from node s to all other nodes
    vector<int> p(num_vertices);
    // if(rank==0){
    int summ = 0;
    for(int k=0; k < num_vertices-1; k++){

        int cr = row_ls[k];
        int cc = col_ls[k];

        // if(rank==0){
        // cout << "cr : " << cr << endl;
        // cout << "cc : " << cc << endl;
        // }
        
        if(k == 0){ // if current node is the first node, add weight
            // p[k] = A[cr][cc];
            // cout << "index: "<< cr * num_vertices + cc << endl;
            p[k] = A[cr * num_vertices + cc];
            // p[k] = A[cc * num_vertices + cr];

        }

        else if(cr == row_ls[0]){ // if current node is same as first node, add weight
            // p[k] = A[cr][cc];
            // cout << cr;
            p[k] = A[cr * num_vertices + cc];
            // p[k] = A[cc * num_vertices + cr];
        }

        else if(cr == col_ls[k-1]){ 
            // cout << cr;
            // else, the current node is connected to the previous node -> sum up weights from path leading up to it
            int pc = col_ls[k-1];
            summ = summ + A[cr * num_vertices + pc];
            // summ = summ + A[pc * num_vertices + cr];
            p[k] = summ;
            }
        }
        // }
    return p;

}

void ClosenessCentrality(int maxId, int rank){

    // initialize list
    vector<float> closeness_centrality(num_vertices);


    list<int>::iterator i;
    for(int i=0; i < num_vertices; i++){
        // sum min path starting at node i
        vector<int> sp = prims(i,rank);

         // add up path distances to neighboring nodes
        int tot_sp = accumulate(sp.begin(), sp.end(), 0);
        // if(rank==0){
            // cout<<"tot sp: "<< tot_sp << endl;
        // }
        // calculate closeness
        // not normalized 
        if(tot_sp>0 and num_vertices>1){
            if(rank==0)
            // nummber od nodes / sum of min paths
            closeness_centrality[i] = ((float)num_vertices - 1)/(float)tot_sp; 
            }

        else{
            closeness_centrality[i] = 0.0;
            }
        
    }

    // print for each node's closeness measurement;
    if(rank==0){
        cout << "Closeness Centrality : " << endl;
        for (int i = 1; i < num_vertices; ++i)
            cout << i << " : " << closeness_centrality[i] << "\n";
    }
}


int main(int argc, char *argv[]){
    MPI_Status status;
    double tstart, tend;
    int rank;
    int maxId;
    /* Initialize MPI and get rank and size */
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        printf("Processes: %d\n", size);
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
        // cout << num_vertices;
        A = new int[num_vertices * num_vertices];

        //  Initialize an adjacency matrix
        for (int i = 0; i < num_vertices * num_vertices; i++) {
            A[i] = 0;
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
        // Send the Number of Vertices to the Workers
        for (int i = 1; i < size; i++) {
            MPI_Send(&num_vertices, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
        // Initialize my array of min edges, which contains pairs of weights and edges
        min_edge = new int[size*2];
        for (int i = 0;i<size*2;i+=2){
            min_edge[i] = INT8_MAX;
            min_edge[i+1] = 0;
        }
    } else {
        // Received the vertices
        MPI_Recv(&num_vertices, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        A = new int[num_vertices * num_vertices];

        //  Initialize an adjacency matrix
        for (int i = 0; i < num_vertices * num_vertices; i++) {
            A[i] = 0;
        }
    }

    // number of columns of the array that each worker gets
    nper = (num_vertices) / size;
    offset = rank * nper * num_vertices;
    row_offset = rank * nper;
    blocksize = nper * num_vertices;
    distributeAdjacencyMatrix(rank);

    // if (rank == 0){
    // prims(0, rank);
    tstart = MPI_Wtime();
    ClosenessCentrality(maxId,rank);
    tend = MPI_Wtime();

    /* Timing summary */
    if (rank == 0)
        printf( "Elapsed time: %g s\n",tend-tstart);

    free(A);
    MPI_Finalize();

    return 0;
}
//
void distributeAdjacencyMatrix(int rank){
    MPI_Status status;
    if (rank == 0) {
        for (int i = 1; i < size; i++) {
            MPI_Send(&A[blocksize*i], blocksize, MPI_INT, i, 1, MPI_COMM_WORLD);
        }
    } else {
        MPI_Recv(&A[offset], blocksize, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
    }

}



