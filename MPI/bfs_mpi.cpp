/* Kaela Nelson, CS 205 Project, Spring 2020
Parallelized breadth first search implementation
Adapted from https://github.com/starlordvk/Parallel-BFS/blob/master/MPI/MPI.cpp*/

#include <iostream>
#include <fstream>
#include <list>
#include <algorithm>
#include <bits/stdc++.h> 
#include "mpi.h"
#define MAX_QUEUE_SIZE 5

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
	void BFS(int first, int rank);
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


void Graph::BFS(int first, int rank){

	// broadcast number of vertices and source vertex
	MPI_Bcast(&num_vertices, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&first, 1, MPI_INT, 0, MPI_COMM_WORLD);

	// scatter each row of adjacency matrix, A to each of processes
	// send_data, send_count, MPI dtype, recv_data, recv-count, MPI dtype,COMM
	MPI_Scatter(A, num_vertices, MPI_INT, adjacency_row, num_vertices, MPI_INT, 0, MPI_COMM_WORLD);
	
	// initialize adjecency queue of each process
	// for(int i=0; i < MAX_QUEUE_SIZE; i++)
	// 	adjacency_queue[i] = -1;
	// adjacency_queue[0] = first


	int s;
	if(rank >= first){
		// create visited list, fill with false
		vector<bool> visited(num_vertices, false); 

		int adjacency_row[num_vertices];
		// int bfs_traversal[num_vertices];

		// create a queue, Q
		vector<int> Q;
		// int adjacency_queue[MAX_QUEUE_SIZE];

		// mark s as visited and put s into Q
		visited[first] = true;
		Q.push_back(first);

		while(!Q.empty()){
			// pop off head of Q
			s = Q[0];
			cout << "Process" << rank << ":";
			cout << s << " ";
			Q.erase(Q.begin());
			
			// int index = 0;
			// mark and enqueue all unvisited neighbor nodes of s
			for (int i = 0; i < num_vertices; i++){
				// if not visited, mark as true in visited,a nd push into queue
				// else, do nothing
				if (A[s][i] == 1 && (!visited[i])){
					visited[i] = true;
					Q.push_back(i);
					// adjacency_queue[index++] = i;
				}
			}
		}
	}

	// synchronization
	// MPI_Barrier(MPI_COMM_WORLD);
	// Gather all nodes
	MPI_Gather(Q, num_vertices, MPI_INT, adjacency_row, MAX_QUEUE_SIZE, MPI_INT,0,MPI_COMM_WORLD);

	// free everything up
	// free(bfs_traversal);
	// free(adjacency_row);
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
	G->BFS(source_vertex, rank);

	// free(A);
	MPI_Finalize();

	return 0;
}

