#include <iostream>
#include <vector>
#include <algorithm>
#include <math.h>
#include <stdlib.h>
#include <ctime>

#include "node.h"
#include "quicksort.h"

//#define INT_MAX 2147483647 // Was getting weird compiler errors saying INT_MAX was undefined so I defined it

#define TIME_MAX 600
#define START_TEMP_SCALAR 100
#define TEMP_MIN 1

// Get whether an element exists in a vector
template<typename T>
bool ExistsInVector(std::vector<T>& vector, T element) {
    for (int i = 0; i < vector.size(); i++) {
        if (vector[i] == element) return true;
    }
    return false;
}

template<typename T>
void Swap(T obj1, T obj2) {
    T temp = obj1;
    obj1 = obj2;
    obj2 = temp;
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

// Overload to use a specific vector rather than n possible colors
void SeedNodeColors(std::vector<Node>& node_list, const std::vector<int>& colors_used) {
    for (int i = 0; i < node_list.size(); i++) {
        node_list[i].SetPossibleColors(colors_used);
    }
}

// Clear possible colors from nodes
void ClearNodes(std::vector<Node>& node_list) {
    for (int i = 0; i < node_list.size(); i++) {
        node_list[i].possible_colors = std::vector<int>();
        node_list[i].color = -1;
    }
}

void ClearNodeColors(std::vector<Node>& node_list) {
    for (int i = 0; i < node_list.size(); i++) {
        node_list[i].color = -1;
    }
}

// Remove color from nodes adjacent to cur_node
void RemoveFromAdjacentNodes(std::vector<Node>& node_list, const std::vector< std::vector<bool> >& adjacency_matrix, int cur_node, int remove_color) {
    for (int i = 0; i < node_list.size(); i++) {
        if (node_list[i].Id() != cur_node && adjacency_matrix[node_list[i].Id()][node_list[cur_node].Id()]) {
            node_list[i].RemovePossibleColor(remove_color);
        }
    }
} 

// Add color to nodes adjacent to cur_node
void AddToAdjacentNodes(std::vector<Node>& node_list, const std::vector< std::vector<bool> >& adjacency_matrix, int cur_node, int add_color) {
    for (int i = 0; i < node_list.size(); i++) {
        if (node_list[i].Id() != cur_node && adjacency_matrix[node_list[i].Id()][node_list[cur_node].Id()]) {
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

// Swap two random elements in the list of nodes, our "random" change in simulated annealing
void SwapRandomNodes(std::vector<Node>& node_list) {
    int swap_node1 = 0;
    int swap_node2 = 0;
    while (swap_node1 == swap_node2) {
        swap_node1 = rand() % node_list.size();
        swap_node2 = rand() % node_list.size();
    }
    Swap<Node>(node_list[swap_node1], node_list[swap_node2]);
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

// Count the colors used to make the graph
int CountColors(const std::vector<Node>& node_list) {
    std::vector<int> colors_used;
    GetColorsUsed(node_list, colors_used);
    return colors_used.size();
}

// Validate node_list
bool ValidateNodes(std::vector<Node>& node_list, const std::vector< std::vector<bool> > & adjacency_matrix) {
    for (int i = 0; i < node_list.size(); i++) {
        for (int j = 0; j < node_list.size(); j++) {
            if (i != j && node_list[i].color == node_list[j].color && adjacency_matrix[node_list[i].Id()][node_list[j].Id()]) {
                return false;
            }
        }
    }
    return true;
}

// Shuffle possible_colors so we can get a different result
void ShufflePossibleColors(std::vector<Node>& node_list) {
    for (int i = 0; i < node_list.size(); i++) {
        node_list[i].ShufflePossibleColors();
    }
}

//Normalize the node colors to to from 0 -> n-1
void NormalizeNodeColors(std::vector<Node>& node_list) {
    std::vector<Node> reference_list(node_list);
    std::vector<int> colors_used;
    GetColorsUsed(node_list, colors_used);
    for (int i = 0; i < colors_used.size(); i++) {
        for (int j = 0; j < node_list.size(); j++) {
            if (reference_list[j].color == colors_used[i]) {
                node_list[j].color = i;
            }
        }
    }
}

// Initial graph coloring
bool ColorGraph_Helper(std::vector<Node>& node_list, const std::vector< std::vector<bool> >& adjacency_matrix, int cur_node, const time_t& start_time) {
    if (cur_node == node_list.size()) return true;
    if (node_list[cur_node].possible_colors.size() == 0) return false;
    if (start_time + 1 <= time(NULL)) return false;
    int old_color = node_list[cur_node].color;
    for (int j = 0; j < node_list[cur_node].possible_colors.size(); j++) {
        int cur_color = node_list[cur_node].possible_colors[j];
        UpdateNodes(node_list, adjacency_matrix, cur_node, cur_color);
        if (ColorGraph_Helper(node_list, adjacency_matrix, cur_node + 1, start_time)) {
            return true;
        } else {
            UpdateNodes(node_list, adjacency_matrix, cur_node, old_color);
        }
    }
    return false;
}

bool QuickColor(std::vector<Node>& node_list, const std::vector< std::vector<bool> >& adjacency_matrix, int cur_node) {
    if (cur_node == node_list.size()) return true;
    else if (node_list[cur_node].possible_colors.size() == 0) return false;
    else {
        UpdateNodes(node_list, adjacency_matrix, cur_node, node_list[cur_node].possible_colors[0]);
        return QuickColor(node_list, adjacency_matrix, cur_node + 1);
    }
}


// Solve graph coloring
int ColorGraph(std::vector<Node>& node_list, std::vector< std::vector<bool> >& adjacency_matrix, const time_t& start_time) {

    // Deep copy a vector that we can use to work
    std::vector<Node> work_list = std::vector<Node>(node_list);

    
    // Get a quick heuristic
    SeedNodeColors(work_list, node_list.size());
    bool result = ColorGraph_Helper(work_list, adjacency_matrix, 0, start_time);
    if (result) {
        node_list = std::vector<Node>(work_list);
    }else {
        return -1;
    }

    
    // Simulated Annealing

    // Initialize temperature
    double initial_temp = (START_TEMP_SCALAR * START_TEMP_SCALAR) / double(node_list.size());
    double temp = double(initial_temp);
    
    // Get this so we can compare data in the debugger
    int pre_annealing_check = CountColors(node_list);

    // Seed node colors
    std::vector<int> colors_used;
    GetColorsUsed(node_list, colors_used);
    SeedNodeColors(node_list, colors_used);
    
    std::vector<Node> backup_list = std::vector<Node>(node_list);

    // Start simulated annealing
    int iteration = 1;
    while (temp > TEMP_MIN) {

        // Get a working list
        std::vector<Node> annealing_list = std::vector<Node>(node_list);
        GetColorsUsed(node_list, colors_used);
        SeedNodeColors(annealing_list, colors_used);
        ClearNodeColors(annealing_list);

        // Reshuffle all of the possible_colors
        //ShufflePossibleColors(annealing_list);
        std::random_shuffle(annealing_list.begin(), annealing_list.end());
        //SwapRandomNodes(annealing_list);

        // Recolor the graph
        //QuickColor(annealing_list, adjacency_matrix, 0);
        time_t cur_time = time(NULL);
        ColorGraph_Helper(annealing_list, adjacency_matrix, 0, cur_time);
        bool valid = ValidateNodes(annealing_list, adjacency_matrix);

        if (valid) {

            int check = CountColors(annealing_list);

            temp = initial_temp / double(iteration);
            //temp *= cooling_rate;
            double take_probability = exp(-1 * (check - CountColors(node_list)) / (temp + 1));

            // We need to calulate the probability to take it.
            double random = double(rand()) / double(RAND_MAX);
            if (check < CountColors(node_list) || random < take_probability) {
                node_list = std::vector<Node>(annealing_list);
            }
        }
        iteration++;

    }
    
    // Make sure we sort by id
    for (int i = 0; i < node_list.size(); i++) {
        node_list[i].SortById = true;
    }
    
    int post_annealing_check = CountColors(node_list);
    if (post_annealing_check <= pre_annealing_check) {
        QuickSort<Node>::Sort(node_list);
        std::vector<int> final_colors_used;
        GetColorsUsed(node_list, final_colors_used);
        NormalizeNodeColors(node_list);
        GetColorsUsed(node_list, final_colors_used);
        return post_annealing_check;
    } else {
        QuickSort<Node>::Sort(backup_list);
        node_list = std::vector<Node>(backup_list);
        std::vector<int> final_colors_used;
        GetColorsUsed(node_list, final_colors_used);
        NormalizeNodeColors(node_list);
        return pre_annealing_check;
    }
}