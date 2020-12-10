#include <stdexcept>
#include "room.h"

Room::Room(int x_pos, int y_pos, int x_size, int y_size) {
    if (x_size < 1 || y_size < 1)
        throw std::invalid_argument("Room must have positive size");

    upper_left = Point{x_pos, y_pos};
    lower_right = Point{x_pos + x_size, y_pos + y_size};
}

bool Room::overlaps(Room other) {
    Point other_p1;
    Point other_p2;

    std::tie(other_p1, other_p2) = other.getCoords();

    if (lower_right.x < other_p1.x || upper_left.x > other_p2.x ||
            lower_right.y < other_p1.y || upper_left.y > other_p2.y)
        return false;

    return true;
}

bool Room::overlaps(int x_pos, int y_pos, int x_size, int y_size) {
    if (lower_right.x < x_pos || upper_left.x > x_pos + x_size ||
            lower_right.y < y_pos || upper_left.y > y_pos + y_size)
        return false;

    return true;

}

bool Room::operator==(Room other) {
    Point other_UL = other.getUpperLeftPoint();
    Point other_LR = other.getLowerRightPoint();

    return upper_left.x == other_UL.x && upper_left.y == other_UL.y &&
        lower_right.x == other_LR.x && lower_right.y == other_LR.y;
}
