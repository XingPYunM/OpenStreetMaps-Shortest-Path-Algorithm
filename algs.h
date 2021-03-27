/*algs.h*/

//
// University of Illinois at Chicago
// CS 251: Fall 2020
// Project #7 - Openstreet Maps
//

#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <limits>

#include "graph.h"

using namespace std;

const double INF = numeric_limits<double>::max();

vector<long long> Dijkstra(graph<long long, double>& G,
  long long startV,
  map<long long, double>& distances,
  map<long long, long long>& predecessors);
