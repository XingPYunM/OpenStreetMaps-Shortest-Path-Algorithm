// Title: application.cpp
// Author: Timothy Villaraza
//
// University of Illinois at Chicago
// CS 251: Fall 2020
// Project #7 - Openstreet Maps
//
// References:
// TinyXML: https://github.com/leethomason/tinyxml2
// OpenStreetMap: https://www.openstreetmap.org
// OpenStreetMap docs:
//   https://wiki.openstreetmap.org/wiki/Main_Page
//   https://wiki.openstreetmap.org/wiki/Map_Features
//   https://wiki.openstreetmap.org/wiki/Node
//   https://wiki.openstreetmap.org/wiki/Way
//   https://wiki.openstreetmap.org/wiki/Relation
//
#include <iostream>
#include <iomanip>		// setprecision
#include <limits>		// For INF definition
#include <string>
#include <vector>
#include <stack>
#include <queue>
#include <map>
#include <cstdlib>
#include <cstring>
#include <cassert>
#include "tinyxml2.h"
#include "graph.h"
#include "dist.h"
#include "osm.h"

using namespace std;
using namespace tinyxml2;

#define INF (numeric_limits<double>::max())

// checkForMatch
//
// Checks if user building query matches the building info passed in.
// First checks the abbreviation, then for a partial name.
// If a match is found, return the current building info then return true,
// else, return false.
//
bool checkForMatch(const BuildingInfo& currBuilding, const string& query,
string &buildingFullName, Coordinates &buildingCoordinates) {
	bool matchFound;

	if (currBuilding.Abbrev == query) {
		matchFound = true;
	} else if (currBuilding.Fullname.find(query) != string::npos) {
		matchFound = true;
	} else {
		matchFound = false;
	}

	if (matchFound) {
		// Save Name
		buildingFullName = currBuilding.Fullname;

		// Save Coords
		buildingCoordinates = currBuilding.Coords;

		return true;
	} else {
		return false;
	}
}

// Prioritize functor
// Gives the priority queue instructions on how to prioritize the incoming pairs
class prioritize {
	public:
	bool operator()(const pair<long long, double> &p1,
					const pair<long long, double> &p2) const {
		if (p1.second > p2.second) {
			return true;
		} else if (p1.second < p2.second) {
			return false;
		} else {  // If distances are equal, sort by ID instead
			return p1.first > p2.first;
		}
	}
};

// Dijkstra's Algorithm w/ Shortest Path Implementation
vector<long long> Dijkstra(
	const graph<long long, double>& G,       // Graph of node IDs and Distances
	const long long startV,				     // Start vertexID
	map<long long, double>& distances, 	     // <vertexID, shortest distance from startV and current vertex>
	map<long long, long long>& predecessors  // <VertexID, Closest Prev Neighbor>
) {
	// Return Value: IDs of all visited verticies during runtime of algoritm
	vector<long long>  visited;

	// Keeps track of the verticies that the Dijkstra's will visit
	priority_queue<pair<long long, double>,
				   vector<pair<long long, double>>,
				   prioritize> unvisitedQueue;

	// Keeps track of visited vertex IDs so none are revisited
	set<long long> visitedSet;

	//////////////////////////////////////////////////////////////////////////
	// Fill unvisited queue with all verticies from graph with distance INF //
	//////////////////////////////////////////////////////////////////////////

	for (auto currVertex : G.getVertices()) {
		unvisitedQueue.push(make_pair(currVertex, INF));
		distances[currVertex] = INF;  // Main loop termination requirement
		predecessors[currVertex] = -1;
	}

	// Add start vertex to distance map
	distances[startV] = 0;
	// Distance from start vertex to itself is 0
	unvisitedQueue.push(make_pair(startV, 0));

	// For each unvisited node in the graph
	//		Calculate the distance between the currentV and startV
	//			If a shorter path was found, update the distance

	// For each node in the unvisited queue
	// NOTE: !unvisitedQueue.empty() condition almost never met
	while (!unvisitedQueue.empty()) {
		////////////////////////////////////////////////
		// Pop the first node from the priority queue //
		////////////////////////////////////////////////

		long long currentV = unvisitedQueue.top().first;
		unvisitedQueue.pop();

		///////////////////////////////////////////////////////////////////
		// while loop depends on current vertex from the unvisited queue //
		///////////////////////////////////////////////////////////////////

		// Base case: All vertices visited, rest of queue has INF distance
		if (distances[currentV] == INF) {
			break;  // Rest of the nodes in unvisitedQueue have INF distance
		} else if (visitedSet.count(currentV) != 0) {  // Visited previously
			continue;  // Skip
		} else {  // Vertex has not been visited yet
			visitedSet.insert(currentV);  // Prevents revisting of node
			visited.push_back(currentV);  // Adds to return value
		}

		////////////////////////////////////////////////////////
		// Update distance and path if shorter path was found //
		////////////////////////////////////////////////////////

		// For each neighbor of the current vertex
		for (auto currNeighbor : G.neighbors(currentV)) {
			double distance;     // Current closest distance found between
								 // startV and currNeighbor

			double altDistance;  // Distance between startV and currNeighbor

			// Grab distance between currentV and it's current neighbor
			G.getWeight(currentV, currNeighbor, distance);
			altDistance = distances.at(currentV) + distance;

			// Update if a shorter path from startV to currNeighbor is found
			if (altDistance < distances[currNeighbor]) {
				distances[currNeighbor] = altDistance;
				unvisitedQueue.push(make_pair(currNeighbor, altDistance));
				predecessors[currNeighbor] = currentV;
			}
		}
	}

	return visited;  // Return all nodes visited
}

//
// findStartAndDest
//
// Looks at all footway nodes and compares distances
// Will return the two nodes closest to the start and end
// destinations 
//
void findStartAndDest(const Coordinates &startBuilding,
                      const Coordinates &destBuilding,
                      Coordinates &startCoords,
                      Coordinates &destCoords,
                      const vector<FootwayInfo> &Footways,
                      const map<long long, Coordinates> &mapNodes) {
	// IDs of closest footway nodes to the start and destination buildings
	// The ID is looked up looked up at the very last step.
	long long destID;
	long long startID;

	// Save lat and lon coordinates for start building
	double startBuildingLat = startBuilding.Lat;
	double startBuildingLon = startBuilding.Lon;

	// Save lat and lon coordinates for dest building
	double destBuildingLat = destBuilding.Lat;
	double destBuildingLon = destBuilding.Lon;

	// Initialize closest distances
	double closestStartDistance = INF;
	double closestDestDistance = INF;

	////////////////////////////////////////////////////////////
	// Search for the closest footway node for both buildings //
	////////////////////////////////////////////////////////////

	// For each footway
	for (auto currFootway : Footways) {
		// For each node in the footway
		for (int i = 0; i < (int)currFootway.Nodes.size(); i++) {
			// Look up the current footway node in the Nodes map
			Coordinates currCoordinates = mapNodes.at(currFootway.Nodes[i]);
			double currLat = currCoordinates.Lat;
			double currLon = currCoordinates.Lon;

			///////////////////////////////////////////////////////////////////
			// Calculate distance between buildings and current footway node //
			///////////////////////////////////////////////////////////////////

			// Distance between start building and current footway node
			double currentStartDistance = distBetween2Points(startBuildingLat,
			startBuildingLon, currLat, currLon);

			// Distance between destination building and current footway node
			double currentDestDistance = distBetween2Points(destBuildingLat,
			destBuildingLon, currLat, currLon);

			/////////////////////////////////////////////////////////
			// If a shorter distance was found, update closest IDs //
			/////////////////////////////////////////////////////////

			// Closer footway node to start building found
			if (currentStartDistance < closestStartDistance) {
				closestStartDistance = currentStartDistance;
				startID = currCoordinates.ID;
			}

			// Closer footway node to destination building found
			if (currentDestDistance < closestDestDistance) {
				closestDestDistance = currentDestDistance;
				destID = currCoordinates.ID;
			}
		}
	}

	/////////////////////////////////////////////////
	// Return closest coordinates to each building //
	/////////////////////////////////////////////////

	startCoords = mapNodes.at(startID);
	destCoords = mapNodes.at(destID);
}

int main() {
	// maps a Node ID to it's coordinates (lat, lon)
	map<long long, Coordinates> Nodes;
	// info about each footway, in no particular order
	vector<FootwayInfo> Footways;
	// info about each building, in no particular order
	vector<BuildingInfo> Buildings;
	XMLDocument xmldoc;
	// Graph<Node IDs, Bidirectional Distance Between>
	graph<long long, double> G;

	cout << "** Navigating UIC open street map **" << endl;
	cout << endl;
	cout << std::setprecision(8);

	string def_filename = "map.osm";
	string filename;

	cout << "Enter map filename> ";
	getline(cin, filename);

	if (filename == "") {
		filename = def_filename;
	}

	//
	// Load XML-based map file
	//
	if (!LoadOpenStreetMap(filename, xmldoc)) {
		cout << "**Error: unable to load open street map." << endl;
		cout << endl;
		return 0;
	}

	//
	// Read the nodes, which are the various known positions on the map:
	//
	int nodeCount = ReadMapNodes(xmldoc, Nodes);

	//
	// Read the footways, which are the walking paths:
	//
	int footwayCount = ReadFootways(xmldoc, Footways);

	//
	// Read the university buildings:
	//
	int buildingCount = ReadUniversityBuildings(xmldoc, Nodes, Buildings);

	//
	// Stats
	//
	assert(nodeCount == (int) Nodes.size());
	assert(footwayCount == (int) Footways.size());
	assert(buildingCount == (int) Buildings.size());

	cout << endl;
	cout << "# of nodes: " << Nodes.size() << endl;
	cout << "# of footways: " << Footways.size() << endl;
	cout << "# of buildings: " << Buildings.size() << endl;

	////////////////////////////
	// Add All Nodes to Graph //
	////////////////////////////

	for (pair<long long, Coordinates> currPair : Nodes) {
		G.addVertex(currPair.second.ID);
	}

	///////////////////////////////////////
	// Add Edges Between Nodes Both Ways //
	///////////////////////////////////////

	//
	//  For each foot way
	//		For each point on the foot way
	//			Calculate the distance between two subsequent nodes
	//			and add a new edge between the two points with the
	//			calculated distance
	//

	// For each foot way
	for (auto currFootway : Footways) {
		// For each point on the foot way
		for (int i = 0; i < (int)currFootway.Nodes.size() - 1; i++) {
			// IDs of two points on map
			long long p1 = currFootway.Nodes[i];
			long long p2 = currFootway.Nodes[i + 1];

			// Lat and Lon of two points
			double p1Lat = Nodes.at(p1).Lat;
			double p1Lon = Nodes.at(p1).Lon;
			double p2Lat = Nodes.at(p2).Lat;
			double p2Lon = Nodes.at(p2).Lon;

			// Calculate Distance
			double distance = distBetween2Points(p1Lat, p1Lon, p2Lat, p2Lon);

			// Add edge both ways
			G.addEdge(p1, p2, distance);
			G.addEdge(p2, p1, distance);
		}
	}

	cout << "# of vertices: " << G.NumVertices() << endl;
	cout << "# of edges: " << G.NumEdges() << endl;
	cout << endl;

	//
	// Navigation from building to building
	//
	string startQuery, destQuery;

	cout << "Enter start (partial name or abbreviation), or #> ";
	getline(cin, startQuery);

	while (startQuery != "#") {
		Coordinates startBuildingCoordinates;
		Coordinates destBuildingCoordinates;

		cout << "Enter destination (partial name or abbreviation)> ";
		getline(cin, destQuery);

		////////////////////////////////////////////////
		// Look for start and destination coordinates //
		////////////////////////////////////////////////

		// Indicates if start and destinations were found
		bool startFound = false;
		bool destFound = false;

		// Saves building names if found
		string startBuildingFullName;
		string destBuildingFullName;

		///////////////////////////////////////////////////////////////
		// Search the building vector for the user inputted name.    //
		// Checks abbreviation, if no match, the finds partial name. //
		///////////////////////////////////////////////////////////////

		for (auto currBuilding : Buildings) {
			// Base Case: Start and Destination Buildings are Valid
			if (startFound && destFound) {
				break;
			}

			// Check if the current building matches the start building query
			if (!startFound) {
				startFound = checkForMatch(currBuilding, startQuery,
				startBuildingFullName, startBuildingCoordinates);
			}

			// Check if the current building matches the dest building query
			if (!destFound) {
				destFound = checkForMatch(currBuilding, destQuery,
				destBuildingFullName, destBuildingCoordinates);
			}
		}

		////////////////////////////////////////////////////////
		// Case: Start and/or Destination Point was not Found //
		////////////////////////////////////////////////////////

		if (!startFound) {
			cout << "Start building not found" << endl;
		} else {
			if (!destFound) {
				cout << "Destination building not found" << endl;
			}
		}

		////////////////////////////////////////////////////////
		// Look for and return path from start to destination //
		////////////////////////////////////////////////////////

		if (startFound && destFound) {
			Coordinates startPointCoords;
			Coordinates destPointCoords;

			////////////////////////////////////////////////////////////////////
			// Find closest footway nodes for start and destination buildings //
			////////////////////////////////////////////////////////////////////

			findStartAndDest(startBuildingCoordinates, destBuildingCoordinates,
			startPointCoords, destPointCoords, Footways, Nodes);

			////////////////////
			// Output Results //
			////////////////////

			// Print Start Building Point
			cout << "Starting point:" << endl;
			cout << " " << startBuildingFullName << endl;
			cout << " (" << startBuildingCoordinates.Lat << ", "
			<< startBuildingCoordinates.Lon << ")" << endl;;

			// Print Destination Building Point
			cout << "Destination point:" << endl;
			cout << " " << destBuildingFullName << endl;
			cout << " (" << destBuildingCoordinates.Lat << ", "
			<< destBuildingCoordinates.Lon << ")" << endl;

			cout << endl;

			// Print Nearest Footway Node to Start Building
			cout << "Nearest start node:" << endl;
			cout << " " << startPointCoords.ID << endl;
			cout << " (" << startPointCoords.Lat << ", "
			<< startPointCoords.Lon << ")";
			cout << endl;

			// Print Nearest Footway Node to Destination Building
			cout << "Nearest destination node:" << endl;
			cout << " " << destPointCoords.ID << endl;
			cout << " (" << destPointCoords.Lat << ", "
			<< destPointCoords.Lon << ")";
			cout << endl;

			cout << endl;

			//////////////////////////////
			// Run dijkstra's algorithm //
			//////////////////////////////

			cout << "Navigating with Dijkstra..." << endl;
			map<long long, double> distances;
			map<long long, long long>predecessors;
			vector<long long> visited = Dijkstra(G, startPointCoords.ID,
										distances, predecessors);

			double travelDistance = distances.at(destPointCoords.ID);

			///////////////////////////////////////////////////
			// Output Reachability of Destination from Start //
			///////////////////////////////////////////////////

			if (travelDistance != INF) {
				cout << "Distance to dest: " << travelDistance << " miles";
			} else {
				cout << "Sorry, destination unreachable";
			}
			cout << endl;

			///////////////////////////////////////////////////////////
			// Save path from Destination to Start Vertex in a Stack //
			///////////////////////////////////////////////////////////

			//
			// Given the predecessors map from Dijkstra's, we can only pull
			// the path from the destination footway vertex to the start vertex.
			// We have to reverse the order to get the path from
			// the start vertex to the destination vertex
			//
			stack<long long> path;  // Store the path from start to destiantion

			if (startFound && destFound) {
				path.push(destPointCoords.ID);
				long long predecessorID = predecessors.at(destPointCoords.ID);
				while (predecessorID != -1) {  // Path ends at -1
					path.push(predecessorID);
					predecessorID = predecessors.at(predecessorID);
				}
			}

			//////////////////////////////////////////////////////////////
			// Output Path Footway Node Route from Start to Destination //
			//////////////////////////////////////////////////////////////

			// Ouputs pathway from start to destination in correct order
			if (travelDistance != INF) {
				cout << "Path: ";

				for (int i = path.size(); i > 0; i--) {
					if (i == 1) {
						cout << path.top() << endl;
						path.pop();
					} else {
						cout << path.top() << "->";
						path.pop();
					}
				}
			}
		}

		////////////////////////////
		// Prompt for next search //
		////////////////////////////

		cout << endl;
		cout << "Enter start (partial name or abbreviation), or #> ";
		getline(cin, startQuery);
	}

	//
	// done:
	//
	cout << "** Done **" << endl;
	return 0;
}
