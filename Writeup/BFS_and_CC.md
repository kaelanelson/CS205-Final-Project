# Breadth First Search

## Sequential Algorithm
For a graph, g, and starting node, s, the algorithm is as follows:
1.	Create an array, V, that records with nodes of g have been visited. Mark the entry at node s as visited.
2.	Create a queue, Q, that contains the next nodes to be visited. Assign the first node to be visited as s.
3.	While Q is not empty:
a.	Access the first node in Q, erase it from Q, and  check if this node has already been visited, as marked in V.
b.	If not visited, add it to the end of Q, and mark as visited in V
Note: Print out order of traversal by printing out the next element at the front of Q

## Parallelization with OpenMP 


# Closeness Centrality

Closeness Centrality finds the most central node in a graph. It is very useful for analyzing large networks, such as social networks. 

## Sequential algorithm
For each node in a connected graph g, with starting vertex, s:
1 . Find the minimum distances to all other nodes, or path, from s. Distance is calculated through a minimum spanning tree algorithm - we use prims algorithm. 
2.  Sum all of these distances. 
3. If the sum is greater than 0 and the graph has more than 1 node, calculate closeness centrality by dividing the number of nodes by this sum. Otherwise, set the closeness measure for this node to 0. 
The node with the smallest value is the most central.

## Parallelization with MPI

## Hybrid Parallelization with MPI and OpenMP
Closeness centrality algorithm was not designed to be computationally efficient on large graph structures because it requires visiting each node and find the minimum path. From our implementation of Prim?s algorithm, we can see that parallelization of this alone is a non-trivial task. Previous literature suggests that speed up is mainly achieved though parallelizing the minimum path algorithm used to find distance (prim?s algorithm in our case), and then using implementing a hybrid parallelization. Thus, for MPI parallelization of closeness centrality algorithm, we first use our MPI parallelized prims algorithm to first see how much it does speed up closeness centrality alone. See prim?s algorithm section for how we parallelized with MPI.


Then, we created the hybrid version with OpenMP by updating loops to calculate distance and centrality in parallel.