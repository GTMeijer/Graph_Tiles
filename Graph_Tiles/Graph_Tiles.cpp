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

// https://stackoverflow.com/a/101613/3738557
unsigned long long int ipow(int base, int exp)
{
    unsigned long long int result = 1;
    unsigned long long int lbase = base;
    for (;;)
    {
        if (exp & 1)
            result *= lbase;
        exp >>= 1;
        if (!exp)
            break;
        lbase *= lbase;
    }

    return result;
}

int main()
{
    std::ofstream file("C:\\dev\\grid_output.txt");

    int valid = 0;

    const int dimension_x = 4;
    const int dimension_y = 4;

    Grid grid = init_grid(dimension_x, dimension_y);

    backtrack(0, dimension_x, dimension_y, grid, valid, file);

    if (valid >= 0)
    {
        //std::cout << "Number of valid configurations: " << valid << "/" << ipow(tile_types.size(), dimension_x * dimension_y) << std::endl;
        std::cout << "Number of valid configurations: " << valid << std::endl;
    }
    else
    {
        std::cout << "No solutions found." << std::endl;
    }
    file.close();
}