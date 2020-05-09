#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define SIZE 4         /* Size of matrices */
#define MAX_RAND     10

int A[SIZE][SIZE], B[SIZE][SIZE], C[SIZE][SIZE];
void fill_matrix(int m[SIZE][SIZE])
{
    //static int n=0;
    int i, j;

    printf("\n*****************************\n");
    for (i=0; i<SIZE; i++)
    {
        for (j=0; j<SIZE; j++){     
            m[i][j] = rand() % MAX_RAND;
            printf("%2d ", m[i][j]);
        }
        printf("\n");
    }
    printf("\n*****************************\n");
}



void fill_vector(int m[SIZE][1])
{
    //static int n=0;
    int i, j;

    printf("\n*****************************\n");
    for (i=0; i<SIZE; i++)
    {
        for (j=0; j<1; j++){     
            m[i][j] = rand() % MAX_RAND;
            printf("%2d ", m[i][j]);
        }
        printf("\n");
    }
    printf("\n*****************************\n");
}


void print_matrix(int m[SIZE][SIZE])
{
    int i, j = 0;
    for (i=0; i<SIZE; i++) {
        printf("\n\t| ");
        for (j=0; j<SIZE; j++)
            printf("%2d ", m[i][j]);
        printf("|");
    }
}




void print_vector(int m[SIZE][1])
{
    int i, j = 0;
    for (i=0; i<SIZE; i++) {
        printf("\n\t| ");
        for (j=0; j<1; j++)
            printf("%2d ", m[i][j]);
        printf("|");
    }
}


int main(int argc, char *argv[])
{
    int myrank, P, from, to, i, j, k;
    //  int tag = 666;      /* any value will do */
    //  MPI_Status status;

    MPI_Init (&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);   /* who am i */
    MPI_Comm_size(MPI_COMM_WORLD, &P); /* number of processors */


    if (SIZE%P!=0) {
        if (myrank==0) printf("Matrix size not divisible by number of processors\n");
        MPI_Finalize();
        exit(-1);
    }

    from = myrank * SIZE/P;
    to = ((myrank+1) * SIZE/P);

    /* Process 0 fills the input matrices and broadcasts them to the rest */
    /* (actually, only the relevant stripe of A is sent to each process) */

    if (myrank==0) {


        //static int n=0;
        int i, j;

        printf("\n*****************************\n");
        for (i=0; i<SIZE; i++)
        {
            for (j=0; j<SIZE; j++){     
                A[i][j] = rand() % MAX_RAND;        
                printf("%d ", A[i][j]);
            }
            printf("\n");

            printf("\n*****************************\n");
        }
        fill_matrix(B);
    }

    //int s=SIZE*SIZE/P;
    // printf("computing slice %d (from row %d to %d)\n", myrank, from, to-1);
    MPI_Bcast (B, SIZE*SIZE, MPI_INT, 0, MPI_COMM_WORLD);
    //    printf("\n\n%d",s);
    //print_vector(s);
    //printf("\n\n");
    if(myrank==0){
        MPI_Scatter (&A[0][0], SIZE*SIZE/P, MPI_INT, MPI_IN_PLACE, SIZE*SIZE/P, MPI_INT, 0, MPI_COMM_WORLD);
    }else{
        MPI_Scatter (&A[0][0], SIZE*SIZE/P, MPI_INT, &A[from][0], SIZE*SIZE/P, MPI_INT, 0, MPI_COMM_WORLD);
    }


    printf("computing slice %d (from row %d to %d)\n", myrank, from, to-1);
    for (i=from; i<to; i++) 
        for (j=0; j<SIZE; j++) {
            C[i][j]=0;
            for (k=0; k<SIZE; k++){
                C[i][j] += A[i][k]*B[k][j];
            }
        }

    print_matrix(C);

    if(myrank==0){
         MPI_Gather (MPI_IN_PLACE, SIZE/P, MPI_INT, &C[0][0], SIZE/P, MPI_INT, 0, MPI_COMM_WORLD);
    }else{
         MPI_Gather (&C[to][0], SIZE/P, MPI_INT, &C[0][0], SIZE/P, MPI_INT, 0, MPI_COMM_WORLD);
    }

    if (myrank==0) {
        printf("\n\n");

        print_matrix(A);

        printf("\n\n");
        //   print_matrix(D);
        printf("\n\n\t       * \n");
        print_matrix(B);
        printf("\n\n\t       = \n");
        print_matrix(C);
        printf("\n\n");
        //   print_vector(E);
        //   printf("\n\n");
    }

    MPI_Finalize();
    return 0;
}