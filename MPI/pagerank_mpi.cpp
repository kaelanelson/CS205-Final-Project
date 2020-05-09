/* Paulina Toro Isaza, CS 205 Project, Spring 2020
sequential pagerank implementation adapted from:
http://www.ccs.neu.edu/home/daikeshi/notes/PageRank.pdf
https://www.cs.usfca.edu/~cruse/math202s11/pagerank.cpp */

#include <iostream>
#include <fstream>
#include <algorithm>
#include <stdio.h>
#include <mpi.h>

using namespace std;

int numVertices;
int **adjMatrix;
double **transMatrix;
double **currentMatrix;
double **nextMatrix;
double damping = 0.85;
double squareDiff, prevDiff, currentDiff = 0.0;

void addEdge(int i, int j);

void printMatrix();

void createTransitionMatrix();

void printTransitionMatrix();

void dampenTransitionMatrix();

void initializeMatrixPower();

int main(int argc, char *argv[]) {
	MPI_Status status;
	int rank, size, from, to;
	double tstart, tend;

	/* Initialize MPI and get rank and size. */
	MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

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

	/* Start PageRank algorithm. */
	tstart = MPI_Wtime();
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

	/* Initialize next matrix. */
	nextMatrix = new double *[numVertices];
	for (int i = 0; i < numVertices; i++) {
		nextMatrix[i] = new double[numVertices];
		for (int j = 0; j < numVertices; j++)
			nextMatrix[i][j] = 0;
	}

	from = rank * numVertices/size;
	to = (rank + 1) * numVertices/size;

	double cm[numVertices], nm[numVertices];

    if (rank == 0) {
		if (numVertices % size != 0) {
		    if (rank == 0) printf("Matrix size not divisible by number of processors\n");
		    MPI_Finalize();
		    exit(-1);
	  	}
	  	else
  			printf("Matrix size valid. Proceeeding.\n");

    	printf("Processes: %d\n", size);

		createTransitionMatrix();
		dampenTransitionMatrix();
		initializeMatrixPower();
	}
	
	/* Main Loop for PageRank */

	int step = 0; 
	do	{
		MPI_Bcast (transMatrix, numVertices*numVertices, MPI_INT, 0, MPI_COMM_WORLD);
  		MPI_Scatter (currentMatrix, numVertices*numVertices/size, MPI_INT, currentMatrix[from], numVertices*numVertices/size, MPI_INT, 0, MPI_COMM_WORLD);

  		MPI_Barrier(MPI_COMM_WORLD);

  		printf("computing slice %d (from row %d to %d)\n", rank, from, to-1);
		// compute the next matrix power 
		for (int i = from; i < to; i++)
			for (int j = 0; j < numVertices; j++) {
				double sum = 0.0;
				for (int k = 0; k < numVertices; k++)
					sum += currentMatrix[i][k] * transMatrix[k][j];
				nextMatrix[i][j] = sum;
		}
		
		MPI_Gather (nextMatrix[from], numVertices*numVertices/size, MPI_INT, nextMatrix, numVertices*numVertices/size, MPI_INT, 0, MPI_COMM_WORLD);
		MPI_Barrier(MPI_COMM_WORLD);   

		// Check convergence
		/*
		prevDiff = squareDiff;
		double diff = 0.0;
		squareDiff = 0.0; 

		for (int j = 0; j < numVertices; j++) 
		for (int i = 1; i < numVertices; i++) {
			diff = (currentMatrix[i][j] - currentMatrix[0][j]);
			squareDiff += diff * diff;
		}
		currentDiff = abs(prevDiff - squareDiff); 
		*/

		currentMatrix = nextMatrix;
		step++;
	}
	while(step < 5);
	printf( "\n" );

	/* End PageRank algorithm. */
	tend = MPI_Wtime();

	if (rank == 0) {
		/* Timing */
        printf("Elapsed time: %g s\n", tend-tstart);

		/* Print output to file. */
		ofstream outfile;
		outfile.open ("pagerankOutput_mpi.txt");

		double	rank[numVertices];
		int	vertex[numVertices];
		for (int j = 0; j < numVertices; j++) {
			vertex[j] = j;
			rank[j] = currentMatrix[0][j]; 
			}
		for (int j = 0; j < numVertices; j++) outfile << "vertex: " << vertex[j] << ", rank: " << rank[j] << "\n";
		outfile.close();
		/* End of unsorted output. */
	}

	// Sorting PageRank vector
	/*
	int	i = 0, j = 1;
	do {
		if ( rank[i] < rank[j] ) {
			int	node_i, node_j;
			double	temp_i, temp_j;
			temp_i = rank[i];
			temp_j = rank[j];
			rank[i] = temp_j;
			rank[j] = temp_i;
			node_i = vertex[i];9
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
	*/

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

	MPI_Finalize();

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