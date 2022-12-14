#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cassert>

using namespace std;

// /Users/nick/Documents/School/Semester 3/CS490/GraphColoring/data/gc_20_1 /Users/nick/Documents/School/Semester 3/CS490/GraphColoring/data/OUTPUT_gc_20_1

void Verify(string input_filename, string output_filename) {
  ifstream input;
  input.open(input_filename.data());
  assert(input.is_open());
  ifstream output;
  output.open(output_filename.data());
  assert(output.is_open());

  int num_vertices;
  int num_edges;
  input >> num_vertices;
  input >> num_edges;
  int num_colors;
  output >> num_colors;
  cout << "claimed colors: " << num_colors << endl;
  vector<vector<bool> > Edges(num_vertices);
  for(int i = 0; i < Edges.size(); i++)
    Edges[i].resize(num_vertices, false);
  
  for(int i = 0; i < num_edges; i++){
    int u,v;
    input>> u >> v;
    Edges[u][v] = true;
    Edges[v][u] = true;
  }
  vector<int> colors(num_vertices);
  
  for(int i = 0; i < num_vertices; i++){
    int color;
    output >> color;
    if(color >= num_colors || color < 0){
      cout << "ERROR: Color of vertex " << i << " is illegal: " << color;
    }
    colors[i] = color;
  }
  for(int i = 0; i < num_vertices; i++)
    for(int j = 0; j < num_vertices; j++){
      if(Edges[i][j] && colors[i] == colors[j])
	      cout << "ERROR: The edge from " << i << " to " << j << " both have color << " << colors[i] << endl;
    }
  cout << "Done checking" << endl;
}

int main(int argc, char* args[]) {
    for (int i = 1; i < argc; i+=2) {    
      std::cout << "Checking file - " << string(args[i]) << "\n";
      Verify(string(args[i]), string(args[i + 1]));
    }
}