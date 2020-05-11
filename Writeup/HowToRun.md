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

