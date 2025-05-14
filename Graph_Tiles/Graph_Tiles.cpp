// Graph_Tiles.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <stdio.h>
#include <fstream>
#include <iostream>
#include <vector>

#include "Tile.h"

using Grid = std::vector<std::vector<Tile>>;

//const std::vector<int> tile_types{ 0b0000, 0b0001, 0b0010, 0b0100, 0b1000 }; //1,2
//const std::vector<int> tile_types{ 0b0000, 0b1010, 0b0101, 0b1110, 0b1101, 0b1011, 0b0111 }; //1,3,5
const std::vector<int> tile_types{ 0b0000, 0b1010, 0b0101, 0b1111 }; //1,3,6

const int dimension_x = 11;
const int dimension_y = 11;

uint64_t total_configs = static_cast<uint64_t>(std::pow(tile_types.size(), dimension_x * dimension_y));
uint64_t call_counter = 0;

std::unordered_map<int, int> tile_counts;

void print_grid(const Grid& grid, std::ofstream& output)
{
    if (!output)
    {
        std::cerr << "Could not open file!\n";
        return;
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

int count_bits(int value)
{
    int count = 0;
    while (value)
    {
        count += value & 1;
        value >>= 1;
    }

    return count;
}

bool backtrack_placement_valid(Grid& grid, const int y, const int x)
{
    Tile tile = grid[y][x];

    // Left exit
    if (tile.exits & 0b0001 && x > 0)
    {
        Tile neighbor = grid[y][x - 1];
        if ((neighbor.exits & 0b0100) == 0) return false; // Neighbor missing right
    }

    // No left exit
    if (!(tile.exits & 0b0001) && x > 0)
    {
        Tile neighbor = grid[y][x - 1];
        if ((neighbor.exits & 0b0100) != 0) return false; // Neighbor right
    }

    // Top exit
    if (tile.exits & 0b1000 && y > 0)
    {
        Tile neighbor = grid[y - 1][x];
        if ((neighbor.exits & 0b0010) == 0) return false; // Neighbor missing bottom
    }

    // No top exit
    if (!(tile.exits & 0b1000) && y > 0)
    {
        Tile neighbor = grid[y - 1][x];
        if ((neighbor.exits & 0b0010) != 0) return false; // Neighbor bottom
    }

    return true;
}

void change_tile_counts(int old_tile, int new_tile)
{
    // Decrement old tile count, but not below 0
    auto it = tile_counts.find(count_bits(old_tile));
    if (it != tile_counts.end() && it->second > 0) {
        --it->second;
    }

    // Increment new tile count (default is 0 if not present)
    ++tile_counts[count_bits(new_tile)];
}

void backtrack(int index, const int W, const int H, Grid& grid, int& valid_count, std::ofstream& file)
{
    call_counter++;
    if (call_counter % 100000 == 0)
    {
        double percent = (100.0 * call_counter) / total_configs;
        std::cout << "Checked: " << call_counter << " (" << percent << "%)\n";
    }

    if (index == W * H)
    {
        //Check if we use at least one of each tile
        for (const auto& [tile, count] : tile_counts)
        {
            if (count == 0)
                return;
        }

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

        change_tile_counts(grid[index_y][index_x].exits, tile_types[tile]);

        grid[index_y][index_x].exits = tile_types[tile];
        if (backtrack_placement_valid(grid, index_y, index_x))
        {
            backtrack(index + 1, W, H, grid, valid_count, file);
        }
    }
}

std::vector<std::vector<Tile>> init_grid(int width, int height, int starting_type)
{
    return std::vector<std::vector<Tile>>(height, std::vector<Tile>(width, Tile(starting_type)));
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

    for (auto& t : tile_types)
    {
        tile_counts[count_bits(t)] = 0;

    }

    int valid = 0;

    tile_counts[0] = dimension_x * dimension_y;

    int starting_type = tile_types[0];

    Grid grid = init_grid(dimension_x, dimension_y, starting_type);

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