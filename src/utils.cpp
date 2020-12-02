#include <memory>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <iostream>

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
        std::cout << "iterating" << std::get<0>(it) << " " << std::get<1>(it) << "\n";
        // Too tall room stack, cannot fit
        if (y_pos + std::get<1>(it) >= y_size) {
            res.clear();
            return res;
        }

        // Start by trying to fit in the same row next to the last room
        if (x_pos + std::get<0>(it) < x_size) {
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

            if (x_pos + std::get<0>(it) < x_size) {
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

        // TODO: introduce some jitter to avoid clustering rooms

    }
    return res;
}

// Creates a number of rooms with random sizes that fit on a specified grid
// size with no overlaps
std::vector<Room> generateRooms(int num, int x_size, int y_size) {
    int max_width = x_size / (num / 4);
    int max_height = y_size / (num / 4);
    int min_width = 5;
    int min_height = 5;
    std::vector<Room> res;
    srand(time(NULL));

    int iterations = 0;

    while (0 == res.size()) {
        std::cout << "iterations: " << iterations << "\n";
        iterations++;
        std::vector<std::tuple<int, int>> room_sizes;

        for (int i = 0; i < num; i++) {
            room_sizes.push_back(std::tuple<int, int>(rand() % max_width + min_width, rand() % max_height + min_height));
        }

        res = binPack(room_sizes, x_size, y_size);
    }

    return res;
}

/*
 * Generate n rooms
 * Attempt to bin pack
 * If failed, try again with a smaller largest room, based on area
 * Continue until it works
 */
