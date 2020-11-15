#include <vector>
#include <time.h>
#include <thread>
#include <mutex>
#include "point.h"

using namespace std;

class RainfallSim {
    private:
        int P, M, N;
        float A;
        int timeSteps;
        vector<vector<Point*> > landscape;
        double totalTime;
        bool finished;

    public:
        RainfallSim(int P, int M, float A, int N, vector<vector<int> > input);
        ~RainfallSim();

        // Call at start and end of simulation
        void startSim_seq();
        void startSim_pt();
        void generateOutput();

        // Call during simulation: parallelization helpers
        void newRain(int id, int size);
        void trickle(int id, int size);

        // Chekers
        bool validPosition(int x, int y);

};

struct arg_struct {
    RainfallSim * sim;
    int id;
    int size;
};

// wrappers
static void* newRain_wrapper(void* object) {
    struct arg_struct * args = (struct arg_struct *)object;
    reinterpret_cast<RainfallSim*>(args->sim)->newRain(args->id, args->size);
    return 0;
}

static void* trickle_wrapper(void* object) {
    struct arg_struct * args = (struct arg_struct *)object;
    reinterpret_cast<RainfallSim*>(args->sim)->trickle(args->id, args->size);
    return 0;
}


// ========== Constructor & Destructor ========== //
RainfallSim::RainfallSim(int P, int M, float A, int N, vector<vector<int> > input) {
    this->P = P;
    this->M = M;
    this->A = A;
    this->N = N;
    this->timeSteps = 0;
    this->finished = false;

    // populate landscape
    const int dx[] = {0, 1, 0, -1};
    const int dy[] = {-1, 0, 1, 0};
    for (int i = 0; i < N; i++) {
        vector<Point*> tmp;
        for (int j = 0; j < N; j++) {
            Point* p = new Point(input[i][j]);
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
                p->addNeighbor(lowest[ind].first, lowest[ind].second);
            }
            tmp.push_back(p);
        }
        this->landscape.push_back(tmp);
    }
}

RainfallSim::~RainfallSim() {}


// ========== Call at start and end of simulation ========== //
// Sequential version
void RainfallSim::startSim_seq() {
    struct timespec start_time, end_time;
    clock_gettime(CLOCK_MONOTONIC, &start_time);
    while(true) {
        this->finished = true;
        this->timeSteps++;
        
        // Traverse over all landscape points
        newRain(0, this->N);
        trickle(0, this->N);

        if (this->finished) {
            clock_gettime(CLOCK_MONOTONIC, &end_time);
            double start_sec = (double)start_time.tv_sec * 1000000000.0 + (double)start_time.tv_nsec;
            double end_sec = (double)end_time.tv_sec * 1000000000.0 + (double)end_time.tv_nsec;
            this->totalTime = (end_sec - start_sec) / 1000000000;
            break;
        }
    }
}

// Parallel version
void RainfallSim::startSim_pt() {
    struct timespec start_time, end_time;
    clock_gettime(CLOCK_MONOTONIC, &start_time);

    // array of argument structs for BOOST
    arg_struct *structs = new arg_struct[this->P];

    // Get problem size
    int size = this->N / this->P;

    while(true) {
        this->finished = true;
        this->timeSteps++;

        // Traverse over all landscape points
        vector<thread*> container;
        for (int i = 0; i < this->P; i++) {
            structs[i].sim = this;
            structs[i].id = i;
            structs[i].size = size;
            container.push_back(new thread(&newRain_wrapper, (void *)&structs[i]));
        }
        for (int i = 0; i < this->P; i++) {
            container[i]->join();
        }
        container.clear();

        for (int i = 0; i < this->P; i++) {
            structs[i].sim = this;
            structs[i].id = i;
            structs[i].size = size;
            container.push_back(new thread(&trickle_wrapper, (void *)&structs[i]));
        }
        for (int i = 0; i < this->P; i++) {
            container[i]->join();
        }
        container.clear();
        
        if (this->finished) {
            clock_gettime(CLOCK_MONOTONIC, &end_time);
            double start_sec = (double)start_time.tv_sec * 1000000000.0 + (double)start_time.tv_nsec;
            double end_sec = (double)end_time.tv_sec * 1000000000.0 + (double)end_time.tv_nsec;
            this->totalTime = (end_sec - start_sec) / 1000000000;
            break;
        }
    }
}


void RainfallSim::generateOutput() {
    cout << "Rainfall simulation completed in " << this->timeSteps << " time steps\n";
    cout << "Runtime = " << this->totalTime << " seconds\n";
    cout << "The following grid shows the number of raindrops absorbed at each point:\n";
    for (int i = 0; i < this->N; i++) {
        for (int j = 0; j < this->N; j++) {
            cout << this->landscape[i][j]->getAbsorbedDrops();
            if (j != this->N - 1) {
                cout << " ";
            }
        }
        cout << endl;
    }
}


// ========== Call during simulation: parallelization helpers ========== //
void RainfallSim::newRain(int id, int size) {
    // Traverse over all landscape points
    for (int i = id * size; i < (id + 1) * size; i++) {
        for (int j = 0; j < this->N; j++) {
            Point* p = this->landscape[i][j];
            // 1) Receive a new raindrop (if it is still raining) for each point.
            if (timeSteps <= this->M) {
                p->receiveFromSky();
            }
            // 2) If there are raindrops on a point, absorb water into the point
            float remaining = p->getRemainingDrops();
            if (remaining > 0) {
                p->absorb(this->A < remaining ? this->A : remaining);
            }
            // 3a) Calculate the number of raindrops that will next trickle to the lowest neighbor(s)
            remaining = p->getRemainingDrops();
            if (p->getNeighbors().size() != 0) {
                float trickleAmount = remaining <= 0 ? 0 : (remaining < 1 ? remaining : 1);
                p->setTrickleAmount(trickleAmount);
            }
        }
    }
}


void RainfallSim::trickle(int id, int size) {
    // Make a second traversal over all landscape points
    for (int i = id * size; i < (id + 1) * size; i++) {
        for (int j = 0; j < this->N; j++) {
            // For each point, use the calculated number of raindrops that will trickle to the
            // lowest neighbor(s) to update the number of raindrops at each lowest neighbor
            Point* p = this->landscape[i][j];
            float amount = p->getTrickleAmount();
            if (amount > 0) {
                p->giveToNeighbor(amount);
                amount /= (float)p->getNeighbors().size();
                for (size_t _ = 0; _ < p->getNeighbors().size(); _++) {
                    Point* cur = this->landscape[p->getNeighbors()[_].first][p->getNeighbors()[_].second];
                    
                    // Update trickled rain
                    int rem = p->getNeighbors()[_].first % size;
                    if (rem == 0 || rem == size-1) {
                        cur->lock();
                        cur->receiveFromNeighbor(amount);
                        cur->unlock();
                    }
                    else {
                        cur->receiveFromNeighbor(amount);
                    }
                }
            }
            if (p->getRemainingDrops() > 0 || this->timeSteps < this->M) {
                this->finished = false;
            }
        }
    }
}


// ========== Checkers ========== //
bool RainfallSim::validPosition(int x, int y) {
    return x >= 0 && x < N && y >= 0 && y < N;
}