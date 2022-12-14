#include <iostream>
#include <vector>
#include <algorithm>
#include <math.h>
#include <stdlib.h>
#include <ctime>

#include "node.h"
#include "quicksort.h"

#define PER_ANNEAL_TIME_MAX 30
#define TIME_MAX 300
#define START_TEMP_SCALAR 100
#define PERCENT_CHANGES 0.3
#define MAX_NUM_CHANGES 100 
#define TEMP_MIN 2

// Get whether an element exists in a vector
template<typename T>
bool ExistsInVector(std::vector<T>& vector, T element) {
    for (int i = 0; i < vector.size(); i++) {
        if (vector[i] == element) return true;
    }
    return false;
}

// Swap two elements
template<typename T>
void Swap(T& obj1, T& obj2) {
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

// Clear the color of all nodes in node_list
void ClearNodeColors(std::vector<Node>& node_list) {
    for (int i = 0; i < node_list.size(); i++) {
        node_list[i].color = -1;
    }
}

// Remove color from nodes adjacent to cur_node
void RemoveFromAdjacentNodes(std::vector<Node>& node_list, const std::vector< std::vector<bool> >& adjacency_matrix, int cur_node, int remove_color) {
    for (int i = 0; i < node_list.size(); i++) {
        if (node_list[i].Id() != node_list[cur_node].Id() && adjacency_matrix[node_list[i].Id()][node_list[cur_node].Id()]) {
            node_list[i].RemovePossibleColor(remove_color);
        }
    }
} 

// Add color to nodes adjacent to cur_node
void AddToAdjacentNodes(std::vector<Node>& node_list, const std::vector< std::vector<bool> >& adjacency_matrix, int cur_node, int add_color) {
    for (int i = 0; i < node_list.size(); i++) {
        if (node_list[i].Id() != node_list[cur_node].Id() && adjacency_matrix[node_list[i].Id()][node_list[cur_node].Id()]) {
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

// Insert item at given index
template<typename T>
bool InsertAtPosition(std::vector<T>& vect, T& add_item, int index) {
    vect.resize(vect.size() + 1);
    if (index >= vect.size() || index < 0) throw std::out_of_range ("Out of Range Exception: Insert index is out of range of vector.");
    for (int i = vect.size() - 2; i >= index; i--) {
        vect[i + 1] = vect[i];
    }
    vect[index] = add_item;
    return true;
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
bool ColorGraph_Helper(std::vector<Node>& node_list, const std::vector< std::vector<bool> >& adjacency_matrix, int cur_node, const time_t& start_time, bool track_time, bool& immediate_break) {
    if (cur_node == node_list.size()) return true;
    if (node_list[cur_node].possible_colors.size() == 0) return false;
    if (start_time + 1 <= time(NULL) && track_time) {
        immediate_break = true;
        return false;
    }
    int old_color = node_list[cur_node].color;
    for (int j = 0; j < node_list[cur_node].possible_colors.size(); j++) {
        int cur_color = node_list[cur_node].possible_colors[j];
        if (node_list[cur_node].CheckColorPossible(node_list, adjacency_matrix, cur_color)) {
            UpdateNodes(node_list, adjacency_matrix, cur_node, cur_color);
            bool instant_break = false;
            if (ColorGraph_Helper(node_list, adjacency_matrix, cur_node + 1, start_time, track_time, instant_break)) {
                return true;
            } else {
                UpdateNodes(node_list, adjacency_matrix, cur_node, old_color);
                if (instant_break) break;
            }
        }
    }
    return false;
}

void Anneal(std::vector<Node>& node_list, std::vector< std::vector<bool> >& adjacency_matrix, int num_changes, time_t start_time) {

    // Simulated Annealing
    // Initialize temperature
    double initial_temp = (START_TEMP_SCALAR * START_TEMP_SCALAR) / double(node_list.size());
    double temp = double(initial_temp);
    

    // Seed node colors
    std::vector<int> colors_used;
    GetColorsUsed(node_list, colors_used);
    SeedNodeColors(node_list, colors_used);
    

    // Start simulated annealing
    int iteration = 1;
    
    while (temp > TEMP_MIN && time(NULL) - start_time < PER_ANNEAL_TIME_MAX) {

        // Get a working list
        std::vector<Node> annealing_list = std::vector<Node>(node_list);
        GetColorsUsed(node_list, colors_used);
        SeedNodeColors(annealing_list, colors_used);
        ClearNodeColors(annealing_list);

        // Reshuffle all of the possible_colors - maybe we could make a different change
        for (int i = 0; i < num_changes; i++) {
            SwapRandomNodes(annealing_list);
        }

        // Recolor the graph
        time_t cur_time = time(NULL);
        bool instant_break;
        bool valid = ColorGraph_Helper(annealing_list, adjacency_matrix, 0, cur_time, true, instant_break);

        if (valid) {

            int check = CountColors(annealing_list);

            double take_probability = exp(-1 * (check - CountColors(node_list)) / (temp + 1));
            double random = double(rand()) / double(RAND_MAX);
            if (check < CountColors(node_list) || (random < take_probability && check < CountColors(node_list))) {
                node_list = std::vector<Node>(annealing_list);
            }
        }
        temp = initial_temp / double(iteration);
        iteration++;

    }
}

// Solve graph coloring
int ColorGraph(std::vector<Node>& node_list, std::vector< std::vector<bool> >& adjacency_matrix, const time_t& start_time) {

    // Deep copy a vector that we can use to work
    std::vector<Node> work_list = std::vector<Node>(node_list);

    
    // Get a quick heuristic
    bool result = false;
    while (!result) {
        SeedNodeColors(work_list, node_list.size());
        bool instant_break; // this value doesn't really matter
        ColorGraph_Helper(work_list, adjacency_matrix, 0, start_time, false, instant_break);
        result = ValidateNodes(work_list, adjacency_matrix);
        if (result) {
            node_list = std::vector<Node>(work_list);
        }else {
            return -1;
        }
    }
    
    // Backup result - just in case
    std::vector<Node> backup_list = std::vector<Node>(node_list);
    
    // Get this so we can compare data in the debugger
    int pre_annealing_check = CountColors(node_list);

    // Perform annealing
    int max_changes = PERCENT_CHANGES * node_list.size();
    if (max_changes == 0) max_changes = 1;
    for (int i = 1; i <= max_changes && time(NULL) - start_time < TIME_MAX; i++) {
        Anneal(node_list, adjacency_matrix, i, time(NULL));
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