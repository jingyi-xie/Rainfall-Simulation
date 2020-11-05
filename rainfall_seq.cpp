#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <point.h>
#include <rainfallsim.h>
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
             row.push_back(stoi(entry));
         }
         matrix.push_back(row);
    }
    input.close();
    return matrix;
}

int main(int argc, char *argv[]) {
    /* 
    Usage
    P = # of threads
    M = # of simulation time steps with rainfall
    A = absorption rate
    N = dimension of lanescape
    elevation_file = file name
    ./rainfall <P> <M> <A> <N> <elevation_file>
    */
    if (argc != 6) {
        cout << "./rainfall <P> <M> <A> <N> <elevation_file>\n";
        exit(EXIT_FAILURE);
    }

    // Rean input
    const int P = atoi(argv[1]);
    const int M = atoi(argv[2]);
    const float A = atof(argv[3]);
    const int N = atoi(argv[4]);
    vector<vector<int> > input = readFile(argv[5]);

    // Start simulation
    RainfallSim simulator(P, M, A, N, input);
    simulator.startSim();
    simulator.generateOutput();
    
    return 0;
}