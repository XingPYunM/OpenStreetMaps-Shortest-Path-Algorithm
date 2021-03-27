// graph.h
// Author: Timothy Villaraza
// Date: 11/21/2020
//
// University of Illinois at Chicago
// CS 251: Fall 2020
// Project #7 - Openstreet Maps
//

#pragma once

#include <iostream>
#include <stdexcept>
#include <iterator>
#include <vector>
#include <map>
#include <set>

using namespace std;

template<typename VertexT, typename WeightT>
class graph {
	private:
	////////////////////////////////////////////////////////////////////////////
	// Private Type Definitions
	////////////////////////////////////////////////////////////////////////////

	typedef typename map<VertexT, map<VertexT, WeightT>>::iterator myMapIterator;
	typedef typename map<VertexT, map<VertexT, WeightT>>::const_iterator myConstMapIterator;

	////////////////////////////////////////////////////////////////////////////
	// Private Member Variables
	////////////////////////////////////////////////////////////////////////////
	
	// map<A vertex, All Neighbors and WeightT between them>
	map<VertexT, map<VertexT, WeightT>> AdjList;

	int nVertices;
	int nEdges;

	public:
	////////////////////////////////////////////////////////////////////////////
	// Constructors
	////////////////////////////////////////////////////////////////////////////

	//
	// Default Constructor
	//
	graph() {
		nVertices = 0;
		nEdges = 0;
	}

	//
	// Copy Constructor
	//
	graph(const graph& other) {
		// TO DO: Does this work?
		this->AdjList = other.AdjList;
		this->nVertices = other.nVertices;
		this->nEdges = other.nEdges;
	}

	////////////////////////////////////////////////////////////////////////////
	// Operators
	////////////////////////////////////////////////////////////////////////////

	//
	// Assignment Operator
	//
	graph& operator=(const graph& other) {
		this->AdjList = other.AdjList;
		this->nVertices = other.nVertices;
		this->nEdges = other.nEdges;

		return *this;
	}

	//
	// Equality Operator
	//
	bool operator==(const graph& other) {
		if (this->AdjList == other.AdjList &&
		this->nVertices == other.nVertices &&
		this->nEdges == other.nEdges) {
			return true;
		}

		return false;
	}

	////////////////////////////////////////////////////////////////////////////
	// Public Functions
	////////////////////////////////////////////////////////////////////////////

	//
	// NumVertices
	//
	// Returns the # of vertices currently in the graph.
	//
	int NumVertices() const {
		return this->nVertices;
	}

	//
	// NumEdges
	//
	// Returns the # of edges currently in the graph.
	//
	int NumEdges() const {
		return this->nEdges;
	}

	//
	// addVertex
	//
	// Adds the vertex v to the graph if there's room, and if so
	// returns true.  If the vertex already exists in the graph,
	// then false is returned.
	//
	bool addVertex(VertexT v) {
		///////////////////////////////////////
		// If the vertex doesn't exist,      //
		// add it to AdjList and return true //
		///////////////////////////////////////

		if (AdjList.find(v) == AdjList.end()) {
			map<VertexT, WeightT> emptyNeighborsMap;
			AdjList.emplace(v, emptyNeighborsMap);

			this->nVertices++;
			return true;
		}

		return false;
	}

	//
	// addEdge
	//
	// Adds the edge (from, to, weight) to the graph, and returns
	// true. If the vertices do not exist, false is returned.
	//
	// NOTE: if the edge already exists, the existing edge weight
	// is overwritten with the new edge weight.
	//
	bool addEdge(VertexT from, VertexT to, WeightT weight) {
		///////////////////////////
		// Create or Update Edge //
		///////////////////////////

		myMapIterator fromMap = this->AdjList.find(from);
		myMapIterator toMap = this->AdjList.find(to);

		// Check if the "from" and "to" vertices exist
		if (fromMap == AdjList.end() || toMap == AdjList.end()) {
			return false;
		}

		// Abstract details using references
		// TO DO: Ask if this is safe to access AdjList.end()->second, so that
		// I can put it on the top instead
		map<VertexT, WeightT> &fromNeighbors = fromMap->second;

		//////////////////////////////////////////////////
		// Check if Edge Exists Between "from" and "to" //
		//////////////////////////////////////////////////

		if (fromNeighbors.find(to) == fromNeighbors.end()) {
			this->nEdges++;  // Increment Edge Count
		}

		////////////////////////////////
		// Create or Update Edge Data //
		////////////////////////////////

		fromNeighbors[to] = weight;

		return true;
	}

	//
	// getWeight
	//
	// Returns the weight associated with a given edge.  If
	// the edge exists, the weight is returned via the reference
	// parameter and true is returned.  If the edge does not
	// exist, the weight parameter is unchanged and false is
	// returned.
	//
	bool getWeight(VertexT from, VertexT to, WeightT& weight) const {
		//////////////////////////////////////////////////
		// Check if vertex "from" and vertex "to" exist //
		//////////////////////////////////////////////////

		// Save location of "from" and "to" in map if it exists
		myConstMapIterator fromMap = this->AdjList.find(from);
		myConstMapIterator toMap = this->AdjList.find(to);

		// Check if "from" and "to" exist.
		if (fromMap == AdjList.end() || toMap == AdjList.end()) {
			return false;
		}

		// Abstract details using references
		// fromMap->first is key ("from") and fromMap->second is the AdjList
		const map<VertexT, WeightT> &fromNeighbors = fromMap->second;

		//////////////////////////////////////////////////////////////////
		// Check if an edge between "from" vertex and "to" vertex exist //
		//////////////////////////////////////////////////////////////////

		if (fromNeighbors.find(to) == fromNeighbors.end()) {
			return false;
		}

		//////////////////////////////////////////////
		// Update Pass-By-Reference weight Argument //
		//////////////////////////////////////////////

		weight = fromNeighbors.at(to);

		return true;
	}

	//
	// neighbors
	//
	// Returns a set containing the neighbors of v, i.e. all
	// vertices that can be reached from v along one edge.
	// Since a set is returned, the neighbors are returned in
	// sorted order; use foreach to iterate through the set.
	//
	set<VertexT> neighbors(VertexT v) const {
		myConstMapIterator fromVertexAndMapPair = AdjList.find(v);
		set<VertexT>  S;  // Return Value

		// For Each "From" Vertex
		if (fromVertexAndMapPair != AdjList.end()) {
			// For Each Neighbor of "From"
			for (auto neighborVertexAndWeightPair : fromVertexAndMapPair->second) {
				// Insert the neighbor into the set
				S.insert(neighborVertexAndWeightPair.first);
			}
		}

		return S;
	}

	//
	// getVertices
	//
	// Returns a vector containing all the vertices currently in
	// the graph.
	//
	vector<VertexT> getVertices() const {
		vector<VertexT> vertices;

		for (auto currVertex : AdjList) {
			vertices.push_back(currVertex.first);
		}

		return vertices;
	}

	//
	// dump
	//
	// Dumps the internal state of the graph for debugging purposes.
	//
	// Example:
	//    graph<string,int>  G(26);
	//    ...
	//    G.dump(cout);  // dump to console
	//
	void dump(ostream& output) const {
		output << "***************************************************" << endl;
		output << "********************* GRAPH ***********************" << endl;

		output << "**Num vertices: " << this->NumVertices() << endl;
		output << "**Num edges: " << this->NumEdges() << endl;

		output << endl;
		output << "**Vertices:" << endl;
		for (auto currPair : AdjList) {
		  output << currPair.first << endl;
		}

		output << endl;
		output << "**Edges:" << endl;
		for (auto currPair : AdjList) {
			output << "Vertex " << currPair.first << "'s Neighbors: ";

			output << endl;
			output << "    ";

			for (auto currNeighborPairs : currPair.second) {
				output << "(" << currNeighborPairs.first << ", "
				<< currNeighborPairs.second << "), ";
			}

			output << endl;
		}

		output << endl;
		output << "**************************************************" << endl;
	}
};
