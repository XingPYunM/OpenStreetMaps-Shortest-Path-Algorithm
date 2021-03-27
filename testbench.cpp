#include <gtest/gtest.h>
#include "graph.h"

TEST(graph, constructor) {
	graph<int, int> G;
    EXPECT_EQ(G.NumVertices(), 0);
}

TEST(graph, addVertex) {
	graph<int, int> G;
    EXPECT_EQ(G.NumVertices(), 0);
    
    // Test nVertices increment
    G.addVertex(100);
    EXPECT_EQ(G.NumVertices(), 1);
    
    // Test nVertices duplicate
    G.addVertex(100);
    EXPECT_EQ(G.NumVertices(), 1);
    
    // Test nVertices increment (2)
    G.addVertex(200);
    EXPECT_EQ(G.NumVertices(), 2);
    
    // G.dump(cout);
}

TEST(graph, addEdge) {
	graph<int, int> G;
    EXPECT_EQ(G.NumVertices(), 0);
    EXPECT_EQ(G.NumEdges(), 0);
    
    ////////////////////////////////////////////////////////////////////////////
    // Previous Tests
    ////////////////////////////////////////////////////////////////////////////
    
    G.addVertex(100);
    EXPECT_EQ(G.NumVertices(), 1);
    G.addVertex(100);
    EXPECT_EQ(G.NumVertices(), 1);
    G.addVertex(200);
    EXPECT_EQ(G.NumVertices(), 2);
    G.addVertex(300);
    EXPECT_EQ(G.NumVertices(), 3);
    
    ////////////////////////////////////////////////////////////////////////////
    // Current Tests
    ////////////////////////////////////////////////////////////////////////////
    
    // Invalid "from" vertex
    G.addEdge(50, 100, 10);
    EXPECT_EQ(G.NumEdges(), 0);
    
    // Invalid "to" vertex
    G.addEdge(100, 50, 10);
    EXPECT_EQ(G.NumEdges(), 0);
    
    // Valid "from" and "to" vertex
    G.addEdge(100, 200, 10);
    EXPECT_EQ(G.NumEdges(), 1);
    
    // Duplicate edge, overwrite data
    G.addEdge(100, 200, 100);
    EXPECT_EQ(G.NumEdges(), 1);
    
    // Valid "from" and "to" vertex (2)
    G.addEdge(200, 300, 10);
    EXPECT_EQ(G.NumEdges(), 2);
    
    // Self Neighbor
    G.addEdge(100, 100, -999);
    EXPECT_EQ(G.NumEdges(), 3);

    // G.dump(cout);
}

TEST(graph, getWeight) {
	graph<int, int> G;
    EXPECT_EQ(G.NumVertices(), 0);
    EXPECT_EQ(G.NumEdges(), 0);
    
    ////////////////////////////////////////////////////////////////////////////
    // Previous Tests
    ////////////////////////////////////////////////////////////////////////////
    
    /////////////////////
    // addVertex Tests //
    /////////////////////

    G.addVertex(100);
    EXPECT_EQ(G.NumVertices(), 1);
    G.addVertex(100);
    EXPECT_EQ(G.NumVertices(), 1);
    G.addVertex(200);
    EXPECT_EQ(G.NumVertices(), 2);
    G.addVertex(300);
    EXPECT_EQ(G.NumVertices(), 3);
    
    ///////////////////
    // addEdge Tests //
    ///////////////////

    G.addEdge(50, 100, 10);
    EXPECT_EQ(G.NumEdges(), 0);
    G.addEdge(100, 50, 10);
    EXPECT_EQ(G.NumEdges(), 0);
    G.addEdge(100, 200, 10);
    EXPECT_EQ(G.NumEdges(), 1);
    G.addEdge(100, 200, 100);
    EXPECT_EQ(G.NumEdges(), 1);
    G.addEdge(200, 300, 10);
    EXPECT_EQ(G.NumEdges(), 2);
    G.addEdge(100, 100, -999);
    EXPECT_EQ(G.NumEdges(), 3);
    
    ////////////////////////////////////////////////////////////////////////////
    // Current Tests
    ////////////////////////////////////////////////////////////////////////////
    
    int edgeWeight = 0;
    
    // Get existing weight
    G.getWeight(100, 100, edgeWeight);
    EXPECT_EQ(edgeWeight, -999);
    
    // Get non-existant weight
    EXPECT_EQ(G.getWeight(100, 500, edgeWeight), false);
    EXPECT_EQ(edgeWeight, -999);  // edgeWeight should not be updated
    
    // G.dump(cout);
	
}

TEST(graph, neighbors) {
	graph<int, int> G;
    EXPECT_EQ(G.NumVertices(), 0);
    EXPECT_EQ(G.NumEdges(), 0);
    
    ////////////////////////////////////////////////////////////////////////////
    // Previous Tests
    ////////////////////////////////////////////////////////////////////////////
    
    /////////////////////
    // addVertex Tests //
    /////////////////////

    G.addVertex(100);
    EXPECT_EQ(G.NumVertices(), 1);
    G.addVertex(100);
    EXPECT_EQ(G.NumVertices(), 1);
    G.addVertex(200);
    EXPECT_EQ(G.NumVertices(), 2);
    G.addVertex(300);
    EXPECT_EQ(G.NumVertices(), 3);
    
    ///////////////////
    // addEdge Tests //
    ///////////////////

    G.addEdge(50, 100, 10);
    EXPECT_EQ(G.NumEdges(), 0);
    G.addEdge(100, 50, 10);
    EXPECT_EQ(G.NumEdges(), 0);
    G.addEdge(100, 200, 10);
    EXPECT_EQ(G.NumEdges(), 1);
    G.addEdge(100, 200, 100);
    EXPECT_EQ(G.NumEdges(), 1);
    G.addEdge(200, 300, 10);
    EXPECT_EQ(G.NumEdges(), 2);
    G.addEdge(100, 100, -999);
    EXPECT_EQ(G.NumEdges(), 3);
    
    /////////////////////
    // getWeight Tests //
    /////////////////////
    
    int edgeWeight = 0;
    G.getWeight(100, 100, edgeWeight);
    EXPECT_EQ(edgeWeight, -999);
    EXPECT_EQ(G.getWeight(100, 500, edgeWeight), false);
    EXPECT_EQ(edgeWeight, -999);
    
    ////////////////////////////////////////////////////////////////////////////
    // Current Tests
    ////////////////////////////////////////////////////////////////////////////
    
    // for (auto currNeighbor : G.neighbors(100)) {
    // 	cout << currNeighbor << endl;
    // }
    
    EXPECT_EQ(G.neighbors(100).size(), 2);
    EXPECT_EQ(G.neighbors(200).size(), 1);
    EXPECT_EQ(G.neighbors(300).size(), 0);
    EXPECT_EQ(G.neighbors(-999).size(), 0);
    
    // for (auto currVertex : G.getVertices()) {
    // 	cout << currVertex << endl;
    // }

	// G.dump(cout);
}

TEST(graph, copyConstuctor) {
	graph<int, int> G;
    EXPECT_EQ(G.NumVertices(), 0);
    EXPECT_EQ(G.NumEdges(), 0);
    
    ////////////////////////////////////////////////////////////////////////////
    // Previous Tests
    ////////////////////////////////////////////////////////////////////////////
    
    /////////////////////
    // addVertex Tests //
    /////////////////////
    
    for (int i = 0; i < 100; i++) {
    	G.addVertex(i);
    }    
    
    ///////////////////
    // addEdge Tests //
    ///////////////////

    for (int i = 0; i < 100; i++) {
    	G.addEdge(i, 1, i);
    }    
    
    /////////////////////
    // getWeight Tests //
    /////////////////////
    
    //
    // 
    //
    int i = 0;
    int edgeWeight = 0;
    while (G.getWeight(i, i, edgeWeight)) {
    	cout << i << endl;
    	EXPECT_EQ(edgeWeight, i);
    	i++;
    }
    
    // G.getWeight(100, 100, edgeWeight);
    // EXPECT_EQ(edgeWeight, -999);
    // EXPECT_EQ(G.getWeight(100, 500, edgeWeight), false);
    // EXPECT_EQ(edgeWeight, -999);
    
    /////////////////////
    // neighbors Tests //
    /////////////////////
    
    // EXPECT_EQ(G.neighbors(100).size(), 2);
    // EXPECT_EQ(G.neighbors(200).size(), 1);
    // EXPECT_EQ(G.neighbors(300).size(), 0);
    // EXPECT_EQ(G.neighbors(-999).size(), 0);
    
    // for (auto currVertex : G.getVertices()) {
    // 	cout << currVertex << endl;
    // }
    
    ////////////////////////////////////////////////////////////////////////////
    // Current Tests
    ////////////////////////////////////////////////////////////////////////////
    
    graph<int, int> G2 = G;
    graph<int, int> G3;
    G3 = G;

	// G.dump(cout);
	// G2.dump(cout);
	// G3.dump(cout);
	
	// if (G == G2 && G2 == G3) {
	// 	cout << "True" << endl;
	// }
}
