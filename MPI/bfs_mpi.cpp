/* Kaela Nelson, CS 205 Project, Spring 2020
Parallelized breadth first search implementation
Adapted from https://github.com/starlordvk/Parallel-BFS/blob/master/MPI/MPI.cpp*/

#include <iostream>
#include <fstream>
#include <list>
#include <algorithm>
#include <bits/stdc++.h> 
#include "mpi.h"
#define MAX_QUEUE_SIZE 10

using namespace std;

// Build Graph from adjacency matrix (represented as a list)
class Graph
{
	int num_vertices; 
	int num_edges;
	int** A;

public:
	Graph(int num_vertices, int num_edges);
	void addEdge(int n, int e);
	void BFS(int first, int rank, int size);
	int areAllVisited(int visited[]);
};

Graph::Graph(int num_vertices, int num_edges){
	this->num_vertices = num_vertices;
	this->num_edges = num_edges;
	A = new int*[num_vertices];
	for(int r=0; r < num_vertices; r++){
		A[r] = new int[num_vertices];
		for(int c=0; c < num_vertices; c++){
			A[r][c] = 0;
		} 
	}
}

void Graph::addEdge(int n, int e){
	// initialize bidirectional
	A[n][e] = 1;
	A[e][n] = 1;
}

int Graph::areAllVisited(int visited[])
{
	for(int i = 0; i < num_vertices; i++)
	{
		if(visited[i] == 0)
			return 0;
	}
	return 1;
}


void Graph::BFS(int first, int rank, int size){
	// create visited list, fill with false
	// create a queue, Q
	int Q[MAX_QUEUE_SIZE];
	int visited[num_vertices*num_vertices];
	int adjacency_row[num_vertices]; // create adjacency row to be distributed to processes
	int bfs_traversal[num_vertices*num_vertices];

	// initialize Q
	for(int i = 0; i < MAX_QUEUE_SIZE; i++)
	{
		Q[i] = -1;
	}

	// broadcast number of vertices and source vertex
	MPI_Bcast(&num_vertices, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&first, 1, MPI_INT, 0, MPI_COMM_WORLD);

	// scatter each row of adjacency matrix, A to each of processes
	// send_data, send_count, MPI dtype, recv_data, recv-count, MPI dtype,COMM
	MPI_Scatter(A, num_vertices*num_vertices, MPI_INT, adjacency_row, num_vertices, MPI_INT, 0, MPI_COMM_WORLD);


	int s;
	int index = 0;
	if(rank >= first){
		for (int i = 0; i < num_vertices; i++){
			if (adjacency_row[i] == 1){
				Q[index++] = i;
			}
		}
	}

	cout << "Process " << rank << ":";
	for(int i=0;i<index;i++){
		cout<<Q[i]<<" ";
	}
	cout << endl;

	// synchronization
	MPI_Barrier(MPI_COMM_WORLD);

	// Gather all nodes
	// send_data, send_count, MPI dtype, recv_data, recv-count, MPI dtype, COMM
	MPI_Gather(&Q, MAX_QUEUE_SIZE, MPI_INT, bfs_traversal, MAX_QUEUE_SIZE, MPI_INT, 0, MPI_COMM_WORLD);

	// initialize visited with all 0's
	for(int i = 0; i < num_vertices; i++)
	{
		visited[i] = 0;
	}

	// print out order
	if(rank == 0)
	{
		cout<<"\nBFS Traversal: "<<endl;
		cout<<first;
		for(int i = 0; i < num_vertices*MAX_QUEUE_SIZE; i++)
		{
			//Exit Condition
			if(areAllVisited(visited))
			{
				break;
			}

			if(bfs_traversal[i] != -1)
			{
				if(visited[bfs_traversal[i]] == 0)
				{
					cout<<" "<<bfs_traversal[i];
					visited[bfs_traversal[i]] = 1;
				}
			}
			else
			{
				continue;
			}
		}
	}
}

int main(int argc, char * argv[]){

	int size,rank;
	int source_vertex = 1;
	int maxId;
	// initialize graph
	Graph* G;
	
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	if(rank==0){
		if(argc != 2){
	        cout << "Specify an input file" << endl;
	        exit(1);
	    }
	    
	    ifstream file(argv[1], ifstream::binary);
	    if(!file.is_open()) {
	        cout << "Unable to open file " << argv[1] << endl;
	        exit(2);
	    }
	    // Count number of IDs 
	    int maxId = 0;
	    string id;
	    while(!file.eof()){
	        file >> id;
	        maxId = max(maxId, stoi(id));
	    }
	    file.clear();
	    file.seekg(0);
	
	
	    // initialize graph
		Graph G(maxId+1, maxId);

		// Add edges
		string i1, i2;
		while(!file.eof()){
			file >> i1;
			file >> i2;
			G.addEdge(stoi(i1), stoi(i2));
		}
		file.close();
	}
	

	cout << "BFS traversal starting. from vertex 1 \n";
	G->BFS(source_vertex, rank, size);

	// free(A);
	MPI_Finalize();
	return 0;
}



