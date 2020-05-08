/* Kaela Nelson, CS 205 Project, Spring 2020
serial breadth first search implementation
adapted from https://www.geeksforgeeks.org/breadth-first-search-or-bfs-for-a-graph/ and
https://www.geeksforgeeks.org/implementation-of-bfs-using-adjacency-matrix/ */

#include <iostream>
#include <fstream>
#include <list>
#include <algorithm>
#include <bits/stdc++.h> 
#include "mpi.h"

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
	void BFS(int first);
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


void Graph::BFS(int first){
	// create visited list, fill with false
	vector<bool> visited(num_vertices, false); 

	// create a queue, Q
	vector<int> Q;

	// mark s as visited and put s into Q
	visited[first] = true;
	Q.push_back(first);

	int bfs_traversal[num_vertices];
	// broadcast number of vertices and source vertex
	MPI_Bcast(&num_vertices, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&first, 1, MPI_INT, 0, MPI_COMM_WORLD);

	// scatter each row of adjacency matrix, A to each of processes
	// adjacency row = A[s]
	MPI_Scatter(A, num_vertices, MPI_INT, num_vertices, MPI_INT, 0, MPI_COMM_WORLD);
	

	int s;
	if(rank >= source_vertex){
		while(!Q.empty()){
			// pop off head of Q
			s = Q[0];
			cout << "Process" << rank << ":";
			cout << s << " ";
			Q.erase(Q.begin());
			

			// mark and enqueue all unvisited neighbor nodes of s
			for (int i = 0; i < num_vertices; i++){
				// if not visited, mark as true in visited,a nd push into queue
				// else, do nothing
				if (A[s][i] == 1 && (!visited[i])){
					visited[i] = true;
					Q.push_back(i);
				}
			}
		}
	}

	// synchronization
	MPI_Barrier(MPI_cOMM_WORLD);
	// Gather all nodes
	MPI_Gather(Q, num_vertices, MPI_INT, bfs_traversal, num_vertices, MPI_INT,0,MPI_COMM_WORLD);
}

int main(int argc, char * argv[]){

	int size,rank;
	int source_vertex = 1;
	
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
	G.BFS(source_vertex);

	
	MPI_Finalize();

	return 0;
}

