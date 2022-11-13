// A C++ Program to implement A* Search Algorithm

// Code Source: https://www.geeksforgeeks.org/a-search-algorithm/

#include "math.h"
#include <array>
#include <chrono>
#include <cstring>
#include <iostream>
#include <queue>
#include <set>
#include <stack>
#include <tuple>
#include <bits/stdc++.h>
#include <limits.h>
using namespace std;

vector<pair<int, int>> maze_path;

 
// Creating a shortcut for int, int pair type
typedef pair<int, int> Pair;
// Creating a shortcut for tuple<int, int, int> type
typedef tuple<double, int, int> Tuple;
 
// A structure to hold the necessary parameters
struct cell {
    // Row and Column index of its parent
    Pair parent;
    // f = g + h
    double f, g, h;
    cell()
        : parent(-1, -1)
        , f(-1)
        , g(-1)
        , h(-1)
    {
    }
};
 
// A Utility Function to check whether given cell (row, col)
// is a valid cell or not.
template <size_t ROW, size_t COL>
bool isValid(const array<array<int, COL>, ROW>& grid,
             const Pair& point)
{ // Returns true if row number and column number is in
  // range
    if (ROW > 0 && COL > 0)
        return (point.first >= 0) && (point.first < ROW)
               && (point.second >= 0)
               && (point.second < COL);
 
    return false;
}
 
// A Utility Function to check whether the given cell is
// blocked or not
template <size_t ROW, size_t COL>
bool isUnBlocked(const array<array<int, COL>, ROW>& grid,
                 const Pair& point)
{
    // Returns true if the cell is not blocked else false
    return isValid(grid, point)
           && grid[point.first][point.second] == 1;
}
 
// A Utility Function to check whether destination cell has
// been reached or not
bool isDestination(const Pair& position, const Pair& dest)
{
    return position == dest;
}
 
// A Utility Function to calculate the 'h' heuristics.
double calculateHValue(const Pair& src, const Pair& dest)
{
    // h is estimated with the two points distance formula
    return sqrt(pow((src.first - dest.first), 2.0)
                + pow((src.second - dest.second), 2.0));
}
 
// A Utility Function to trace the path from the source to
// destination
template <size_t ROW, size_t COL>
void tracePath(
    const array<array<cell, COL>, ROW>& cellDetails,
    const Pair& dest)
{
    printf("\nThe Path is ");
 
    stack<Pair> Path;
 
    int row = dest.first;
    int col = dest.second;
    Pair next_node = cellDetails[row][col].parent;
    do {
        Path.push(next_node);
        next_node = cellDetails[row][col].parent;
        row = next_node.first;
        col = next_node.second;
    } while (cellDetails[row][col].parent != next_node);
 
    Path.emplace(row, col);
    while (!Path.empty()) {
        Pair p = Path.top();
        Path.pop();
        printf("-> (%d,%d) ", p.first, p.second);
        maze_path.push_back(p);
    }
    printf("\n");
}
 
// A Function to find the shortest path between a given
// source cell to a destination cell according to A* Search
// Algorithm
template <size_t ROW, size_t COL>
void aStarSearch(const array<array<int, COL>, ROW>& grid,
                 const Pair& src, const Pair& dest)
{
    // If the source is out of range
    if (!isValid(grid, src)) {
        printf("Source is invalid\n");
        return;
    }
 
    // If the destination is out of range
    if (!isValid(grid, dest)) {
        printf("Destination is invalid\n");
        return;
    }
 
    // Either the source or the destination is blocked
    if (!isUnBlocked(grid, src)
        || !isUnBlocked(grid, dest)) {
        printf("Source or the destination is blocked\n");
        return;
    }
 
    // If the destination cell is the same as source cell
    if (isDestination(src, dest)) {
        printf("We are already at the destination\n");
        return;
    }
 
    // Create a closed list and initialise it to false which
    // means that no cell has been included yet This closed
    // list is implemented as a boolean 2D array
    bool closedList[ROW][COL];
    memset(closedList, false, sizeof(closedList));
 
    // Declare a 2D array of structure to hold the details
    // of that cell
    array<array<cell, COL>, ROW> cellDetails;
 
    int i, j;
    // Initialising the parameters of the starting node
    i = src.first, j = src.second;
    cellDetails[i][j].f = 0.0;
    cellDetails[i][j].g = 0.0;
    cellDetails[i][j].h = 0.0;
    cellDetails[i][j].parent = { i, j };
 
    /*
    Create an open list having information as-
    <f, <i, j>>
    where f = g + h,
    and i, j are the row and column index of that cell
    Note that 0 <= i <= ROW-1 & 0 <= j <= COL-1
    This open list is implemented as a set of tuple.*/
    std::priority_queue<Tuple, std::vector<Tuple>,
                        std::greater<Tuple> >
        openList;
 
    // Put the starting cell on the open list and set its
    // 'f' as 0
    openList.emplace(0.0, i, j);
 
    // We set this boolean value as false as initially
    // the destination is not reached.
    while (!openList.empty()) {
        const Tuple& p = openList.top();
        // Add this vertex to the closed list
        i = get<1>(p); // second element of tuple
        j = get<2>(p); // third element of tuple
 
        // Remove this vertex from the open list
        openList.pop();
        closedList[i][j] = true;
        /*
                Generating all the 8 successor of this cell
                        N.W N N.E
                        \ | /
                        \ | /
                        W----Cell----E
                                / | \
                        / | \
                        S.W S S.E
 
                Cell-->Popped Cell (i, j)
                N --> North     (i-1, j)
                S --> South     (i+1, j)
                E --> East     (i, j+1)
                W --> West         (i, j-1)
                N.E--> North-East (i-1, j+1)
                N.W--> North-West (i-1, j-1)
                S.E--> South-East (i+1, j+1)
                S.W--> South-West (i+1, j-1)
        */
        for (int add_x = -1; add_x <= 1; add_x++) {
            for (int add_y = -1; add_y <= 1; add_y++) {
                Pair neighbour(i + add_x, j + add_y);
                // Only process this cell if this is a valid
                // one
                if (isValid(grid, neighbour)) {
                    // If the destination cell is the same
                    // as the current successor
                    if (isDestination(
                            neighbour,
                            dest)) { // Set the Parent of
                                     // the destination cell
                        cellDetails[neighbour.first]
                                   [neighbour.second]
                                       .parent
                            = { i, j };
                        printf("The destination cell is "
                               "found\n");
                        tracePath(cellDetails, dest);
                        return;
                    }
                    // If the successor is already on the
                    // closed list or if it is blocked, then
                    // ignore it.  Else do the following
                    else if (!closedList[neighbour.first]
                                        [neighbour.second]
                             && isUnBlocked(grid,
                                            neighbour)) {
                        double gNew, hNew, fNew;
                        gNew = cellDetails[i][j].g + 1.0;
                        hNew = calculateHValue(neighbour,
                                               dest);
                        fNew = gNew + hNew;
 
                        // If it isn’t on the open list, add
                        // it to the open list. Make the
                        // current square the parent of this
                        // square. Record the f, g, and h
                        // costs of the square cell
                        //             OR
                        // If it is on the open list
                        // already, check to see if this
                        // path to that square is better,
                        // using 'f' cost as the measure.
                        if (cellDetails[neighbour.first]
                                       [neighbour.second]
                                           .f
                                == -1
                            || cellDetails[neighbour.first]
                                          [neighbour.second]
                                              .f
                                   > fNew) {
                            openList.emplace(
                                fNew, neighbour.first,
                                neighbour.second);
 
                            // Update the details of this
                            // cell
                            cellDetails[neighbour.first]
                                       [neighbour.second]
                                           .g
                                = gNew;
                            cellDetails[neighbour.first]
                                       [neighbour.second]
                                           .h
                                = hNew;
                            cellDetails[neighbour.first]
                                       [neighbour.second]
                                           .f
                                = fNew;
                            cellDetails[neighbour.first]
                                       [neighbour.second]
                                           .parent
                                = { i, j };
                        }
                    }
                }
            }
        }
    }
 
    // When the destination cell is not found and the open
    // list is empty, then we conclude that we failed to
    // reach the destination cell. This may happen when the
    // there is no way to destination cell (due to
    // blockages)
    printf("Failed to find the Destination Cell\n");
}
 
// // Driver program to test above function
// int main()
// {
//     /* Description of the Grid-
//     1--> The cell is not blocked
//     0--> The cell is blocked */
//     array<array<int, 10>, 9> grid{
//         { { { 1, 0, 1, 1, 1, 1, 0, 1, 1, 1 } },
//           { { 1, 1, 1, 0, 1, 1, 1, 0, 1, 1 } },
//           { { 1, 1, 1, 0, 1, 1, 0, 1, 0, 1 } },
//           { { 0, 0, 1, 0, 1, 0, 0, 0, 0, 1 } },
//           { { 1, 1, 1, 0, 1, 1, 1, 0, 1, 0 } },
//           { { 1, 0, 1, 1, 1, 1, 0, 1, 0, 0 } },
//           { { 1, 0, 0, 0, 0, 1, 0, 0, 0, 1 } },
//           { { 1, 0, 1, 1, 1, 1, 0, 1, 1, 1 } },
//           { { 1, 1, 1, 0, 0, 0, 1, 0, 0, 1 } } }
//     };
 
//     // Source is the left-most bottom-most corner
//     Pair src(8, 0);
 
//     // Destination is the left-most top-most corner
//     Pair dest(0, 0);
 
//     aStarSearch(grid, src, dest);
 
//     return 0;
// }


const matrixToAdjList = (matrix) => {
  const graph = {};
  const R, C = GRID_SIZE,GRID_SIZE;
  for (let r = 0; r < R; r++) {
    for (let c = 0; c < C; c++) {
      const node = C*r+c;
      // if (!(node in graph)) graph[node] = {} this check is redundant. we visit each node only once. I assume that the node names are unique, otherwise this algo wouldn't work.
      graph[node] = {}
      if (c < C-1) {
          graph[node][matrix[r][c+1]] = 10 // move right
      }
      if (r < R-1) {
          graph[node][matrix[r+1][c]] = 25 // move down
      }
    }
  }
  return graph;
};


 
// Number of vertices in the graph
#define V 9
 
// A utility function to find the vertex with minimum
// distance value, from the set of vertices not yet included
// in shortest path tree
int minDistance(int dist[], bool sptSet[])
{
 
    // Initialize min value
    int min = INT_MAX, min_index;
 
    for (int v = 0; v < V; v++)
        if (sptSet[v] == false && dist[v] <= min)
            min = dist[v], min_index = v;
 
    return min_index;
}
 
// A utility function to print the constructed distance
// array
void printSolution(int dist[])
{
    cout << "Vertex \t Distance from Source" << endl;
    for (int i = 0; i < V; i++)
        cout << i << " \t\t\t\t" << dist[i] << endl;
}
 
// Function that implements Dijkstra's single source
// shortest path algorithm for a graph represented using
// adjacency matrix representation
void dijkstra(const array<array<int, COL>, ROW>& grid,
                 const Pair& src, const Pair& dest) //(int graph[V][V], int src)
{
	int V=GRID_SIZE;
	
    int dist[V]; // The output array.  dist[i] will hold the
                 // shortest
    // distance from src to i
 
    bool sptSet[V]; // sptSet[i] will be true if vertex i is
                    // included in shortest
    // path tree or shortest distance from src to i is
    // finalized
 
    // Initialize all distances as INFINITE and stpSet[] as
    // false
    for (int i = 0; i < V; i++)
        dist[i] = INT_MAX, sptSet[i] = false;
 
    // Distance of source vertex from itself is always 0
    dist[src] = 0;
 
    // Find shortest path for all vertices
    for (int count = 0; count < V - 1; count++) {
        // Pick the minimum distance vertex from the set of
        // vertices not yet processed. u is always equal to
        // src in the first iteration.
        int u = minDistance(dist, sptSet);
 
        // Mark the picked vertex as processed
        sptSet[u] = true;
 
        // Update dist value of the adjacent vertices of the
        // picked vertex.
        for (int v = 0; v < V; v++)
 
            // Update dist[v] only if is not in sptSet,
            // there is an edge from u to v, and total
            // weight of path from src to  v through u is
            // smaller than current value of dist[v]
            if (!sptSet[v] && graph[u][v]
                && dist[u] != INT_MAX
                && dist[u] + graph[u][v] < dist[v])
                dist[v] = dist[u] + graph[u][v];
    }
 
    // print the constructed distance array
    printSolution(dist);
}
 
// driver's code
int main()
{
 
    /* Let us create the example graph discussed above */
    int graph[V][V] = { { 0, 4, 0, 0, 0, 0, 0, 8, 0 },
                        { 4, 0, 8, 0, 0, 0, 0, 11, 0 },
                        { 0, 8, 0, 7, 0, 4, 0, 0, 2 },
                        { 0, 0, 7, 0, 9, 14, 0, 0, 0 },
                        { 0, 0, 0, 9, 0, 10, 0, 0, 0 },
                        { 0, 0, 4, 14, 10, 0, 2, 0, 0 },
                        { 0, 0, 0, 0, 0, 2, 0, 1, 6 },
                        { 8, 11, 0, 0, 0, 0, 1, 0, 7 },
                        { 0, 0, 2, 0, 0, 0, 6, 7, 0 } };
 
    // Function call
    dijkstra(graph, 0);
 
    return 0;
}
