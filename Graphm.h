#include <iostream>
#include <fstream>
#include <limits>
#include "nodedata.h"
#ifndef GRAPH_GRAPHM_H
#define GRAPH_GRAPHM_H

//-----------------------------------------------------------------------------
// Kieran Coito
// 27/01/2019
// 03/02/2019
//-----------------------------------------------------------------------------
// GraphM class:
// This class creates a graph of nodes that represent locations, and the
// distance between these locations. It then performs Dijkstra's algorihm to
// find the shortest path between each node. It can print to the user all
// pathes from each node to each other node. It is also able to print a
// specific path and then print each location that is moved through along the
// the path.
//-----------------------------------------------------------------------------
// Assumptions:
// - The graph can not contain anymore than 100 nodes.
// - All distances between nodes are postive.
// - All nodes are positive numbers, negative nodes are not possible
// - The shortest path algorithm only takes into account distance and not the
//   number of nodes visited.
// - The file that is used to create the graph is formatted specfically for
//   this class.
// - Edges can only be inserted and removed within the graph that has already
//   been created. So if the size is 5, an edge cant be inserted at 6.
//
//-----------------------------------------------------------------------------
class GraphM {

public:
    GraphM();
    void buildGraph(ifstream& infile);
    bool insertEdge(int fromNode, int toNode, int edgeValue);
    bool removeEdge(int fromNode, int toNode);
    void findShortestPath();
    void displayAll();
    void display(int fromNode, int toNode);

private:

    struct TableType {
        bool visited;          // whether node has been visited
        int dist;              // shortest distance from source known so far
        int path;              // previous node in path of min dist
    };

    NodeData data[101];              // data for graph nodes
    int C[101][101];            // Cost array, the adjacency matrix
    int size;                             // number of nodes in the graph
    TableType T[101][101];      // stores visited, distance, path

    int minimumDistance(int direct, int indirect);
    void findingOpenNode(int source, int lastNum, int &w);
    void openNodes(int source, int &i);


};


#endif //GRAPH_GRAPHM_H
