#include <memory>
#include <stdlib.h>
#include <vector>
#include <iostream>
#include <random>

// Solves the bin packing problem to see if we can fit the provided rooms into
// the grid space. We do not need a perfect solution, because we can always
// regenerate the rooms and try again, so we do not handle things like rotation
// or permuting the rooms
// Returns vector of rooms if successful and empty vector otherwise
std::vector<Room> binPack(std::vector<std::tuple<int, int>> rooms, int x_size, int y_size) {
    std::vector<Room> res;

    // Track the current x/y edges of the rooms packed
    int x_pos = 0;
    int y_pos = 0;
    // Also track the height of the largest room in this row
    int y_jump = 0;

    for (auto it: rooms) {
        // Start by trying to fit in the same row next to the last room
        if (x_pos + std::get<0>(it) < x_size && y_pos + std::get<1>(it) < y_size) {
                res.push_back(Room(x_pos, y_pos, std::get<0>(it), std::get<1>(it)));
            x_pos += std::get<0>(it) + 1;

            // Track the tallest room to start the next row above it to
            // simplify collision checks
            if (std::get<1>(it) > y_jump)
                y_jump = std::get<1>(it);
        }
        // Move up a row to continue packing if it does not fit in the
        // current row
        else {
            x_pos = 0;
            y_pos += y_jump + 1;
            y_jump = 0;

            if (x_pos + std::get<0>(it) < x_size && y_pos + std::get<1>(it) < y_size) {
                res.push_back(Room(x_pos, y_pos, std::get<0>(it), std::get<1>(it)));
                x_pos += std::get<0>(it) + 1;

                // Create dead space to simplify checks in the future
                if (std::get<1>(it) > y_jump)
                    y_jump = std::get<1>(it);
            }
            // Does not fit
            else {
                res.clear();
                return res;
            }
        }
    }
    return res;
}

// Returns the largest rectangle that fits within the surrounding rooms and
// map edges for the specified room
// Edge case where the selected room has nearby x-coordinate neightbors that
// otherwise do not affect its y coordinates
std::tuple<Point, Point> determinePerturbRange(Room room,
        std::vector<Room> rooms, int x_size, int y_size) {
    Point min_xy;
    Point max_xy;
    min_xy.x = 0;
    min_xy.y = 0;
    max_xy.x = x_size - 1;
    max_xy.y = y_size - 1;

    // Grow the height of the room so it collides with the nearest edge or room
    // to create the starting area
    Point my_UL = room.getUpperLeftPoint();
    Point my_LR = room.getLowerRightPoint();
    for (auto it: rooms) {
        Point other_UL = it.getUpperLeftPoint();
        Point other_LR = it.getLowerRightPoint();

        // Do not compare to self
        if (room == it)
            continue;

        if ((other_UL.x >= my_UL.x && other_UL.x <= my_LR.x) ||
                (other_LR.x >= my_UL.x && other_LR.x <= my_LR.x) ||
                (other_UL.x < my_UL.x && other_LR.x > my_LR.x)) {
            if (other_LR.y <= my_UL.y)
                min_xy.y = std::max(min_xy.y, other_LR.y + 1);
            else if (other_UL.y >= my_LR.y)
                max_xy.y = std::min(max_xy.y, other_UL.y - 1);
            else { } // Do nothing
        }
    }

    // Change this starting area to be bounded in width by the nearest
    // edges/rooms
    for (auto it: rooms) {
        Point other_UL = it.getUpperLeftPoint();
        Point other_LR = it.getLowerRightPoint();

        // Do not compare to self
        if (room == it)
            continue;

        if ((other_UL.y >= min_xy.y && other_UL.y <= max_xy.y) ||
                (other_LR.y >= min_xy.y && other_LR.y <= max_xy.y) ||
                (other_UL.y < min_xy.y && other_LR.y > max_xy.y)) {
            if (other_LR.x <= my_UL.x)
                min_xy.x = std::max(min_xy.x, other_LR.x + 1);
            else if (other_UL.x >= my_LR.x)
                max_xy.x = std::min(max_xy.x, other_UL.x - 1);
            else { } // Do nothing
        }
    }

    return std::tuple<Point, Point>(min_xy, max_xy);
}

void perturbRooms(std::vector<Room> *rooms, int x_size, int y_size) {
    std::random_device rand;
    std::mt19937 gen(rand());
    // Reverse iterate to make sure earlier rooms have a bit more room to
    // perturbe
    for (auto it = rooms->rbegin(); it != rooms->rend(); it++) {
        std::tuple<Point, Point> limits = determinePerturbRange(*it, *rooms, x_size, y_size);

        std::uniform_int_distribution<> x_dist(std::get<0>(limits).x,
                std::get<1>(limits).x - it->getWidth());
        std::uniform_int_distribution<> y_dist(std::get<0>(limits).y,
                std::get<1>(limits).y - it->getHeight());
        int new_x = x_dist(gen);
        int new_y = y_dist(gen);

        int width = it->getWidth();
        int height = it->getHeight();

        it->setUpperLeftPoint(new_x, new_y);
        it->setLowerRightPoint(new_x + width, new_y + height);
    }
}

// Creates a number of rooms with random sizes that fit on a specified grid
// size with no overlaps
std::vector<Room> generateRooms(int num, int x_size, int y_size) {
    int max_width = x_size / (num / 4);
    int max_height = y_size / (num / 4);
    const int min_width = 5;
    const int min_height = 5;
    int perturbations = 3;
    std::vector<Room> res;
    std::random_device rand;
    std::mt19937 gen(rand());

    // If our naive algorithm does not pack a group of rooms, try again with a
    // new set until it does
    while (0 == res.size()) {
        std::vector<std::tuple<int, int>> room_sizes;

        for (int i = 0; i < num; i++) {
            std::uniform_int_distribution<> width_dist(min_width, max_width);
            std::uniform_int_distribution<> height_dist(min_height, max_height);
            room_sizes.push_back(std::tuple<int, int>(width_dist(gen), height_dist(gen)));
        }

        res = binPack(room_sizes, x_size, y_size);
    }

    while (perturbations > 0) {
        perturbRooms(&res, x_size, y_size);
        perturbations--;
    }

    return res;
}
