/* Kyra Ballard, CS 205 Project, Spring 2020
 sequential degree centrality implementation */

#include <iostream>
#include <fstream>
#include <mpi.h>
#include <string>

using namespace std;

int num_vertices;
int* A;
int offset, blocksize, size, nper, row_offset;
typedef struct { double deg; int v; } DegCent;

void distributeAdjacencyMatrix(int rank){
    MPI_Status status;
    if (rank == 0) {
        for (int i = 1; i < size; i++)
            MPI_Send(&A[blocksize*i], blocksize, MPI_INT, i, 1, MPI_COMM_WORLD);
    } else {
        MPI_Recv(&A[offset], blocksize, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
    }
}

void addEdge(int v1, int v2) {
    A[v1 * num_vertices + v2] = 1;
    A[v2 * num_vertices + v1] = 1;
}

int main(int argc, char * argv[]){
    MPI_Status status;
    int rank;
    int size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    if (rank == 0) {
        printf("Processes: %d\n", size);
        ifstream file("all.edges", ifstream::binary);
        
        int maxId = 0;
        string id1, id2;
        while (!file.eof()) {
            file >> id1 >> id2;
            maxId = max(maxId, stoi(id1));
            maxId = max(maxId, stoi(id2));
        }
        num_vertices = maxId + 1;
        A = new int[num_vertices * num_vertices];
        for (int i = 0; i < num_vertices * num_vertices; i++) {
            A[i] = 0;
        }
        file.clear();
        file.seekg(0);
        while (!file.eof()) {
            file >> id1 >> id2;
            addEdge(stoi(id1), stoi(id2));
        }
        file.close();
        for (int i = 1; i < size; i++) {
            MPI_Send(&num_vertices, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
    } else {
        MPI_Recv(&num_vertices, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        A = new int[num_vertices * num_vertices];
        for (int i = 0; i < num_vertices * num_vertices; i++) {
            A[i] = 0;
        }
    }
    
    nper = (num_vertices) / size;
    offset = rank * nper * num_vertices;
    blocksize = nper * num_vertices;
    
    double tstart = MPI_Wtime();
    distributeAdjacencyMatrix(rank);
    double tend = MPI_Wtime();

    if (rank == 0)
        printf( "Elapsed time: %g s\n",tend-tstart);
    
    free(A);
    MPI_Finalize();
    return 0;
}
