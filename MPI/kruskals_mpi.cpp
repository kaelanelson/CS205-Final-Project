/* Simon Warchol, CS 205 Project, Spring 2020
Kruskal's algorithm, adapted from https://www.geeksforgeeks.org/prims-mst-for-adjacency-list-representation-greedy-algo-6/
 and https://www.geeksforgeeks.org/kruskals-minimum-spanning-tree-using-stl-in-c/*/

#include <iostream>
#include <queue>
#include <fstream>
#include <list>
#include <algorithm>
#include <mpi.h>
#include <math.h>


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
edge **edgesPerProcess;
int *numEdgesPerProcess;

int offset, size, nper, rank;
MPI_Datatype mpi_edge;

void terminate_process();
void kruskals(int rank, int num_iters);
void divideAndDistributeEdges(int argc, char *argv[]);
// This comparison of edges via https://pdfs.semanticscholar.org/14be/5e7a0feeafaf7fc9c9339dee3d8c837b115c.pdf,
// which will allow me to quicksort
int compare_edges(const void *a, const void *b) {
    edge *ea = (edge *)a;
    edge *eb = (edge *)b;

    if (ea->weight < eb->weight) {
        return -1;
    } else if (ea->weight > eb->weight) {
        return 1;
    }

    // Never happens if weights are distinct
    return 0;
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

void terminate_process(){
    free(E);
    MPI_Finalize();
}



void kruskals(int rank, int num_iters) {
    // cout<<"Rank"<<rank<<" ITERS"<<num_iters<<"\n";
    MPI_Status status;
    int msf_edge_count =  0;
    edge* msf_edges = new edge[num_edges];

    for (int iter = 1; iter<num_iters+2; iter++){
        // Sort Edges by weight, increasing
        qsort(E, num_edges, sizeof(edge), compare_edges);

        DisjointSets DS(num_vertices);

        for (int i = 0;i < num_edges ; i++) {
            int v1 = E[i].v1;
            int v2 = E[i].v2;
            int w = E[i].weight;
            int set_v1 = DS.find(v1);
            int set_v2 = DS.find(v2);
            if (set_v1 != set_v2) {
                // cout << v1 << " - " << v2 << " - " << w << "\n";
                DS.merge(set_v1, set_v2);
                msf_edges[msf_edge_count] = E[i];
                msf_edge_count++;
            }
        }
        int remaining_processes = size/iter;
        if(remaining_processes>1){
            // High ranking processes
            if (rank >= (remaining_processes/2)){
                int to_who = rank - remaining_processes/2;
                MPI_Send(&msf_edge_count, 1, MPI_INT, to_who, 2, MPI_COMM_WORLD);
                MPI_Send(msf_edges, msf_edge_count, mpi_edge, to_who, 3, MPI_COMM_WORLD);
            } else {
                int from_who = rank + remaining_processes/2;
                int new_edges;
                cout<<"RANK:"<<rank<<" receiving from "<<from_who<<"\n";
                cout<<"\nMyList\n";
                for (int i = 0;i<msf_edge_count;i++){
                    cout << msf_edges[i].v1 << " - " << msf_edges[i].v2 << " - " << msf_edges[i].weight << "\n";
                }
                MPI_Recv(&new_edges, 1, MPI_INT, from_who, 2, MPI_COMM_WORLD, &status);
                cout<<"NEW EDGES"<<new_edges<<"\n";
                edge* new_E = new edge[new_edges+msf_edge_count];
                MPI_Recv(new_E, new_edges, mpi_edge, from_who, 3, MPI_COMM_WORLD, &status);
                cout<<"\nTheirList\n";
                for (int i = 0;i<new_edges;i++){
                    cout << new_E[i].v1 << " - " << new_E[i].v2 << " - " << new_E[i].weight << "\n";
                }
                for (int i =0;i<msf_edge_count;i++){
                    new_E[new_edges+i] = msf_edges[i];
                }
                E = new_E;
                num_edges = new_edges+msf_edge_count;
                cout<<"\nCombinedList"<<num_edges<<"\n";
                for (int i = 0;i<num_edges;i++){
                    cout << E[i].v1 << " - " << E[i].v2 << " - " << E[i].weight << "\n";
                }
                cout<<"Done\n";

            }
        } else {
            num_edges = msf_edge_count;
            E = msf_edges;
        }
        msf_edge_count =  0;
    }
}

void divideAndDistributeEdges(int argc, char *argv[]){
    numEdgesPerProcess = new int[size];
    for (int i = 0;i<size;i++){
        numEdgesPerProcess[i] = 0;
    }
    
    // Divide the vertices into groups by process
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
    file.clear();
    file.seekg(0);

    // initialize graph
    num_vertices  =  maxId +1;
    int range_per_process = num_vertices / size;
    num_edges = nEdges;
    edgesPerProcess = new edge*[size];
    for (int i =0;i<size;i++){
        edgesPerProcess[i] = new edge[num_edges];
    }
    // Add edges
    string i1, i2, i3;
    while (!file.eof()) {
        file >> i1;
        file >> i2;
        file >> i3;
        edge e = { stoi(i1), stoi(i2), stoi(i3)};
        int process_v1 = e.v1/range_per_process;
        int process_v2 = e.v2/range_per_process;
        edgesPerProcess[process_v1][numEdgesPerProcess[process_v1]] = e; 
        numEdgesPerProcess[process_v1]++;
        if(process_v1 != process_v2){
            edge e2 = { stoi(i1), stoi(i2), stoi(i3)};
            edgesPerProcess[process_v2][numEdgesPerProcess[process_v2]] = e2; 
            numEdgesPerProcess[process_v2]++;
        }

    }
    file.close();
    for (int i =1;i < size; i++){
        // Send each process the number of edges 
        MPI_Send(&num_vertices, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
        MPI_Send(&numEdgesPerProcess[i], 1, MPI_INT, i, 0, MPI_COMM_WORLD);
        MPI_Send(edgesPerProcess[i], numEdgesPerProcess[i], mpi_edge, i, 1, MPI_COMM_WORLD);
    }
    // For process 0, set my E array to first group of edges
    free(E);
    E = edgesPerProcess[0];
    num_edges = numEdgesPerProcess[0];
    file.close();
}

int main(int argc, char *argv[]) {
    MPI_Status status;

    int rank;
    
    /* Initialize MPI and get rank and size */
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    // Declaring an MPI datatype for an edge, which is just 3 ints
    MPI_Type_contiguous(3, MPI_INT, &mpi_edge);
    MPI_Type_commit(&mpi_edge);
    int num_iters = static_cast<int>(log2(size));
    if(rank == 0){
        divideAndDistributeEdges(argc,argv);
    } else {
        MPI_Recv(&num_vertices, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        MPI_Recv(&num_edges, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        E = new edge[num_edges];
        MPI_Recv(E, num_edges, mpi_edge, 0, 1, MPI_COMM_WORLD, &status);
    }
    kruskals(rank, num_iters);
    if(rank==0){
        cout<<"\nFinal\n";
        for (int i = 0;i < num_edges; i++){
            int v1 = E[i].v1;
            int v2 = E[i].v2;
            int w = E[i].weight;
            cout << v1 << " - " << v2 << " - " << w << endl;
        }
        for (int i=0;i<size;i++){
            free(edgesPerProcess[i]);
        }
        free(edgesPerProcess);
        free(numEdgesPerProcess);
    }
    terminate_process();

}


