# directed-weighted-graph-dynamic-shortest-path-cpp
A C++ program to parse a graph from a file, or from stdin, and find the shortest path between two points, or from every point to every other point, using a dynamic programming algorithm.


## How it works

The program represents a graph as a matrix of total distances between nodes, and the next node to go to to get from one node to another. The program uses a dynamic programming algorithm to ensure that whenever a new edge is added, the next hop and total weight represent the shortest possible path from every node to every other node.

When a new edge is added, the program goes through every pair of nodes, and checks to see if it is faster to use the newly added edge, than to use the current path. So if a new edge is added from point `a` to point `b` with weight `w`, the algorithm will check for every pair of nodes `n`, `m`, if the route from `n` to `a` to `b` to `m` is shorter than the current route from `n` to `m`, if so, it updates the total distance to the distance from `n` to `a` plus `w` plus the distance form `b` to `m`, and the next hop to `a` if `a != n`, otherwise `b`.

## Running the program

To build the program run the command `make bin/main`.
To test the program with a graph europe run the command `make europe_test`.
To check for memory leaks on MacOS run the command `make memtest`.
