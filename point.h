#include <vector>
using namespace std;

class Point {
    private:
        int elevation;
        float remainingDrops;
        float absorbedDrops;
        float trickleAmount;
        mutex mtx;
        vector<pair<int, int> > neighbors;

    public:
        Point(int elevation);
        ~Point();

        // Neighbor relations
        void addNeighbor(int x, int y);

        // Setters
        void receiveFromSky();
        void receiveFromNeighbor(float amount);
        void giveToNeighbor(float amount);
        void absorb(float amount);
        void setTrickleAmount(float amount);

        // Getters
        int getElevation();
        float getRemainingDrops();
        float getAbsorbedDrops();
        vector<pair<int, int> > getNeighbors();
        float getTrickleAmount();

        // Locks
        void lock();
        void unlock();
};


// ========== Constructor & Destructor ========== //
Point::Point(int elevation) {
    this->elevation = elevation;
    this->remainingDrops = 0.0;
    this->absorbedDrops = 0.0;
    this->trickleAmount = 0.0;
}

Point::~Point() {
    neighbors.clear();
}

// ========== New neighbor ========== //
void Point::addNeighbor(int x, int y) {
    neighbors.push_back(pair<int, int>(x, y));
}

// ========== Setters ========== //
void Point::receiveFromSky() {
    remainingDrops++;
}

void Point::receiveFromNeighbor(float amount) {
    remainingDrops += amount;
}

void Point::giveToNeighbor(float amount) {
    remainingDrops -= amount;
}

void Point::absorb(float amount) {
    absorbedDrops += amount;
    remainingDrops -= amount;
}

void Point::setTrickleAmount(float amount) {
    trickleAmount = amount;
}

// ========== Getters ========== //
int Point::getElevation() {
    return elevation;
}

float Point::getRemainingDrops() {
    return remainingDrops;
}

float Point::getAbsorbedDrops() {
    return absorbedDrops;
}

vector<pair<int, int> > Point::getNeighbors() {
    return neighbors;
}

float Point::getTrickleAmount() {
    return trickleAmount;
}

// ========== Locks ========== //
void Point::lock() {
    mtx.lock();
}

void Point::unlock() {
    mtx.unlock();
}