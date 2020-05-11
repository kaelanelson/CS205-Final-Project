# Final Discussion

### MST Algorithms

#### Prim's Algorithm

The parallelizability of Prim's and Kruskal's algorithms differed significantly. Prim's, because it operates on an adjacency matrix, required dividing up this matrix among procesess and then having a master process compare the minimum edge of each sub matrix for each iteration. As such, the greatest improvement came from shared-memory parallelism, implemented via OpenMP. However, by refactoring to use MPI, improvements were still seen. Overall, my hybrid implementation had a speedup of 8x, which is a pretty astonishing improvement. However, it should be noted that this parallel version was still slower than the sequential version of Kruskals.

#### Kruskal's Algorithm

Kruskal's algorithm saw a more marked improvement via MPI parallelization, as each process can operate on a different group of edges, and, by doing a small amount of extra work (applying kruskal's again to each combination of MSF edges). However, since this algorithm needs to process each edge from smallest weight to highest sequentially, there is a less benefit from shared-memory parallelization. Moreover, a flaw in my analyses is perhaps that the algorithm is too fast and the dataset was thus not large enough, as i was only able to achieve a maximum 1.5x speedup. Perhaps if I had used a larger dataset, the overheads would be a lesser fraction of the overall time and speedups would be larger.

### Centrality algorithms

#### Closeness Centrality
The parallelizability of the closeness centrality algorithm relied heavily on the parallelizability of the minimum path algorithm it uses to find minimum distance between the source node and all other nodes. As a result, parallelizing this algorithm was not as complex, once we parallelized prims algorithm (see prims section for more details). With MPI and hybrid parallelizations, we achieved linear speed up, indicating that how effective MPI is at handling message passing and overheads. In addition, because of this speed up, in. the future, I would test closeness centrality on a larger graph and see at what point does the problem size slow down speed up.

#### Degree Centrality

### Shortest Path Calculation: Breadth First Search

The parallelizability of the bfs algorithm proved a more difficult task than initially anticipated. This largely due to the sequential nature of bfs, as it traverses through the graph. After looking at many sources and trying to implement MPI parallelization, we were unable to achieve a successful result. However, we were able to parallelize with OpenMP, which enabled us to to keep the visited and queue the same among all threads with the command ```pragma omp critical```. From this parallelization, we initially achieved a max speed up of 1.603 with 4 cores. However, there is a slight slowdown in speed up after 4 cores. We may see this because eventually the parallelized code will reach maximum speed up, i.e. more points to each processor slows it down. In addition, the time for each processor to communicate to each other increases, and synchronization overhead may increase. Perhaps if we used a larger data set, the overheads would be a lesser fraction of the overall time and speedups would be greater.

### PageRank

The key to parallelizing PageRank was recognizing that the "bulk" of the algorithm is merely matrix multiplication. Thus, applying data parallelism at the coarse-grained as well as the coarse-and-fine-grained levels generally met our expectations of speedup linear with the number of processors. We were also able to show that for the hybrid implementation in configurations of only one instance could perform comparably to MPI implementations with two instances . For example, For 8 total processes, the MPI + OpenMP configuration has 94% of the speedup of the MPI version. When testing for weak scaling, we found that speedup was fairly consistent across the tested problem sizes. The largest number of vertices tested was 4032, which took an hour and 15 minutes to run on the sequential version and about 11 minutes on the best performing MPI + OpenMP configuration for 8 processes. However, we should highlight that a dataset with 4032 edges represents a relatively small graph and so there is more work to be done.

### General Discussion

As is the case in a more general sense, the extent to which a given algorithm can be parallelized differs drastically, and as such, adopting a hybrid approach is preferred, which allows for flexibility. We would say that generally, with the exception of VERY dense networks, an algorithm which operates on adjacency matrices is preferred, and this is visible from the comparable performance of Prim's vs Kruskal's.


Generally, we are satisfied the outcome of our project, and we were, across the board, able to improve the performance of each graph algorithm via hybrid computing. Nonetheless, we did run into a few obstacles. We initially planned on using a hybrid approach of MPI and MapReduce. However, after gaining a deeper understanding of our algorithms and seeing that libraries such as the Parallel Boost Graph Library apply both coarse and fine-grained parallelism, we decided to use a hybrid MPI and OpenMP model. Doing so also allowed for easier comparison between the MPI and Hybrid versions of our algorithms, given the similarity in code structure. 

Our development process was to first develop a sequential version of the algorithm, followed by a MPI version, and finally a Hybrid version. It may be due to this process that, in many cases, the largest improvements in speedup were seen via MPI as opposed to shared-memory parallelism, and that if we had first focused first on OpenMP, this outcome would have been reversed.

Our largest takeaway is that, while hybrid parallelism is almost always preferred, the division between the two hybrid components is rarely going to be equal; in situations like IO, it makes much more sense to leverage shared-memory parallelism, while large sequential algorithms, such as Kruskal's, benefit more from the division of the problem size. Thus, a testament to hybrid parallel computing is that most algorithms can be improved in some manner, and having flexibility is paramount.



