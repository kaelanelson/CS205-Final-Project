# CS 205 Final Project: Parallel Graph Algorithms

## Spring 2020

## Group 12

- Kyra Ballard
- Kaela Nelson
- Simon Warchol
- Paulina Toro Isaza

## Overview

Graph analytics, or network analysis, is an area of data analysis concerned with describing and investigating the relationships between data points. While many will think of its use for analysis of online social networks such as Facebook or Twitter, graph analytics can be used in a variety of domains: detecting fraud, identifying points of weakness in infrastructure networks like power or water grids, identifying key characters in narrative texts, analyzing road coverage, mapping the interaction between physiological systems like the heart and brain, etc. Thus, algorithms that create, traverse, and analyze graphs are helpful for a diverse set of problems.

To investigate how parallel computing can solve these challenges, we picked seven important graph algorithms to parallelize.

- **Graph Representation as an Adjacency Matrix**: Most graph data is provided in a format where each record is an edge between two nodes. In order to use this information, the data must be transformed into a graph representation such as an adjacency matrix. 
- **Shortest Path Calculation using Breath First Search**: We are often interested in finding the shortest path between two nodes in a graph. An example use case is finding the shortest path between two intersections on a road map. 
- **Minimum Spanning Tree:** We calculate the tree of minimum weight that touches every edge in a graph. For example, if we want to install cablelines that cover a particular neighbor, we want to ensure that we use reach every house in the most efficient way possible. Finding a minimum spanning tree is also often a precursor for other analyses such as centrality. As this is a graph calculation for which many different implementations exist, we will parallelize and compare two different algorithms. 
  - **Prims**
  - **Kruskals**
- **Degree Centrality:** A simple measure of the most important (or central) nodes in a graph is that of the number of edges, or degrees, for each node. 
- **Closeness Centrality**: Another common measure of centrality defines a central node as one that is closer to all other nodes. 
- **PageRank**: Initially developed for ranking web pages, this algorithm can be applied to any graph. It measures centrality through a recursive definition: important nodes are those that are connected to from other important nodes.

## How to Use

For a complete list of instructions on how to use the programs found in this repository, please see [How To Run]() on the project website. 

## Table of Contents
1. [Problem Statement](/Writeup/ProblemStatement.md)
2. [Solution](/Writeup/Solution.md)
3. [Model and Data](/Writeup/ModelAndData.md)
4. [Specs](/Writeup/Specs.md)
5. [How To Run](/Writeup/HowToRun.md)
6. [BFS](/Writeup/BFS.md)
7. [Closeness Centrality](/Writeup/Closeness_Centrality.md)
8. [MST Algorithms](/Writeup/MST.md)
9. [Page Rank](/Writeup/PageRank.md)
10. [Adjacency Matrix](/Writeup/AdjacencyMatrix.md)
11. [Degree Centrality](/Writeup/DegreeCentrality.md)
12. [Discussion](/Writeup/Discussion.md)