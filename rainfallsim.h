#include <vector>
#include <point.h>
using namespace std;

class RainfallSim {
    private:
        int P, M, A, N;
        int timeSteps;
        int totalRainfall;
        int totalAbsorbed;
        vector<vector<Point> > landscape;

    public:
        RainfallSim(int P, int M, int A, int N, vector<vector<int> > input);
        ~RainfallSim();

        // Call at start and end of simulation
        void startSim();
        void generateOutput();

        // Call during simulation
        void newRain();
        void trickle();

        // Chekers
        bool validPosition(int x, int y);
        bool isLower(Point a, Point b);
        // ...
};