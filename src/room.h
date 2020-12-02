#include <tuple>

struct Point {
    int x;
    int y;
};

class Room {
    private:
        Point upper_left;
        Point lower_right;

    public:
        Room(int x_pos, int y_pos, int x_size, int y_size);
        bool overlaps(Room other);
        bool overlaps(int x_pos, int y_pos, int x_size, int y_size);

        std::tuple<Point, Point> getCoords() {
            return std::tuple<Point, Point>(upper_left, lower_right);
        }

        Point getUpperLeftPoint() {
            return upper_left;
        }

        Point getLowerRightPoint() {
            return lower_right;
        }
};
