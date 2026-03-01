#include <cmath>

class City
{
public:
    int id;
    double x;
    double y;
    bool is_connected;

    City(int id, double x, double y) : id(id), x(x), y(y), is_connected(false) {}

    // O(1)
    double distance_from(City &other)
    {
        return sqrt(pow(x - other.x, 2) + pow(y - other.y, 2));
    }

    // void print() {
    //     std::cout << "ID " << id << ": (" << x << ", " << y << ")" << std::endl;
    // }
};