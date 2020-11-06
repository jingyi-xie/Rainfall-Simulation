#include <vector>
#include <time.h>
#include "point.h"

using namespace std;

class RainfallSim {
    private:
        int P, M, N;
        float A;
        int timeSteps;
        vector<vector<Point> > landscape;
        double totalTime;

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
            vector<pair<int, int>> lowest;
            int lowest_ele = input[i][j];
            for (int _ = 0; _ < 4; _++) {
                int x = i + dx[_];
                int y = j + dy[_];
                if (!validPosition(x, y) || input[x][y] > lowest_ele || input[x][y] == input[i][j]) {
                    continue;
                }
                if (input[x][y] < lowest_ele) {
                    lowest_ele = input[x][y];
                    lowest.clear();
                }
                lowest.push_back(pair<int, int>(x, y));
            }
            for (size_t ind = 0; ind < lowest.size(); ind++) {
                p.addNeighbor(lowest[ind].first, lowest[ind].second);
            }
            tmp.push_back(p);
        }
        this->landscape.push_back(tmp);
    }

    cout << "print elevation" << endl;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            cout << this->landscape[i][j].getElevation() << " ";
        }
        cout << endl;
    }

    cout << "M " << M << endl;
    cout << "A " << A << endl;
    cout << "N " << N << endl;

    cout << "=====" << endl;
}

RainfallSim::~RainfallSim() {}


// ========== Call at start and end of simulation ========== //
void RainfallSim::startSim() {
    bool finished = true;
    struct timespec start_time, end_time;
    clock_gettime(CLOCK_MONOTONIC, &start_time);
    while(true) {
        this->timeSteps++;
        // Traverse over all landscape points
        for (int i = 0; i < this->N; i++) {
            for (int j = 0; j < this->N; j++) {
                Point p = this->landscape[i][j];
                // 1) Receive a new raindrop (if it is still raining) for each point.
                if (timeSteps <= this->M) {
                    p.receiveFromSky();
                }
                // 2) If there are raindrops on a point, absorb water into the point
                float remaining = p.getRemainingDrops();
                if (remaining > 0) {
                    p.absorb(this->A < remaining ? this->A : remaining);
                }
                // 3a) Calculate the number of raindrops that will next trickle to the lowest neighbor(s)
                remaining = p.getRemainingDrops();
                float trickleAmount = remaining <= 0 ? 0 : (remaining < 1 ? remaining : 1);
                p.setTrickleAmount(trickleAmount);
                cout << "1st traversal: trickle amount " << p.getTrickleAmount() << endl;
            }
        }
        // Make a second traversal over all landscape points
        for (int i = 0; i < this->N; i++) {
            for (int j = 0; j < this->N; j++) {
                // For each point, use the calculated number of raindrops that will trickle to the
                // lowest neighbor(s) to update the number of raindrops at each lowest neighbor
                Point p = this->landscape[i][j];
                float amount = p.getTrickleAmount();
                cout << "2nd traversal: trickle amount " << amount << endl;
                if (amount > 0) {
                    p.giveToNeighbor(amount);
                    amount /= (float)p.getNeighbors().size();
                    for (size_t _ = 0; _ < p.getNeighbors().size(); _++) {
                        Point cur = this->landscape[p.getNeighbors()[_].first][p.getNeighbors()[_].second];
                        cur.receiveFromNeighbor(amount);
                    }
                }
                if (p.getRemainingDrops() > 0 || this->timeSteps < this->M) {
                    finished = false;
                }
            }
        }




        finished = true; // for test, DELETE



        if (finished) {
            clock_gettime(CLOCK_MONOTONIC, &end_time);
            double start_sec = (double)start_time.tv_sec * 1000000000.0 + (double)start_time.tv_nsec;
            double end_sec = (double)end_time.tv_sec * 1000000000.0 + (double)end_time.tv_nsec;
            this->totalTime = (end_sec - start_sec) / 1000000000;
            break;
        }
        finished = true; // go to next timestep  
    }
}

void RainfallSim::generateOutput() {
    cout << "Rainfall simulation completed in " << this->timeSteps << " time steps\n";
    cout << "Runtime = " << this->totalTime << " seconds\n";
    cout << "The following grid shows the number of raindrops absorbed at each point:\n";
    for (int i = 0; i < this->N; i++) {
        for (int j = 0; j < this->N; j++) {
            cout << this->landscape[i][j].getAbsorbedDrops();
            if (j != this->N - 1) {
                cout << " ";
            }
        }
        cout << endl;
    }
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
