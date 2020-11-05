#include <vector>
#include <point.h>
#include <limits.h>

using namespace std;

class RainfallSim {
    private:
        int P, M, N;
        float A;
        int timeSteps;
        // int totalRainfall;
        // int totalAbsorbed;
        vector<vector<Point> > landscape;

    public:
        RainfallSim(int P, int M, float A, int N, vector<vector<int> > input);
        ~RainfallSim();

        // Call at start and end of simulation
        void startSim();
        void generateOutput();

        // Call during simulation
        void newRain();
        void trickle();

        // Chekers
        bool validPosition(int x, int y);

};


// ========== Constructor & Destructor ========== //
RainfallSim::RainfallSim(int P, int M, float A, int N, vector<vector<int> > input) {
    this->P = P;
    this->M = M;
    this->A = A;
    this->N = N;
    this->timeSteps = 0;
    // populate landscape
    const int dx[] = {0, 1, 0, -1};
    const int dy[] = {-1, 0, 1, 0};
    for (int i = 0; i < N; i++) {
        vector<Point> tmp;
        for (int j = 0; j < N; j++) {
            Point p(input[i][j]);
            vectpr<pair<int, int>> lowest;
            int lowest_ele = INT_MAX;
            for (int _ = 0; _ < 4; _++) {
                int x = i + dx[_];
                int y = j + dy[_];
                if (!validPosition(x, y) || input[x][y] > lowest_ele) {
                    continue;
                }
                if (input[x][y] < lowest_ele) {
                    lowest_ele = input[x][y];
                    lowest.clear();
                }
                lowest.push_back(pair<int, int>(x, y));
            }
            for (int ind = 0; ind < lowest.size(); ind++) {
                p.addNeighbor(lowest[ind].first, lowest[ind].second);
            }
            tmp.push_back(p);
        }
        this->landscape.push_back(tmp);
    }
}


// ========== Call at start and end of simulation ========== //
void RainfallSim::startSim() {

}

void RainfallSim::generateOutput() {

}


// ========== Call during simulation ========== //
void RainfallSim::newRain() {

}


void RainfallSim::trickle() {

}


// ========== Checkers ========== //
bool RainfallSim::validPosition(int x, int y) {
    return x >= 0 && x < N && y >= 0 && y < N;
}
