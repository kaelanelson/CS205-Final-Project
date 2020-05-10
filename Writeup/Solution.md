# Solution

## Existing Work

We are not the first to recognize the great need for parallel graph algorithms, and so there already exist many useful libraries for implementing parallel and distributed graph analytics. As data science students who were first introduced to parallel computing through this course, our goal was not to create a solution that would outperform these existing libraries. Rather, our focus was to gain more experience with course concepts and create programs that could serve as a foundation for such a library implementation.

Below are a list of common libraries for parallel and distributed graph analytics. 

- **[Spark GraphX](https://spark.apache.org/graphx/):** Apache Spark provides an API for graphs and parallel graph computations on distributed systems that is available on AWS. 
- **[Parallel Boost Graph Library](https://www.boost.org/doc/libs/1_73_0/libs/graph_parallel/doc/html/index.html)**: An parallel and distributed extension to the Boost Graph Library  in C++ which exploits both coarse and fine-grained parallelism. It is implemented using Bulk Synchronous Parallel MPI. 
- **[SNAP System](https://snap.stanford.edu/snap/index.html)**: The Stanford Network Analysis Platform (from which we obtained our datasets) is described as a "general purpose, high performance system for analysis and manipulation of large networks." Its use is for single multi-core machines and is available for C++ and Python.

## Our Approach

### Type of Application

Because of the the various algorithms implemented, our application can be considered both compute and data-intensive. While there is a high data access to compute ratio for some algorithms like Degree Centrality, others such as PageRank are more compute intensive. 

### Levels of Parallelism

The levels of parallelism are both coarse-grained and fine-grained. At the task level, we applied coarse-grained parallelism using MPI.  At the procedure and loop level, we applied fine-grained parallelism using OpenMP. While we originally planned on a hybrid MapReduce and MPI model, after gaining a deeper understanding of our algorithms and seeing that libraries such as the Parallel Boost Graph Library apply both coarse and fine-grained parallelism, we decided to use a hybrid MPI and OpenMP model. This will also allow us to offer significant improvements for users that do not have access to a distributed system by leveraging multi-threaded cores. 

### Types of Parallelism within Application

The focus is data parallelism as computations on different parts of the graph will be running in parallel. For example, in PageRank, the adjacency matrix representation of the graph is evenly divided row-wise across all processors. Each processor then performs multiplication between the transition probability matrix its rows of the adjacency matrix. 

### Programming Models

We employed a Single Program - Multiple Data programming model as we ran each algorithm as as single program over data that was partitioned and distributed over multiple worker nodes and threads. 

A stretch goal was to develop a Multiple Program - Multiple Data model that could run each algorithm at the same time over the partitioned data, but unfortunately we did not have time to implement this. 

### Infrastructure

##### Amazon Web Services EC2

We developed and tested our programs on two m4.2xlarge instances which provided 8 total vCPUs. This instance type was chosen as as it provides a good compromise between compute and memory efficiency per $-hour, as well as high network performance to ensure that our processors can quickly communicate with each other. For more information, see [Specs](Specs.md). 

