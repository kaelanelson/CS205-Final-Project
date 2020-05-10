# Final Discussion

Generally, we are satisfied the outcome of our project, and we were, across the board, able to improve the performance of each graph algorithm via hybrid computing. However, we did run into a few obstacles. We initially planned on using MapReduce, after gaining a deeper understanding of our algorithms and seeing that libraries such as the Parallel Boost Graph Library apply both coarse and fine-grained parallelism, we decided to use a hybrid MPI and OpenMP model. Doing so also allowed for easier comparison between the MPI and Hybrid versions of our algorithms, given the similarity in code structure. 

Our development process was to first develop a sequential version of the algorithm, followed by a MPI version, and finally a Hybrid version. It may be due to this process that, in many cases, the largest improvements in speedup were seen via MPI as opposed to shared-memory parallelism, and that if we had first focused first on OpenMP, this outcome would have been reversed.

Our largest takeaway is that, while hybrid parallelism is always preferred, the division between the two hybrid components is rarely going to be equal; in situations like IO, it makes much more sense to leverage shared-memory parallelism, while large sequential algorithms, such as Kruskal's, benefit more from the division of the problem size. Thus, a testement to hybrid parallel computing is that most algorithms can be improved in some manner, and having flexibility is paramount.

