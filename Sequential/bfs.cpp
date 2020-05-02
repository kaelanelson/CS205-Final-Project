/* Kaela Nelson, CS 205 Project, Spring 2020
sequential breadth first search implementation */

#include <iostream>
#include <fstream>
#include <list>
#include <algorithm>

using namespace std;

// Build Graph from adjacency matrix (represented as a list)
class Graph
{
	int num_vertices; 
	list<int> *A;

public:
	Graph(int num_vertices);
	void addEdge(int v, int w);
	void BFS(int s);
};

Graph::Graph(int num_vertices){
	this->num_vertices = num_vertices;
	A = new list<int>[num_vertices];
}

void Graph::addEdge(int v, int w){
	A[v].push_back(w);
}


void Graph::BFS(int s){
	// create a queue, Q
	list<int> Q;

	// create visited list
	bool *visited = new bool[num_vertices];
	for(int i=0; i<num_vertices; i++)
		visited[i] = false;

	// mark s as visited and put s into Q
	visited[s] = true;
	Q.push_back(s);

	// list<int>::iterator i;
	while(!Q.empty()){
		// pop off head of Q
		s = Q.front();
		cout << s << " ";
		Q.pop_front();
		

		// mark and enqueue all unvisited neighbor nodes of s
		list<int>::iterator i;
		for (i=A[s].begin(); i != A[s].end(); i++){
			// if not visited, mark as true in visited,a nd push into queue
			// else, do nothing
			if (!visited[*i]){
				visited[*i] = true;
				Q.push_back(*i);
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
	Graph G(maxId+1);

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

