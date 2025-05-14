// Graph_Tiles.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <stdio.h>
#include <fstream>
#include <iostream>
#include <vector>

#include "Tile.h"

using Grid = std::vector<std::vector<Tile>>;

int print_grid(Grid& grid)
{
    

    if (!file)
    {
        std::cerr << "Could not open file!\n";
        return 1;
    }


    for (const auto& row : grid)
    {
        for (const auto& cell : row)
        {
            file << cell.tile_to_string();
        }

        file << "\n";
    }

    file.close();
}

Grid generate_grid(const std::vector<int>& tiles)
{
    Grid grid;

    int grid_x = 2;
    int grid_y = 2;

    for (size_t j = 0; j < grid_y; j++)
    {
        std::vector<Tile> row;

        for (size_t i = 0; i < grid_x; i++)
        {
            row.push_back(Tile(tiles.at(j * grid_x + i)));
        }

        grid.push_back(row);
    }

    return grid;
}

int main()
{
    std::ofstream file("grid_output.txt");
    int tile_type = 16;

    for (int a = 0; a < 16; ++a)
        for (int b = 0; b < 16; ++b)
            for (int c = 0; c < 16; ++c)
                for (int d = 0; d < 16; ++d)
                {
                    std::vector<int> tiles{ a,b,c,d };
                    Grid g = generate_grid(tiles);
                    print_grid(g);
                }


}