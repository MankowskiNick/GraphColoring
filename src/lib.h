
#include <iostream>
#include <vector>
#include <time.h>
#include <ctime>

#include "node.h"

void InputMapper(const std::vector< std::vector<int> >& input, std::vector< std::vector<bool> >& adjacency_matrix, std::vector<Node>& nodes);

//void SeedNodeColors(std::vector<Node>& node_list, int n);

int ColorGraph(std::vector<Node>& node_list, std::vector< std::vector<bool> >& adjacency_matrix, const time_t& start_time);