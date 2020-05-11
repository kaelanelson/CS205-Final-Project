# Instructions for Running Code

## Prim's Algorithm

Data file must be adjacency list in the format  `v1 v2 w`

### Sequential 

```bash
g++ Sequential/prims.cpp -o prims
./prims data/generatedMstDataPrims.txt
```

### MPI

Must be run on cluster configured like so: https://harvard-iacs.github.io/2020-CS205/lab/I7/guide/Guide_I7.pdf

Run on all nodes

```bash
mpic++ prims_mpi.cpp -o prims_mpi
cp prims_mpi cloud/
```

Then from the master node run

```bash
mpirun -np 4 -hosts master,node1 ./prims_mpi generatedMstDataPrims.txt
```

### Hybrid

Must be run on cluster configured like so: https://harvard-iacs.github.io/2020-CS205/lab/I7/guide/Guide_I7.pdf

Run on all nodes, where x is the number of threads

```bash
export OMP_NUM_THREADS=x
mpic++ -fopenmp prims_hybrid.cpp -o prims_hybrid
cp prims_hybrid cloud/
```

Then from the master node run

```bash
mpirun -np 4 -hosts master,node1 ./prims_hybrid generatedMstDataPrims.txt
```

## Kruskal's Algorithm

Data file must be adjacency list in the format  `v1 v2 w`

### Sequential 

```bash
g++ Sequential/kruskals.cpp -o kruskals
./kruskals data/generatedMstDataPrims.txt
```

### MPI

Must be run on cluster configured like so: https://harvard-iacs.github.io/2020-CS205/lab/I7/guide/Guide_I7.pdf

Run on all nodes

```bash
mpic++ kruskals_mpi.cpp -o kruskals_mpi
cp kruskals_mpi cloud/
```

Then from the master node run

```bash
mpirun -np 4 -hosts master,node1 ./kruskals_mpi generatedMstDataKruskals.txt
```

### Hybrid

Must be run on cluster configured like so: https://harvard-iacs.github.io/2020-CS205/lab/I7/guide/Guide_I7.pdf

Run on all nodes, where x is the number of threads

```bash
export OMP_NUM_THREADS=x
mpic++ -fopenmp kruskals_hybrid.cpp -o kruskals_hybrid
cp kruskals_hybrid cloud/
```

Then from the master node run

```bash
mpirun -np 4 -hosts master,node1 ./kruskals_hybrid generatedMstDataKruskals.txt
```

## BFS Algorithm

Data file must be adjacency list in the format  `v1 v2`

### Sequential
```bash
g++ -DUSE_CLOCK bfs.cpp timing.cpp -o bfs
time ./bfs all.edges
```

### OpenMP

Must be run on cluster configured like so: https://harvard-iacs.github.io/2020-CS205/lab/I6/guide/Guide_I6.pdf


```bash
export OMP_NUM_THREADS=x
g++ -fopenmp -Ofast bfs_omp.cpp -o bfs_omp
./bfs_omp all.edges
```

## Closeness Centrality

Data file must be adjacency list in the format  `v1 v2 w`

### Sequential

```bash
g++ -DUSE_CLOCK closeness_centrality.cpp timing.cpp -o cc
time ./cc mst_test3_sub.txt
```

### MPI

Must be run on cluster configured like so: https://harvard-iacs.github.io/2020-CS205/lab/I7/guide/Guide_I7.pdf

Run on all nodes

```bash
mpic++ cc_mpi.cpp -o ccm
cp ccm cloud/
```

Then, on master node, run (with x processors):

```bash
mpirun -np x -hosts master,node1 ./ccm mst_test3_sub.txt 

```

### Hybrid

Must be run on cluster configured like so: https://harvard-iacs.github.io/2020-CS205/lab/I7/guide/Guide_I7.pdf

Run on all nodes, where x is the number of threads

```bash
export OMP_NUM_THREADS=x
mpic++ -fopenmp cc_hybrid.cpp -o cch
cp cch cloud/
```

Then, on master, run:
```
mpirun -np x -hosts master,node1 ./cch mst_test3_sub.txt 

```

