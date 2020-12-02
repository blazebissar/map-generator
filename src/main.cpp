#include <iostream>
#include <vector>

#include "room.h"
#include "utils.cpp"

const int X_SIZE = 150;
const int Y_SIZE = 150;

int main() {
    std::vector<std::vector<bool>> grid(X_SIZE, std::vector<bool>(Y_SIZE, false));

    std::vector<Room> rooms = generateRooms(8, grid.size(), grid[0].size());
    for (auto it: rooms) {
        Point p1;
        Point p2;
        std::tie(p1, p2) = it.getCoords();
        std::cout << p1.x << " " << p1.y << " " << p2.x << " " << p2.y << std::endl;
        for (int i = p1.x; i < p2.x; i++)
            for (int j = p1.y; j < p2.y; j++) {
                grid[i][j] = true;
            }
    }

    // Display the rooms
    for (auto i: grid) {
        std::cout << "\n";
        for (auto j: i) {
            if (j)
                std::cout << ".";
            else
                std::cout << " ";
        }
    }

    return 0;
};
