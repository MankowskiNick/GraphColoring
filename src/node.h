#ifndef NODE_H
#define NODE_H

#include <vector>
#include <algorithm>

// Remove all occurrences of remove_element from the given vector
template<typename T>
void RemoveFromVector(std::vector<T>& list, T remove_element) {
    int list_size = list.size();
    for (int i = 0; i < list_size; i++) {
        if (list[i] == remove_element) {
            list_size--;
            for (int j = i; j < list.size(); j++) {
                list[j] = list[j + 1];
            }
            list.resize(list_size);
        }
    }
}

class Node {
    public:

        Node() {
            Node(-1);
        }

        Node(int initial_id) {
            id = initial_id;
            color = -1;
            SortById = true;
        }

        int color;

        int Id() {
            return id;
        }

        void SetPossibleColors(const std::vector<int>& potential_color_list) {
            // Deep copy the list
            possible_colors = std::vector<int>(potential_color_list); 
        }

        bool RemovePossibleColor(const int remove_color) {
            // RemoveFromVector<int>(possible_colors, remove_color);
            bool found_color = false;
            int list_size = possible_colors.size();
            for (int i = 0; i < list_size; i++) {
                if (possible_colors[i] == remove_color) {
                    found_color = true;
                    list_size--;
                    for (int j = i; j < possible_colors.size(); j++) {
                        possible_colors[j] = possible_colors[j + 1];
                    }
                    possible_colors.resize(list_size);
                }
            }
            return found_color;
        }

        void AddPossibleColor(int new_color) {
            RemovePossibleColor(new_color);
            int index = 0;
            while (new_color >= possible_colors[index] && index < possible_colors.size()) index++;
            possible_colors.resize(possible_colors.size() + 1);
            if (index < possible_colors.size() - 1) {
                for (int i = possible_colors.size() - 1; i > index; i--) {
                    possible_colors[i] = possible_colors[i - 1];
                }
            }
            possible_colors[index] = new_color;
        }

        const bool CheckColorPossible(std::vector<Node>& node_list, const std::vector< std::vector<bool> >& adjacency_matrix, int new_color) {
            for (int i = 0; i < possible_colors.size(); i++) {
                if (possible_colors[i] == new_color) {
                    for (int j = 0; j < node_list.size(); j++) {
                        if (adjacency_matrix[Id()][node_list[j].Id()] && node_list[j].color == new_color) {
                            return false;
                        }
                    }
                    return true;
                }
            }
            return false;
        }

        void ShufflePossibleColors() {
            std::random_shuffle(possible_colors.begin(), possible_colors.end());
        }

        std::vector<int> possible_colors;

        bool SortById;
        bool operator>(Node node2) {
            if (SortById)
                return id > node2.Id();
            else
                return possible_colors.size() > node2.possible_colors.size();
        }
        bool operator<(Node node2) {
            if (SortById)
                return id < node2.Id();
            else
                return possible_colors.size() < node2.possible_colors.size();
        }
        bool operator>=(Node node2) {
            if (SortById)
                return id >= node2.Id();
            else
                return possible_colors.size() >= node2.possible_colors.size();
        }
        bool operator<=(Node node2) {
            if (SortById)
                return id <= node2.Id();
            else
                return possible_colors.size() <= node2.possible_colors.size();
        }

    private:
        int id;

};

#endif