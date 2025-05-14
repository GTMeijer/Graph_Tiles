// Graph_Tiles.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <stdio.h>
#include <fstream>
#include <iostream>
#include <vector>

#include "Tile.h"

using Grid = std::vector<std::vector<Tile>>;

const std::vector<int> tile_types{ 0, 0b0111, 0b1101, 0b1011, 0b1110 };

int print_grid(Grid& grid, std::ofstream& output)
{
    if (!output)
    {
        std::cerr << "Could not open file!\n";
        return 1;
    }

    for (const auto& row : grid)
    {
        for (const auto& cell : row)
        {
            output << cell.tile_to_string();
        }

        output << "\n";
    }

    output << "\n\n";

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

bool validate_grid(const Grid& grid)
{
    const int height = grid.size();
    const int width = grid[0].size();

    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            Tile tile = grid[y][x];

            // Left exit
            if (tile.exits & 0b0001)
            {
                if (x > 0)
                {
                    Tile neighbor = grid[y][x - 1];
                    if ((neighbor.exits & 0b0100) == 0) return false; // Neighbor missing right
                }
            }

            // Bottom exit
            if (tile.exits & 0b0010)
            {
                if (y + 1 < height)
                {
                    Tile neighbor = grid[y + 1][x];
                    if ((neighbor.exits & 0b1000) == 0) return false; // Neighbor missing top
                }
            }

            // Right exit
            if (tile.exits & 0b0100)
            {
                if (x + 1 < width)
                {
                    Tile neighbor = grid[y][x + 1];
                    if ((neighbor.exits & 0b0001) == 0) return false; // Neighbor missing left
                }
            }

            // Top exit
            if (tile.exits & 0b1000)
            {
                if (y > 0)
                {
                    Tile neighbor = grid[y - 1][x];
                    if ((neighbor.exits & 0b0010) == 0) return false; // Neighbor missing bottom
                }
            }
        }
    }

    return true;
}

bool backtrack_placement_valid(Grid& grid, const int y, const int x)
{
    Tile tile = grid[y][x];

    // Left exit
    if (tile.exits & 0b0001)
    {
        if (x > 0)
        {
            Tile neighbor = grid[y][x - 1];
            if ((neighbor.exits & 0b0100) == 0) return false; // Neighbor missing right
        }
    }

    // No left exit
    if (!(tile.exits & 0b0001))
    {
        if (x > 0)
        {
            Tile neighbor = grid[y][x - 1];
            if ((neighbor.exits & 0b0100) != 0) return false; // Neighbor right
        }
    }

    // Top exit
    if (tile.exits & 0b1000)
    {
        if (y > 0)
        {
            Tile neighbor = grid[y - 1][x];
            if ((neighbor.exits & 0b0010) == 0) return false; // Neighbor missing bottom
        }
    }

    // No top exit
    if (!(tile.exits & 0b1000))
    {
        if (y > 0)
        {
            Tile neighbor = grid[y - 1][x];
            if ((neighbor.exits & 0b0010) != 0) return false; // Neighbor bottom
        }
    }

    return true;
}

void backtrack(int index, const int W, const int H, Grid& grid, int& valid_count, std::ofstream& file)
{
    if (index == W * H)
    {
        // All tiles placed successfully
        print_grid(grid, file);
        //file.flush(); //giga slow

        valid_count++;

        return;
    }

    for (int tile = 0; tile < tile_types.size(); ++tile)
    {
        int index_y = index / W;
        int index_x = index % W;


        grid[index_y][index_x].exits = tile_types[tile];
        if (backtrack_placement_valid(grid, index_y, index_x))
        {
            backtrack(index + 1, W, H, grid, valid_count, file);
        }
    }
}

std::vector<std::vector<Tile>> init_grid(int width, int height)
{
    return std::vector<std::vector<Tile>>(height, std::vector<Tile>(width, Tile()));
}

int main()
{



    std::ofstream file("C:\\dev\\grid_output.txt");
    int tile_types = 16;

    int valid = 0;

    const int dimension_x = 4;
    const int dimension_y = 4;

    Grid grid = init_grid(dimension_x, dimension_y);

    backtrack(0, dimension_x, dimension_y, grid, valid, file);


    std::cout << "Number of valid configurations: " << valid << "/" <<  << std::endl;

    //for (int a = 0; a < 16; ++a)
    //{
    //    std::cout << a << std::endl;
    //    for (int b = 0; b < 16; ++b)
    //    {
    //        for (int c = 0; c < 16; ++c)
    //        {
    //            for (int d = 0; d < 16; ++d)
    //            {
    //                std::vector<int> tiles{ a,b,c,d };
    //                Grid g = generate_grid(tiles);
    //                if (validate_grid(g))
    //                {
    //                    print_grid(g, file);
    //                    //file.flush();
    //                }
    //            }
    //        }
    //    }
    //}

    file.close();
}