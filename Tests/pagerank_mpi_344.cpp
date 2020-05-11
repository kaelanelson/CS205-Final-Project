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

#define numVertices 344 /* Set this when running!*/

int totalSteps = 5; /* Change if you want to run for more steps for better convergence. */
string outfileName = "pagerankOutputMPI_344.txt"; /* Change file name. */

double adjMatrix[numVertices][numVertices], currentMatrix[numVertices][numVertices], transMatrix[numVertices][numVertices], nextMatrix[numVertices][numVertices];
double damping = 0.85;
double squareDiff, prevDiff, currentDiff = 0.0;

void addEdge(int i, int j);

void printAdjacenyMatrix();

void createTransitionMatrix();

void printTransitionMatrix();

void dampenTransitionMatrix();

void initializeMatrixPower();

void printMatrixPower();

void printMatrix(double matrix[numVertices][numVertices]) {
    int i, j = 0;
    for (i=0; i<numVertices; i++) {
        printf("\n\t| ");
        for (j=0; j<numVertices; j++)
            printf("%2f ", matrix[i][j]);
        printf("|");
    }
    printf("\n");
}

int main(int argc, char *argv[]) {
	MPI_Status status;
	int rank, size;
	double tstart, tend;

	/* Initialize MPI and get rank and numVertices. */
    MPI_Init (&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

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

    if (numVertices % size != 0) {
        if (rank == 0) printf("Matrix numVertices not divisible by number of processors\n");
        MPI_Finalize();
        exit(-1);
    }

    int from = rank * numVertices/size;
    int to = ((rank + 1) * numVertices/size);


	/* Initialize adjacency matrix */
	for (int i = 0; i < numVertices; i++) {
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
	for (int i = 0; i < numVertices; i++) {
		for (int j = 0; j < numVertices; j++)
			transMatrix[i][j] = 0;
	}

	/* Initialize current matrix for matrix power*/
	for (int i = 0; i < numVertices; i++) {
		for (int j = 0; j < numVertices; j++)
			currentMatrix[i][j] = 0;
	}

	/* Initialize next matrix. */
	for (int i = 0; i < numVertices; i++) {
		for (int j = 0; j < numVertices; j++)
			nextMatrix[i][j] = 0;
	}

    from = rank * numVertices/size;
    to = ((rank+1) * numVertices/size);

    if (rank == 0) {
    	printf("Processes: %d\n", size);

		createTransitionMatrix();
		dampenTransitionMatrix();
		initializeMatrixPower();
	}
	
	/* Main Loop for PageRank */

    MPI_Bcast (transMatrix, numVertices*numVertices, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    if(rank == 0){
        MPI_Scatter (currentMatrix, numVertices*numVertices/size, MPI_DOUBLE, MPI_IN_PLACE, numVertices*numVertices/size, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    }
    else {
        MPI_Scatter (currentMatrix, numVertices*numVertices/size, MPI_DOUBLE, &currentMatrix[from][0], numVertices*numVertices/size, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    }


    printf("computing slice %d (from row %d to %d)\n", rank, from, to-1);

    for (int i = from; i < to; i++) 
        for (int j = 0; j < numVertices; j++) {
            nextMatrix[i][j] = currentMatrix[i][j];
        }

    int step = 0; 
	do	{
	    for (int i = from; i < to; i++) 
	        for (int j = 0; j < numVertices; j++) {
	        	double sum = 0.0;
	            for (int k = 0; k < numVertices; k++){
	                sum += nextMatrix[i][k]*transMatrix[k][j];
	            nextMatrix[i][j] = sum;
	            }
	        }
	    step++;
    }
    while(step < 5);

    if(rank == 0) {
         MPI_Gather (MPI_IN_PLACE, numVertices*numVertices/size, MPI_DOUBLE, &nextMatrix[0][0], numVertices*numVertices/size, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    } 
    else {
         MPI_Gather (&nextMatrix[from][0], numVertices*numVertices/size, MPI_DOUBLE, &nextMatrix[0][0], numVertices*numVertices/size, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    }

    tend = MPI_Wtime();

    if (rank == 0) {
        printf("\n\n");

		/* Print output to file. */
		ofstream outfile;
		outfile.open(outfileName);

		double	rank[numVertices];
		int	vertex[numVertices];
		for (int j = 0; j < numVertices; j++) {
			vertex[j] = j;
			rank[j] = currentMatrix[0][j]; 
			}
		for (int j = 0; j < numVertices; j++) outfile << "vertex: " << vertex[j] << ", rank: " << rank[j] << "\n";
		outfile.close(); 

		printf("Elapsed time: %g s\n", tend-tstart);
    }

	MPI_Finalize();
	
	return 0;
}

// From adj_matrix.cpp
void addEdge(int i, int j) {
	adjMatrix[i][j] = 1;
	adjMatrix[j][i] = 1;
}

void printAdjacenyMatrix() {
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

void printMatrixPower() {
	for (int i = 0; i < numVertices; i++) {
		cout << i << " : ";
		for (int j = 0; j < numVertices; j++)
			cout << currentMatrix[i][j] << " ";
		cout << "\n";
	}
}