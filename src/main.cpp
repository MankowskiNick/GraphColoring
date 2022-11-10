// Nick Mankowski
// CS 490 - Fall 2022
// Prof. Sean McCulloch
// Graph Coloring

// Instructions to run
//  1) Build the program
//  2) Run the executable generated with the filenames you 
//     would like to generate outputs for.  For example, 
//     "GraphColoring.exe gc_4_1 gc_20_1 gc_100_7"
//     would generate output files for the the three listed input files.
//  3) The output files will match the names of the input files in the same directory, 
//     but will have "OUTPUT_" before the input file name.  For example, 
//     "GraphColoring.exe  gc_4_1 gc_20_1 gc_100_7"
//     will generate OUTPUT_gc_4_1, OUTPUT_kgc_20_1, OUTPUT_ks_100_7

#include <iostream>
#include <fstream>
#include <ctime>

#include "iolib.h"
#include "lib.h"
#include "node.h"
#include "quicksort.h"

// Take arguments from command line
int main(int argc, char* args[]) {

    // Look at each file listed in the command line arguments(did this so I don't have to keep running again and typing different file names)
    // Argument 1 is the "./GraphColoring" command, so we have to start looking at args[1] to get file names
    for (int fileIndex = 1; fileIndex < argc; fileIndex++) {    

        // Declare a file input stream & file output stream
        std::ifstream fin;
        std::ofstream fout;

        // Assign a name to the output file, it will match the input file  
        // but with "OUTPUT_" at the beginning as to not overwrite it.
        // We need to find the file name WITHOUT the directory
        std::string inputFile, outputFile;

        // Supply with the file name from console, and the inputFile & outputFile to set
        SetIOFiles(args[fileIndex], inputFile, outputFile); 

        // Open the input file
        fin.open(inputFile.data());

        // Assert that the input file successfully opened
        AssertInputFileOpenSuccess(fin.fail(), inputFile);

        // Read in data from files, put in 2d array of ints
        std::vector< std::vector<int> > input_vector;
        bool newVect = false;
        int current_value = 0;
        std::vector<int> current_vect;
        while (fin >> current_value) {
            if (!newVect) {
                current_vect.push_back(current_value);
            } else {
                current_vect.push_back(current_value);
                input_vector.push_back(current_vect);
                current_vect.resize(0);
            }

            newVect = !newVect;
        }

        // Map inputs
        std::vector< std::vector<bool> > adjacency_matrix;
        std::vector<Node> node_list;
        
        InputMapper(input_vector, adjacency_matrix, node_list);

        // Order node_list
        for (int i = 0; i < node_list.size(); i++) {
            node_list[i].SortById = false;
        }
        QuickSort<Node>::Sort(node_list);
        time_t start_time = time(NULL);
        int result = ColorGraph(node_list, adjacency_matrix, start_time);

        std::cout << "File:" << inputFile << "\n    -result=" << result << "\n    -time spent=" << time(NULL) - start_time << "\n\n" ;

        // Output files
        fout.open(outputFile.data());

        fout << result << "\n";
        for (int i = 0; i < node_list.size(); i++) {
            fout << node_list[i].color << " ";
        }
        fout << "\n";

        // Close the input & output files
        fin.close();
        fout.close();
    }

    return 0;
}