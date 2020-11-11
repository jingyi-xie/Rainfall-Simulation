#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

// Read input file
vector<vector<int> > readFile(string fileName) {
    // Initialize vector
    vector<vector<int> > matrix;
    
    // read in file
    ifstream input(fileName);
    if (!input.is_open()) {
        cout << "Open file error\n"; 
        exit(EXIT_FAILURE);
    }

    // Iterate through lines
    string line;
    string entry;
    while (getline(input, line)) {
         stringstream rowStream(line);
         vector<int> row;
         while (getline(rowStream, entry, ' ')) {
             if (entry != "") {
                row.push_back(stoi(entry));
             }
         }
         matrix.push_back(row);
    }
    input.close();
    return matrix;
}
