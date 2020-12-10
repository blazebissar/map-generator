#include <iostream>
#include <vector>

#include "room.h"
#include "utils.cpp"

const int X_SIZE = 100;
const int Y_SIZE = 100;

int main() {
    std::vector<std::vector<int>> grid(X_SIZE, std::vector<int>(Y_SIZE, 0));

    std::vector<Room> rooms = generateRooms(8, grid.size(), grid[0].size());
    for (auto it: rooms) {
        Point p1;
        Point p2;
        std::tie(p1, p2) = it.getCoords();
        std::cout << p1.x << " " << p1.y << " " << p2.x << " " << p2.y << std::endl;
        for (int i = p1.y; i < p2.y; i++)
            for (int j = p1.x; j < p2.x; j++) {
                grid[i][j]++;
            }
    }

    // Display the rooms
    for (auto i: grid) {
        std::cout << "\n";
        for (auto j: i) {
            if (j)
            //    std::cout << ".";
                std::cout << j;
            else
                std::cout << " ";
        }
    }

    return 0;
};
