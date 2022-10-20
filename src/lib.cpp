#include <iostream>
#include <vector>
#include <algorithm>

#include "node.h"
#include "quicksort.h"

// Get whether an element exists in a vector
template<typename T>
bool ExistsInVector(std::vector<T>& vector, T element) {
    for (int i = 0; i < vector.size(); i++) {
        if (vector[i] == element) return true;
    }
    return false;
}

// Map input vector to a node list & adjacency matrix
void InputMapper(const std::vector< std::vector<int> >& input, std::vector< std::vector<bool> >& adjacency_matrix, std::vector<Node>& nodes) {
    
    // Sanitize output vector
    adjacency_matrix = std::vector< std::vector<bool> >();

    // Verify input vector is the correct format
    if (input[0].size() != 2 || input.size() <= 1) {
        std::cout << "FATAL ERROR: Error mapping input into adjacency matrix, perhaps an invalid input file?\n";
        //assert(input[0].size() == 2);
        return;
    }

    // Verified input data, now map to adjacency matrix & create node list
    
    // Get # of vertices & edges
    int numVertices = input[0][0];
    int numEdges = input[0][1];

    // Set out adjacency matrix to the correct size & give default value of false
    adjacency_matrix.resize(numVertices);
    for (int i = 0; i < numVertices; i++) {
        adjacency_matrix[i].resize(numVertices);
        for (int j = 0; j < numVertices; j++) {
            adjacency_matrix[i][j] = false;
        }
    }

    int maxNodeID = -1;

    // Map input data to adjacency matrix & create node list
    for (int i = 1; i < input.size(); i++) {
        int node1, node2;
        node1 = input[i][0];
        node2 = input[i][1];

        // Add to nodes if both nodes don't already exist
        bool found_node1 = false;
        bool found_node2 = false;
        for (int j = 0; j < nodes.size(); j++) {
            if (nodes[j].Id() == node1) found_node1 = true;
            if (nodes[j].Id() == node2) found_node2 = true;
        }
        if (!found_node1) nodes.push_back(node1);
        if (!found_node2) nodes.push_back(node2);

        // Update the adjacency matrix
        adjacency_matrix[node1][node2] = true;
        adjacency_matrix[node2][node1] = true;
    }

    QuickSort<Node>::Sort(nodes);
}

// Seed node list with a list of n possible colors
void SeedNodeColors(std::vector<Node>& node_list, int n) {
    std::vector<int> seed_list = std::vector<int>();
    for (int i = 0; i < n; i++) {
        seed_list.push_back(i);
    }

    for (int i = 0; i < node_list.size(); i++) {
        node_list[i].SetPossibleColors(seed_list);
    }
}

// Clear possible colors from nodes
void ClearNodes(std::vector<Node>& node_list) {
    for (int i = 0; i < node_list.size(); i++) {
        node_list[i].possible_colors = std::vector<int>();
        node_list[i].color = -1;
    }
}

// Remove color from nodes adjacent to cur_node
void RemoveFromAdjacentNodes(std::vector<Node>& node_list, const std::vector< std::vector<bool> >& adjacency_matrix, int cur_node, int remove_color) {
    for (int i = 0; i < node_list.size(); i++) {
        if (i != cur_node && adjacency_matrix[i][cur_node]) {
            node_list[i].RemovePossibleColor(remove_color);
        }
    }
} 

// Add color to nodes adjacent to cur_node
void AddToAdjacentNodes(std::vector<Node>& node_list, const std::vector< std::vector<bool> >& adjacency_matrix, int cur_node, int add_color) {
    for (int i = 0; i < node_list.size(); i++) {
        if (i != cur_node && adjacency_matrix[i][cur_node]) {
            node_list[i].AddPossibleColor(add_color);
        }
    }
}

// Update cur_node to reflect the correct color as well as update all adjacent nodes to not have that possible color
void UpdateNodes(std::vector<Node>& node_list, const std::vector< std::vector<bool> >& adjacency_matrix, int cur_node, int replacement_color) {
    if (node_list[cur_node].color > -1) {
        AddToAdjacentNodes(node_list, adjacency_matrix, cur_node, node_list[cur_node].color);
    }
    RemoveFromAdjacentNodes(node_list, adjacency_matrix, cur_node, replacement_color);
    node_list[cur_node].color = replacement_color;
}

// Count the colors used to make the graph
int CountColors(const std::vector<Node>& node_list) {
    std::vector<int> colors_used;
    for (int i = 0; i < node_list.size(); i++) {
        bool color_found = false;
        for (int j = 0; j < colors_used.size(); j++) {
            if (node_list[i].color == colors_used[j]) color_found = true;
        }
        if (!color_found) colors_used.push_back(node_list[i].color);
    }
    return colors_used.size();
}

// Get the colors currently present in node_list
void GetColorsUsed(const std::vector<Node>& node_list, std::vector<int>& output_vector) {
    output_vector = std::vector<int>();
    for (int i = 0; i < node_list.size(); i++) {
        if (!ExistsInVector<int>(output_vector, node_list[i].color)) {
            output_vector.push_back(node_list[i].color);
        }
    }
}

// Insert mistakes into node_list
void TamperWithNodes(std::vector<Node>& node_list, const std::vector< std::vector<bool> >& adjacency_matrix, std::vector<int>& node_edge_counts, int temp) {
    // Order lists & counts by node_vertice_counts value
    QuickSort2<int, Node>::SortDescending(node_edge_counts, node_list);

    // MAYBE: Shuffle possible colors 

    // Make changes to the top 'temp' values
    for (int i = 0; i < temp; i++) {
        if (node_list[i].possible_colors.size() > 1) {
            for (int j = 0; j < node_list[i].possible_colors.size(); j++) {
                if (node_list[i].CheckColorPossible(node_list[i].possible_colors[j])) {
                    node_list[i].RemovePossibleColor(node_list[i].color);
                    UpdateNodes(node_list, adjacency_matrix, i, node_list[i].possible_colors[j]);  // maybe change what color we use to be random. could help!
                    return;
                }
            }
        }
    }

    // Order the list back to the original order
    QuickSort<Node>::Sort(node_list);
}

// Validate node_list
bool ValidateNodes(std::vector<Node>& node_list, const std::vector< std::vector<bool> > & adjacency_matrix) {
    for (int i = 0; i < node_list.size(); i++) {
        for (int j = 0; j < node_list.size(); j++) {
            if (i != j && node_list[i].color == node_list[j].color && adjacency_matrix[i][j]) {
                return false;
            }
        }
    }
    return true;
}
// Color graph 'quickly'
bool ColorGraph_Helper(std::vector<Node>& node_list, const std::vector< std::vector<bool> >& adjacency_matrix, int cur_node) {
    if (cur_node == node_list.size()) return true;
    for (int j = 0; j < node_list[cur_node].possible_colors.size(); j++) {
        int cur_color = node_list[cur_node].possible_colors[j];
        if (node_list[cur_node].CheckColorPossible(cur_color)) {
            UpdateNodes(node_list, adjacency_matrix, cur_node, cur_color);
            return ColorGraph_Helper(node_list, adjacency_matrix, cur_node + 1);
        }
    }
    return false;
}

// Solve graph coloring
int ColorGraph(std::vector<Node>& node_list, std::vector< std::vector<bool> >& adjacency_matrix) {

    // Deep copy a vector that we can use to work
    std::vector<Node> work_list = std::vector<Node>(node_list);

    // Get a quick heuristic
    int i = node_list.size(); 
    bool result = true;
    while (result) {
        SeedNodeColors(work_list, i);
        result = ColorGraph_Helper(work_list, adjacency_matrix, 0);
        if (result) node_list = std::vector<Node>(work_list);
        ClearNodes(work_list);
        i--;
    }


    // Make a backup before we fuck up our list
    std::vector<Node> node_list_backup(node_list);

    // Simulated Annealing
    int temp = node_list.size();

    std::vector<int> node_edge_counts;
    for (int i = 0; i < adjacency_matrix.size(); i++) {
        int num_vertices = 0;
        for (int j = 0; j < adjacency_matrix.size(); j++) {
            if (adjacency_matrix[i][j]) num_vertices++;
        }
        node_edge_counts.push_back(num_vertices);
    }

    while (temp > 0) {
        // Make changes
        TamperWithNodes(node_list, adjacency_matrix, node_edge_counts, temp);

        //ColorGraph_Helper(node_list, adjacency_matrix, 0);

        if (CountColors(node_list) < CountColors(node_list_backup) && ColorGraph_Helper(node_list, adjacency_matrix, 0)) node_list_backup = std::vector<Node>(node_list);
        //else node_list = std::vector<Node>(node_list_backup);
        // Fix them
        temp--;
    }

    QuickSort<Node>::Sort(node_list_backup);
    return CountColors(node_list_backup);
}