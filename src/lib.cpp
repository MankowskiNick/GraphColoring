#include <iostream>
#include <vector>
#include <algorithm>

#include "node.h"

void InputMapper(const std::vector< std::vector<int> >& input, std::vector< std::vector<bool> >& adjacency_matrix, std::vector<Node>& nodes) {
    
    // Sanitize output vector
    adjacency_matrix = std::vector< std::vector<bool> >();

    // Verify input vector is the correct format
    if (input[0].size() != 2 || input.size() <= 1) {
        std::cout << "FATAL ERROR: Error mapping input into adjacency matrix, perhaps an invalid input file?\n";
        //assert(input[0].size() == 2);
        return;
    }

    // Verified input data, now map to adjacency matrix
    
    // Get # of vertices & edges
    int numVertices = input[0][0];
    int numEdges = input[0][1];

    numVertices = 10;

    // Set out adjacency matrix to the correct size & give default value of false
    adjacency_matrix.resize(numVertices);
    for (int i = 0; i < numVertices; i++) {
        adjacency_matrix[i].resize(numVertices);
        for (int j = 0; j < numVertices; j++) {
            adjacency_matrix[i][j] = false;
        }
    }

    int maxNodeID = -1;

    // Map input data to adjacency matrix
    for (int i = 1; i < input.size(); i++) {
        int node1, node2;
        node1 = input[i][0];
        node2 = input[i][1];

        // Add to nodes if both nodes don't already exist
        if (node1 > maxNodeID && node1 > maxNodeID) {
            maxNodeID = std::max(node1, node2);
            nodes.push_back(Node(node1));
            nodes.push_back(Node(node2));
        }

        // Add the max node value if it already not accounted for
        if (node1 > maxNodeID || node2 > maxNodeID) {
            maxNodeID = std::max(node1, node2);
            nodes.push_back(Node(std::max(node1, node2)));
        }

        adjacency_matrix[node1][node2] = true;
        adjacency_matrix[node2][node1] = true;
    }
}