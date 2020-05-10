# Final Discussion

### MST Algorithms

#### Prim's Algorith

The parallelizability of Prim's and Kruskal's algorithms differed significantly. Prim's, because it operates on an adjacency matrix, required dividing up this matrix among procesess and then having a master process compare the mimimum edge of each submatrix for each iteration. As such, the greatest improvement came from shared-memory parallelism, implemented via OpenMP. However, by refactoring to use MPI, improvements were still seen. Overall, my hybrid implementation had a speeup of 8x, which is a pretty astonishing improvement. However, it should be noted that this parallel version was still slower than the sequential version of Kruskals.

#### Kruskal's Algorithm

Kruskal's algorithm saw a more marked improvement via MPI parallelization, as each process can operate on a differen group of edges, and, by doing a small amount of extrar work (applying kruskal's again to each combination of MSF edges). However, since this algorithm needs to process each edge from smallest weight to highest sequentially, there is a less benefit from shared-memory parallelization. Moreover, a flaw in my analyses is perhaps that the algorithm is too fast and the dataset was thus not large enough, as i was only able to achieve a maximum 1.5x speedup. Perhaps if I had used a larger dataset, the overheads would be a lesser fraction of the overall time and speedups would be larger.

#### General MST Discussion

As is the case in a more general sense, the extent to which a given algorithm can be parallelized differs drastically, and as such, adopting a hybrid approach is preferred, which allows for flexibility. I'd generally say that with the exception of VERY dense networks, an algorithm which operates on adjacency matrices is preferred, and this is visible from the comparable performance of Prim's vs Kruskal's.





Generally, we are satisfied the outcome of our project, and we were, across the board, able to improve the performance of each graph algorithm via hybrid computing. However, we did run into a few obstacles. We initially planned on using MapReduce, after gaining a deeper understanding of our algorithms and seeing that libraries such as the Parallel Boost Graph Library apply both coarse and fine-grained parallelism, we decided to use a hybrid MPI and OpenMP model. Doing so also allowed for easier comparison between the MPI and Hybrid versions of our algorithms, given the similarity in code structure. 

Our development process was to first develop a sequential version of the algorithm, followed by a MPI version, and finally a Hybrid version. It may be due to this process that, in many cases, the largest improvements in speedup were seen via MPI as opposed to shared-memory parallelism, and that if we had first focused first on OpenMP, this outcome would have been reversed.

Our largest takeaway is that, while hybrid parallelism is always preferred, the division between the two hybrid components is rarely going to be equal; in situations like IO, it makes much more sense to leverage shared-memory parallelism, while large sequential algorithms, such as Kruskal's, benefit more from the division of the problem size. Thus, a testement to hybrid parallel computing is that most algorithms can be improved in some manner, and having flexibility is paramount.

