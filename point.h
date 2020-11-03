#include <vector>
using namespace std;

class Point {
    private:
        int elevation;
        int remainingDrops;
        int absorbedDrops;
        vector<pair<int, int> > neighbors;

    public:
        Point(int elevation);
        ~Point();

        // Neighbor relations
        void addNeighbor(int x, int y);

        // Setters
        void receiveFromSky();
        void receiveFromNeighbor(int amount);
        void giveToNeighbor(int amount);
        void absorb(int amount);

        // Getters
        int getElevation();
        int getRemainingDrops();
        int getAbsorbedDrops();
        vector<pair<int, int> > getNeighbors();
};


// ========== Constructor & Destructor ========== //
Point::Point(int elevation) {
    this->elevation = elevation;
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

void Point::receiveFromNeighbor(int amount) {
    remainingDrops += amount;
}

void Point::giveToNeighbor(int amount) {
    remainingDrops -= amount;
}

void Point::absorb(int amount) {
    absorbedDrops += amount;
}

// ========== Getters ========== //
int Point::getElevation() {
    return this->elevation;
}

int Point::getRemainingDrops() {
    return remainingDrops;
}

int Point::getAbsorbedDrops() {
    return absorbedDrops;
}

vector<pair<int, int> > Point::getNeighbors() {
    return neighbors;
}