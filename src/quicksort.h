// Nick Mankowski
// 10/19/22
// Quicksort Library

#ifndef QUICKSORT_H
#define QUICKSORT_H

#include <vector>


// "Static class" rather than separate functions so that we can keep all of our "typename T"'s in order
// without the syntax getting less readable
template<typename T>
class QuickSort {
    public:

        // Sort given list
        static void Sort(std::vector<T>& list) {
            Partition(0, list.size(), list);
        }
        
        // Sort given list in descending order
        static void SortDescending(std::vector<T>& list) {
            Sort(list);

            std::vector<T> return_list;
            for (int i = list.size() - 1; i >= 0; i--) {
                return_list.push_back(list[i]);
            }
            list = std::vector<T>(return_list);
        }
        
    private: 

        // Pre:  Two objects of type T to swap the value of
        // Post: Void, simply swaps the two parameter values.
        static void Swap(T& swap1, T& swap2) {
            T temp = swap1;
            swap1 = swap2;
            swap2 = temp;
        }

        // Recursive quicksort helper
        // Pre:  The list to partition, the 'low' index value of the list, the 'high'  
        //       index value of the list, these variables defint the segment of the list we are focusing on.
        // Post: Returns void, eventually will sort the list in the parameter 'list'
        static void Partition(int low, int high, std::vector<T>& list) {

            // If we are looking at a valid range
            if (low < high) {

                // Pick a pivot element
                T pivot = list[low];

                // The last element <= pivot
                int last_small = low;

                // Look through all the unseen elements
                for (int i = low + 1; i < high; i++) {

                    // If the item is less than the pivot, increment last_small and
                    // swap the element at that position for the one we are looking at
                    if (list[i] <= pivot) {
                        last_small++;
                        Swap(list[i], list[last_small]);
                    }

                    // If the item is greater than the pivot, we don't it can stay to the
                    // right, we don't need to do anything
                }

                // Put the pivot element in it's proper place
                Swap(list[last_small], list[low]);

                // Recursively partition the half < the pivot element
                Partition(low, last_small, list);

                // Recursively partition the half > the pivot element
                Partition(last_small + 1, high, list);
            }
        }
};

template<typename T, typename G>
class QuickSort2 {
    public:

        // Sort 2 lists by ascending order
        static void SortDescending(std::vector<T>& list1, std::vector<G>& list2) {
            Sort(list1, list2);

            std::vector<T> return_list1;
            std::vector<G> return_list2;
            for (int i = list1.size() - 1; i >= 0; i--) {
                return_list1.push_back(list1[i]);
                return_list2.push_back(list2[i]);
            }
            list1 = std::vector<T>(return_list1);
            list2 = std::vector<G>(return_list2);

        }

        // Public sort function
        // Pre:  2 Unsorted list
        // Post: Void, but sorts the lists based on list1 T listed
        //       in the parameter.
        static void Sort(std::vector<T>& list1, std::vector<G>& list2) {
            Partition(0, list1.size(), list1, list2);
        }
        
    private: 

        // Pre:  Two objects of type T to swap the value of
        // Post: Void, simply swaps the two parameter values.
        static void Swap(T& swap1, T& swap2) {
            T temp = swap1;
            swap1 = swap2;
            swap2 = temp;
        }

        // Pre:  Two objects of type G to swap the value of
        // Post: Void, simply swaps the two parameter values.
        static void Swap(G& swap1, G& swap2) {
            G temp = swap1;
            swap1 = swap2;
            swap2 = temp;
        }

        // Recursive quicksort helper
        // Pre:  The list to partition, the 'low' index value of the list, the 'high'  
        //       index value of the list, these variables defint the segment of the list we are focusing on.
        // Post: Returns void, eventually will sort the list in the parameter 'list'
        static void Partition(int low, int high, std::vector<T>& list, std::vector<G>& list2) {

            // If we are looking at a valid range
            if (low < high) {

                // Pick a pivot element
                T pivot = list[low];

                // The last element <= pivot
                int last_small = low;

                // Look through all the unseen elements
                for (int i = low + 1; i < high; i++) {

                    // If the item is less than the pivot, increment last_small and
                    // swap the element at that position for the one we are looking at
                    if (list[i] <= pivot) {
                        last_small++;
                        Swap(list[i], list[last_small]);
                        Swap(list2[i], list2[last_small]);
                    }

                    // If the item is greater than the pivot, we don't it can stay to the
                    // right, we don't need to do anything
                }

                // Put the pivot element in it's proper place
                Swap(list[last_small], list[low]);
                Swap(list2[last_small], list2[low]);

                // Recursively partition the half < the pivot element
                Partition(low, last_small, list, list2);

                // Recursively partition the half > the pivot element
                Partition(last_small + 1, high, list, list2);
            }
        }
};

#endif