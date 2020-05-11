# Problem

Graph analytics, or network analysis, is an area of data analysis concerned with describing and investigating the relationships between data points. While many will think of its use for analysis of online social networks such as Facebook or Twitter, graph analytics can be used in a variety of domains: detecting fraud, identifying points of weakness in infrastructure networks like power or water grids, identifying key characters in narrative texts, analyzing road coverage, mapping the interaction between physiological systems like the heart and brain, etc. Thus, algorithms that create, traverse, and analyze graphs are helpful for a diverse set of problems.

## Graph Terminology 

For those unfamiliar with graphs, here are some terms we will be using throughout our writeup.

- **Node** **(or vertex)**: The fundamental unit of interest in a graph. For example, in a graph of the Facebook social network, the node is an individual person. 
- **Edge:** The connection between two nodes. Also referred to as links or lines. In our Facebook example, we can think of two individuals who are friends as two nodes connected by an edge. 
  - Edges can be **undirected** or **directed.** The example above describes an undirected edge. An example of directed edges would be links to and from web pages. Page *A* can link to page *B*, but that does not necessarily mean that page *B* links to page *A*.
  - Edges can have **weights** that describe the strength of the connection. For example, we might want to weigh the edge between two best friends who talk every day higher than acquaintances who only communicate once a year. 

## Challenges

Despite the long history of graph theory and network analysis, there are still many challenges involved in carrying out graph analytics. One major challenge is that graph databases can be very large, on the order of millions of nodes and trillions of edges. We want to perform operations on these graphs quickly. Even smaller graphs, because of their representation and the computations involved, can result in non-trivial runtime for algorithms. Thus, runtimes may vary drastically depending on the representation of the graph. Additionally, some graph algorithms have runtime complexities on the order of high degree polynomial or exponential. Given the size of many graph databases, this can be incredibly slow, or potentially infeasible given limited resources.

## Algorithms

To investigate how parallel computing can solve these challenges, we picked seven important graph algorithms to parallelize. For more information on these algorithms, please see [Model and Data](ModelAndData.html).

- [Graph Representation as an Adjacency Matrix]
- [Shortest Path Calculation using Breath First Search](BFS.html)
- [Minimum Spanning Tree](MST.html)
- [Degree Centrality] 
- [Closeness Centrality](Closeness_Centrality.html)
- [PageRank](PageRank.html)

## Need for High Performance Computing and Big Data

### High Performance Computing

- The bottleneck for some algorithms is the need for a lot of computation. For example, the PageRank algorithm is essentially a loop of matrix multiplications. As the dimensions of the matrices are determined by the number of nodes, even a graph with nodes on the order of thousands can be non-trivial to run. 
- There are varying workloads within different parts of the graph. Graphs that have uniform density can be evenly divided by node to ensure balanced workloads. Meanwhile, sparse graphs with subsections that are more dense than other might require particular attention for creating balanced workloads across processors. 

### Big Data

- **Volume**: There is a lot of data that is available and useful to analyze through graph algorithms. For example, there is a plethora of social network data available through Twitter with 467 million Tweets on Stanford Large Network Dataset Collection. This creates a a high data access to computation ratio suggesting that the problem may be more data intensive than computationally expensive for some algorithms. 
- **Velocity**: Within these large datasets, data is generated daily. (Imagine how many tweets are generated within a day, or even just in one hour.) Thus, datasets are expected to keep growing in size or updated to include newer data points.
- **Variety**: There is a need to handle variety since datasets can have different types of structures. For example, the neurons in the brain are densely connected while friendship networks are sparse, having a small number of connections relative to the number of potential connections. As described above, networks can also be directed or undirected as well as having weights. 

