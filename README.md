🔌 Power Grid Analyzer (COL106 Assignment 6)
📖 Overview
This C++ program models a national power grid as an undirected graph, where:

Cities are nodes.
Transmission lines are edges.
It identifies critical transmission lines (bridges) and efficiently answers queries about how many such important lines exist between two cities using a bridge tree and Lowest Common Ancestor (LCA) approach.

🚀 Features
Detects critical lines (bridges) using Tarjan’s algorithm.
Constructs a bridge tree from the original graph.
Uses binary lifting to preprocess for fast LCA queries.
Answers number of important transmission lines between two cities in O(log n) time.

🛠️ Dependencies
Compatable Compilers-'g++','clang'
Standard Template Library(STL)- used fro vectors, sets, maps, etc. 
