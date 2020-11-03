#include <vector>
using namespace std;

class Point {
    private:
        int elevation;
        int remainingDrops;
        int absorbedDrops;

    public:
        Point(int elevation);
        ~Point();

        // Setters
        void receiveFromSky();
        void receiveFromNeighbor(int amount);
        void absorb(int amount);

        // Getters
        int getElevation();
        int getRemainingDrops();
        int getAbsorbedDrops();
};