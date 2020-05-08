/* Paulina Toro Isaza, CS 205 Project, Spring 2020
sequential pagerank implementation adapted from:
http://www.ccs.neu.edu/home/daikeshi/notes/PageRank.pdf
https://www.cs.usfca.edu/~cruse/math202s11/pagerank.cpp */

#include <iostream>
#include <fstream>
#include <algorithm>
#include <stdio.h>
#include <time.h>
#include "timing.h"

using namespace std;

timing_t tstart, tend;
int numVertices;
int **adjMatrix;
double **transMatrix;
double **currentMatrix;
double damping = 0.85;
double squareDiff, prevDiff, currentDiff = 0.0;

void addEdge(int i, int j);

void printMatrix();

void createTransitionMatrix();

void printTransitionMatrix();

void dampenTransitionMatrix();

void initializeMatrixPower();

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
		file >> i1;
		file >> i2;
//        since array is 0 indexed, but data starts w/ 1
		addEdge(stoi(i1), stoi(i2));
	}
	file.close();

	printMatrix();

	/* PageRank starts here. */
	get_time(&tstart);

	/* Initialize transition matrix */
	transMatrix = new double *[numVertices];
	for (int i = 0; i < numVertices; i++) {
		transMatrix[i] = new double[numVertices];
		for (int j = 0; j < numVertices; j++)
			transMatrix[i][j] = 0;
	}

	/* Initialize current matrix for matrix power*/
	currentMatrix = new double *[numVertices];
	for (int i = 0; i < numVertices; i++) {
		currentMatrix[i] = new double[numVertices];
		for (int j = 0; j < numVertices; j++)
			currentMatrix[i][j] = 0;
	}

	createTransitionMatrix();
	// printTransitionMatrix();
	dampenTransitionMatrix();
	// printTransitionMatrix();
	initializeMatrixPower();

	/* Main Loop for PageRank */
	int step = 0; 
	do	{
		// compute the next matrix power 
		for (int i = 0; i < numVertices; i++)
		for (int j = 0; j < numVertices; j++) {
			double sum = 0.0;
			for (int k = 0; k < numVertices; k++)
				sum += currentMatrix[i][k] * transMatrix[k][j];
			currentMatrix[i][j] = sum;
		}

		// Check convergence 
		prevDiff = squareDiff;
		double diff = 0.0;
		squareDiff = 0.0; 

		for (int j = 0; j < numVertices; j++) 
		for (int i = 1; i < numVertices; i++) {
			diff = (currentMatrix[i][j] - currentMatrix[0][j]);
			squareDiff += diff * diff;
		}
		currentDiff = abs(prevDiff - squareDiff); 
		step++;
	}
	while(currentDiff > 0.0001);
	/* End of PageRank algorithm. */
	get_time(&tend);

	printf( "\n" );

	// Printing out PageRank vector
	double	rank[numVertices];
	int	vertex[numVertices];
	for (int j = 0; j < numVertices; j++) {
		vertex[j] = j;
		rank[j] = currentMatrix[0][j]; 
		}
	printf( "Stationary vector by vertex number: \n" );
	for (int j = 0; j < numVertices; j++) cout << "vertex: " << vertex[j] << ", rank: " << rank[j] << "\n";
	printf( "\n" );

	// Sorting PageRank vector
	int	i = 0, j = 1;
	do {
		if ( rank[i] < rank[j] ) {
			int	node_i, node_j;
			double	temp_i, temp_j;
			temp_i = rank[i];
			temp_j = rank[j];
			rank[i] = temp_j;
			rank[j] = temp_i;
			node_i = vertex[i];
			node_j = vertex[j];
			vertex[i] = node_j;
			vertex[j] = node_i;
			i = 0;
			j = 1;
		}
		else {
			++i;
			++j;
		}
	}
	while (j < numVertices);

	// display the stationary vector's components in sorted order 	
	printf( "Sorted vertices by PageRank: \n" );
	for (int j = 0; j < numVertices; j++) cout << "vertex: " << vertex[j] << ", rank: " << rank[j] << "\n";
	printf( "\n" );

	printf("*****************************************************\n");
	printf ("Elapsed Time: %g s.\n", timespec_diff(tstart,tend));

	/* Deletions */
	for (int i = 0; i < numVertices; i++)
		delete[] adjMatrix[i];
	delete[] adjMatrix;

	for (int i = 0; i < numVertices; i++)
		delete[] transMatrix[i];
	delete[] transMatrix;

	for (int i = 0; i < numVertices; i++)
		delete[] currentMatrix[i];
	delete[] currentMatrix;

	return 0;
}

// From adj_matrix.cpp
void addEdge(int i, int j) {
	adjMatrix[i][j] = 1;
	adjMatrix[j][i] = 1;
}

void printMatrix() {
	for (int i = 0; i < numVertices; i++) {
		cout << i << " : ";
		for (int j = 0; j < numVertices; j++)
			cout << adjMatrix[i][j] << " ";
		cout << "\n";
	}
}

// New for PageRank
void createTransitionMatrix() {
	for (int i = 0; i < numVertices; i++) {
		double rowSum = 0;
		for (int j = 0; j < numVertices; j++)
			rowSum += adjMatrix[i][j];
		if (rowSum > 0)
			for (int j = 0; j < numVertices; j++)
				transMatrix[i][j] = adjMatrix[i][j]/rowSum;
		else
			for (int j = 0; j < numVertices; j++)
				transMatrix[i][j] = 1.0/numVertices;
	}
}

void printTransitionMatrix() {
	for (int i = 0; i < numVertices; i++) {
		cout << i << " : ";
		for (int j = 0; j < numVertices; j++)
			cout << transMatrix[i][j] << " ";
		cout << "\n";
	}
}

void dampenTransitionMatrix() {
	for (int i = 0; i < numVertices; i++) {
		for (int j = 0; j < numVertices; j++){
			double currentMatrix = transMatrix[i][j];
			currentMatrix = (damping * currentMatrix) + ((1.0 - damping)/numVertices);
			transMatrix[i][j] = currentMatrix;
		}
	}
}

void initializeMatrixPower() {
	for (int i = 0; i < numVertices; i++) {
		for (int j = 0; j < numVertices; j++) {
			currentMatrix[i][j] = (i == j) ? 1.0 : 0.0;
		}
	}
}