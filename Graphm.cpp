#include <iostream>
#include <fstream>
#include <limits>
#include <iomanip>
#include <stack>
#include "Graphm.h"
using namespace std;
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


//---------------------------- GraphM -----------------------------------------
// Constructor. Creates an empty graph with a size of 100
// Parameter:   None
// Return:      None
//-----------------------------------------------------------------------------
GraphM::GraphM(){

    //due to no items being in the graph the size is 0
    size = 0;

    // iterate through every node in the 2d arrays
    for(int i = 0; i <= 100; i++){
        for(int j = 0; j <= 100; j++){
            // -1 is used as a stand in for infinity as distance can
            // never be negative
            T[i][j].dist = -1;
            T[i][j].path = 0;
            // all pathes are marked false
            T[i][j].visited = false;

            //all distances between nodes is 0
            C[i][j] = 0;

        }
    }
} // end of constructor

//---------------------------- buildGraph -------------------------------------
// Constructs a graph that represents the direct distance between all nodes
// Also assigns names to each node that is active
// Parameter: infile - a file stream of the data that is being converted into
//                     a graph.
// Return   : None
//-----------------------------------------------------------------------------
void GraphM::buildGraph(ifstream& infile){
    if(infile.eof() != true) {

        //first line in the file will be the number of nodes
        infile >> size;


        //assign location names to each node
        for (int i = 0; i <= size; i++) {
            data[i].setData(infile);
        }

        int point1, point2, point3;
        //get the first three variables from the first line
        infile >> point1;
        infile >> point2;
        infile >> point3;
        //continue to get variables until the terminal variable (0) is hit
        while (point1 != 0) {
            //assign the distance to the specified node location
            C[point1][point2] = point3;
            infile >> point1;
            infile >> point2;
            infile >> point3;
        }
    }

}// end of BuildGraph

//---------------------------- insertEdge -------------------------------------
// Inserts an edge between two nodes as long as the two nodes are within the
// size of the graph that has already been created.
// Parameter: fromNode - the location of the node that the edge starts at
//            toNode - the location of the node that the edge ends at
//            edgeValue - the value of the edge
// Return :   Bool - whether the edge was inserted or not
//-----------------------------------------------------------------------------
bool GraphM::insertEdge(int fromNode, int toNode, int edgeValue){
    // only insert the edge if there does not exist an edge in that location
    // already
    if(C[fromNode][toNode] == 0 && fromNode <= size && toNode <= size){
        // assign the edge value to the given node path
        C[fromNode][toNode] = edgeValue;
        // return true as the edge was inserted
        return true;
    }
    // return false as the edge was not inserted
    return false;
} // end of insertEdge

//---------------------------- removeEdge -------------------------------------
// Removes an edge from the graph as long as it exists
// Parameter: fromNode - the location of the node that the edge starts at
//            toNode - the location of the node that the edge ends at
// Return :  Bool - whether the edge was removed or not
//-----------------------------------------------------------------------------
bool GraphM::removeEdge(int fromNode, int toNode){
    // only remove the edge if it exists (not equal to 0)
    // and it is within the realm of the current graph
    if(C[fromNode][toNode] != 0 && fromNode <= size && toNode <= size){
        // set edge value to 0 which is the equivalent of it being removed
        C[fromNode][toNode] = 0;
        // rerun findShortestPath to account for removed edge
        findShortestPath();
        // return that the edge was in fact removed
        return true;
    }
    // return that the edge was not removed
    return false;

} // end of removeEdge

//---------------------------- findShortestPath -------------------------------
// Finds the shortest path from each node in the graph to each other node
// in the graph
// Parameter: None
// Return :   None
//-----------------------------------------------------------------------------
void GraphM::findShortestPath(){

    //iterate through each of the nodes
    for(int source = 1; source <= size; source++){
        // the distance from a node to itself is zero so set as such
        T[source][source].dist = 0;
        //initial number of adjacent nodes from source
        int i;
        openNodes(source, i);

        // variable used for keeping track of which nodes have been visited
        // keeps track of what is the smallest node that has been visited
        int outerNum = 0;
        //iterate through while there are still open nodes adjacent to source
        while( i != 0){

            //Finds node with smallest path value
            int v;
            findingOpenNode(source, outerNum, v);

            // insert new dist as long as the T dist is set to infinity (-1)
            // or the current direct dist is less than the dist already at
            // this location
            if (T[source][v].dist < 0 || (T[source][v].dist > C[source][v])){
                //mark that the node has been visited
                T[source][v].visited = true;
                T[source][v].dist = C[source][v];
                //mark from which node this path is marked
                T[source][v].path = source;
            }

            //intial number of adaject nodes from v
            int nodeCheck;
            openNodes(v, nodeCheck);

            // variable used for keeping track of which nodes have been visited
            // keeps track of what is the smallest node that has been visited
            int innerNum = 0;
            while( nodeCheck != 0 ){

                //finds node adjacent to v that has the smallest path
                int w;
                findingOpenNode(v, innerNum, w);

                // assign new dist if this node has not been visited or its
                // distance is less than source to v
                if(T[v][w].visited != true || (T[source][v].dist > C[v][w])){

                    // the direct distance from source to w
                    int direct = T[source][w].dist;
                    int indirect = 0;
                    // indirect distance from source to w
                    // if source to v is -1 then it is not added to v to w
                    if(T[source][v].dist == -1){
                        indirect = C[v][w];
                    }else{
                        indirect = T[source][v].dist + C[v][w];
                    }

                    // distance from source to w is the lesser of the two given
                    // distances
                    T[source][w].dist = minimumDistance(direct, indirect);

                    // if the indirect path was taken mark source to w as visited
                    // and that to get to w source first went to v (the path)
                    if(T[source][w].dist == (T[source][v].dist + C[v][w])){
                        T[source][w].visited = true;
                        T[source][w].path = v;
                    }
                }

                //mark this node as visited so that the next open node can be found
                T[v][w].visited = true;
                //updates the number of adjacent nodes to v
                nodeCheck = nodeCheck-1;
                // updates what the smallest node is that has been visited
                innerNum = w;
            }

            // remarks all the adjacent nodes from v to false so that they can
            // be checked later
            for(int k = 1; k <= size; k++){
                T[v][k].visited = false;
            }
            // updates number of adjacent nodes to source
            i = i - 1;
            // updates what the smallest node is that has been visited
            outerNum = v;

        }
    }
} // end of findShortestPath

//---------------------------- displayAll -------------------------------------
// Displays all possible pathes from a each node to each other nodes
// Parameter: NONE
// Return :   NONE
//-----------------------------------------------------------------------------
void GraphM::displayAll(){
    //header information for display
    cout <<"Description"<<setw(20)<< "From Node"<<setw(10);
    cout << setw(10)<<"To Node"<< setw(12)<< "Dijkstra's"<< setw(10)<< "Path" << endl;

    //iterating through each node
    for(int i = 1; i <= size; i++ ){
        //print the location name of the current node
        cout << data[i] << endl;

        //iterate through each possible node that can be reached
        for(int j = 1; j <= size; j++){
            // distance from a node to itself it irrelevant so don't print
            // and then exit this iteration of the loop
            if(i == j){
                continue;
            }
            //print node that is being travelled from
            cout << setw(31) << i;
            //print the node that is being travelled to
            cout << setw(10) <<j;
            //if there is no path between these nodes than print ---
            //and exit this iteration of the loop
            if(T[i][j].dist == -1){
                cout << setw(12) << "---" << endl;
                continue;

            }else{
                //print the distance between the two nodes
                cout << setw(12) << T[i][j].dist;
            }

            //create a stack that holds the path from i to j
            int path = j;
            int source = i;
            stack <int> pathes;
            pathes.push(j);
            while(path != i) {
                pathes.push(T[source][path].path);
                path = T[i][path].path;


            }
            //print out the path from i to j to the user
            cout << setw(7);
            while(!pathes.empty()){
                cout << pathes.top() << " ";
                pathes.pop();
            }
            cout << endl;
        }
        cout << endl;
    }
} // end of displayAll

//---------------------------- display ----------------------------------------
// Displays to the user the path from a given node to another node.
// If there is no path it will print such to the user
// Parameter: fromNode - the node that is being moved from
//            toNode - the node that is being moved to
// Return: NONE
//-----------------------------------------------------------------------------
void GraphM::display( int fromNode, int toNode ){
    // if there is no path between the nodes print that to the user and exit the function
    if(T[fromNode][toNode].dist == -1){
        cout << "There is no path between " << data[fromNode] << " and " << data[toNode] << endl;
        cout << endl;
        return;
    }
    // prints the nodes that are being moved from and to and the distance between them
    cout << setw(10) << fromNode << setw(10) << toNode;
    cout << setw(10) <<T[fromNode][toNode].dist;

    int path = toNode;
    int source = fromNode;

    stack <int> pathes;
    stack <int> locations;

    //create a stack that holds the path from i to j
    pathes.push(toNode);
    while(path != fromNode) {
        pathes.push(T[source][path].path);
        path = T[fromNode][path].path;
    }
    //copies the stack of path information to then display the node names
    locations = pathes;
    cout << setw(7);
    //prints to the user the path from i to j
    while(!pathes.empty()){
        cout << pathes.top() << " ";
        pathes.pop();
    }
    cout << endl;
    // prints the location name of each node that is visited in the path
    // from i to j
    while(!locations.empty()){
        cout << data[locations.top()] << endl;
        locations.pop();
    }
    cout << endl;

} // end of display

//---------------------------- minimumDistance --------------------------------
// Returns the smaller of the two distances that are supplied
// Parameter: direct - the direct distance from two nodes in the graph
//            indirect - the indirect distance from two nodes in the graph
// Return : int - the smaller of the two distances
//-----------------------------------------------------------------------------
int GraphM::minimumDistance(int direct, int indirect){
    // return indirect if it is smaller than direct or if direct is infinity(-1)
    if(indirect < direct || direct == -1){
        return indirect;
    }
    //otherwise return direct
    return direct;
} // end of minimumDistance

//---------------------------- findingOpenNode --------------------------------
// Finds the smallest open adjacent node from a given node source
// Parameter: source - the source node from which the adjacent nodes are being
//                     looked for.
//            lastNum - the previous smallestNode that was found
//            w - a reference int that will store the smallest node
// Return : NONE
//-----------------------------------------------------------------------------
void GraphM::findingOpenNode(int source, int lastNum, int&w){
    //tracker for finding the smallest node
    int smallestNode = 0;
    //iterate through each possible adjacent node
    for(int i = 1; i <= size; i++){
        //if the smallestNode current location is 0 it is not a path
        // so increment smallestNode to the next adjacent node
        if(C[source][smallestNode] == 0 ){
            smallestNode++;
        }

        // if smallestNode is smaller than the last smallest node
        // increment smallestNode
        if(C[source][smallestNode] <= C[source][lastNum] ){
            smallestNode++;
        }

        // if there is an adjacent node that hasn't been visited yet,
        // is smaller than smallestNode and isn't equal to zero
        // assign that node to smallestNode
        if((C[source][smallestNode] > C[source][i]) ){
            if(C[source][i] != 0 && T[source][i].visited != true){
                smallestNode = i;
            }
        }
    }

    // assign w to the current smallest node
    w = smallestNode;
} // end of findingOpenNode

//---------------------------- openNodes --------------------------------------
// Finds out how adjacent nodes a specified node has
// Parameter: source - the node from which adjacent nodes are being looked at
//            w - a reference int that will store the number of adjacent nodes
// Return : NONE
//-----------------------------------------------------------------------------
void GraphM::openNodes(int source, int &w){
    //initial assumption is that there are 0 adjacent nodes
    // count will keep track of how many there are
    int count = 0;
    // iterate through each possible adjacent node
    for(int i = 1; i <= size; i++){
        // if there is a node path increment count
        if(C[source][i] != 0){
            count++;
        }
    }
    //assign count to w
    w = count;
} // end of openNodes