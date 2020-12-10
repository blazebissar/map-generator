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

        int getWidth() {
            return lower_right.x - upper_left.x;
        }

        int getHeight() {
            return lower_right.y - upper_left.y;
        }

        void setUpperLeftPoint(int x, int y) {
            upper_left.x = x;
            upper_left.y = y;
        }

        void setLowerRightPoint(int x, int y) {
            lower_right.x = x;
            lower_right.y = y;
        }

        bool operator==(const Room other);
};
