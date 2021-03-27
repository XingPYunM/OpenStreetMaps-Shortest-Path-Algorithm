/*algs.cpp*/

//
// University of Illinois at Chicago
// CS 251: Fall 2020
// Project #7 - Openstreet Maps
//

#include <iostream>
#include <vector>
#include <queue>
#include <set>
#include <stack>
#include <map>
#include <string>
#include <fstream>
#include <utility>  /*pair*/
#include <limits>   /*numeric_limits*/

#include "graph.h"
#include "algs.h"

using namespace std;


//
// function object that given two pairs, will prioritize based
// on the values
//
class prioritize
{
public:
  bool operator()(const pair<long long, double>& p1, 
                  const pair<long long, double>& p2) const
  {
    //
    // if you want smaller values @ the end, return true if 
    // p1's value < p2's value.  if you want larger values
    // @ the end, return true if p1's value > p2's value.
    //
    if (p1.second > p2.second)
      return true;
    else if (p1.second < p2.second)
      return false;
    else  // values are the same, decide based on keys:
      return p1.first > p2.first;
  }
};


//
// Dijkstra:
//
// Performs Dijkstra's shortest weighted path algorithm from
// the given start vertex.  Returns a vector of vertices in
// the order they were visited, along with a map of (string,int)
// pairs where the string is a vertex V and the int is the 
// distance from the start vertex to V; if no such path exists,
// the distance is INF (defined in util.h).
//
vector<long long> Dijkstra(graph<long long, double>& G,
  long long startV,
  map<long long, double>& distances,
  map<long long, long long>& predecessors)
{
  vector<long long>  visited;
  set<long long>     visitedSet;
  priority_queue<pair<long long,double>,
    vector<pair<long long,double>>,
    prioritize>   unvisitedQueue;

  //
  // initialize data structures are per Dijkstra's alg:
  //
  auto vertices = G.getVertices();
  for (auto v : vertices)
  {
    unvisitedQueue.push(make_pair(v, INF));
    distances[v] = INF;
    predecessors[v] = -1;  // an invalid vertex:
  }

  //
  // we start from startV:
  //
  distances[startV] = 0;
  //
  // whenever we update the distances array, we also need
  // to update the priority queue:
  //
  unvisitedQueue.push(make_pair(startV, 0)); 

  //
  // run algorithm until no more vertices to visit:
  //
  while (!unvisitedQueue.empty())
  {
    //
    // Pop the vertex from the queue with the *minimum* distance:
    //
    auto kvpair = unvisitedQueue.top();
    unvisitedQueue.pop();

    auto currV = kvpair.first;  // vertex:

    //
    // is currV reachable from src?  If not, there's no point
    // in continuing alg:
    //
    if (distances[currV] == INF)  // unreachable:
      break;
    else if (kvpair.second == INF)  // remaining vertices in queue all INF:
      break;
    else if (visitedSet.count(currV) > 0)  // already visited?
      continue;  // if so, skip
    else  // visit and process below:
    {
      visited.push_back(currV);
      visitedSet.insert(currV);
    }

    //
    // visit adjacent nodes:
    //
    auto adjacent = G.neighbors(currV);

    for (auto adjV : adjacent)
    {
      double weight;

      if (!G.getWeight(currV, adjV, weight))
      {
        cout << "**Error: internal error, unable to obtain weight?" << endl;
        continue;
      }

      double altDist = distances[currV] + weight;

      if (altDist < distances[adjV])  // found a less-expensive path:
      {
        //
        // update distances, and update priority queue:
        //
        distances[adjV] = altDist;
        unvisitedQueue.push(make_pair(adjV, altDist));
        predecessors[adjV] = currV;
      }
    }

  }//while

  //
  // done, return the visited vertices (the distances are
  // returned via reference parameter):
  //
  return visited;
}
