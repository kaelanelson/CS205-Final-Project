/* Kaela Nelson, CS 205 Project, Spring 2020
serial breadth first search implementation
adapted from https://www.geeksforgeeks.org/breadth-first-search-or-bfs-for-a-graph/ and
https://www.geeksforgeeks.org/implementation-of-bfs-using-adjacency-matrix/ */

#include <iostream>
#include <fstream>
#include <list>
#include <algorithm>
#include <bits/stdc++.h> 

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

	// list<int>::iterator i;
	int s;
	while(!Q.empty()){
		// pop off head of Q
		#pragma omp critical
		{
		s = Q[0];
		cout << s << " ";
		Q.erase(Q.begin());
		}

		// mark and enqueue all unvisited neighbor nodes of s
		#pragma omp parallel for
		for (int i = 0; i < num_vertices; i++){
			// if not visited, mark as true in visited,and push into queue
			// else, do nothing
			if (A[s][i] == 1 && (!visited[i])){
				#pragma omp critical
				{
				visited[i] = true;
				Q.push_back(i);
				}
			}
		}
	}
}

int main(int argc, char * argv[]){
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

	cout << "BFS traversal starting. from vertex 1 \n";
	G.BFS(1);

	return 0;
}