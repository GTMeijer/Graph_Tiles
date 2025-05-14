// Graph_Tiles.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <stdio.h>
#include <fstream>
#include <iostream>
#include <vector>

#include "Tile.h"

using Grid = std::vector<std::vector<Tile>>;

//std::vector<int> tile_types{ 0b0000, 0b0001, 0b0010, 0b0100, 0b1000 }; //0,1
//std::vector<int> tile_types{ 0b0000, 0b1010, 0b0101, 0b1110, 0b1101, 0b1011, 0b0111 }; //0,2,4
//std::vector<int> tile_types{ 0b0000, 0b1010, 0b0101, 0b1111 }; //0,2,5
std::vector<int> tile_types{ 0b0000, 0b0011,0b0110,0b1100,0b1001,0b1110,0b1101,0b1011,0b0111 }; //0,3,4

int dimension_x = 4;
int dimension_y = 5;

uint64_t total_configs = static_cast<uint64_t>(std::pow(tile_types.size(), dimension_x * dimension_y));
uint64_t call_counter = 0;

std::unordered_map<int, int> tile_type_to_index;
std::unordered_map<int, int> tile_counts;
std::unordered_map<int, int> min_counts;

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

    output.flush();
}

void calculate_progress(const Grid& grid)
{
    uint64_t current_progress = tile_type_to_index[grid[0][0].exits];

    uint64_t total = tile_types.size();

    //for (size_t x = 0; x < grid[0].size(); x++)
    //{
    //    current_progress *= tile_type_to_index[grid[0][x].exits] + 1;
    //}

    //uint64_t total = pow(tile_types.size(), dimension_x);

    std::cout << current_progress << "/" << total << std::endl;


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

bool can_still_meet_min_counts(int remaining_spaces)
{
    int total_required = 0;

    for (const auto& [tile_type, required] : min_counts)
    {
        auto it = tile_counts.find(tile_type);
        int current = (it != tile_counts.end()) ? it->second : 0;

        if (required > current)
            total_required += (required - current);
    }

    return total_required <= remaining_spaces;
}

bool check_min_counts()
{
    for (const auto& [tile_type, required] : min_counts)
    {
        auto it = tile_counts.find(tile_type);
        int current = (it != tile_counts.end()) ? it->second : 0;

        if (current < required)
        {
            return false;
        }
    }

    return true;
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

    // Check if min tile count is still possible
    int remaining_open_tiles = (dimension_x * dimension_y) - (y * dimension_x + x + 1);
    if (!can_still_meet_min_counts(remaining_open_tiles))
    {
        return false;
    }

    return true;
}

void change_tile_counts(int old_tile, int new_tile)
{
    // Decrement old tile count, but not below 0
    auto it = tile_counts.find(Tile::tile_ids.at(old_tile));
    if (it != tile_counts.end() && it->second > 0) {
        --it->second;
    }

    // Increment new tile count (default is 0 if not present)
    ++tile_counts[Tile::tile_ids.at(new_tile)];
}

void backtrack(int index, const int W, const int H, Grid& grid, int& valid_count, std::ofstream& file)
{
    if (index == W * H)
    {
        if (!check_min_counts())
        {
            return;
        }

        // All tiles placed successfully
        print_grid(grid, file);


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

    if (index == 0)
    {
        calculate_progress(grid);
    }
}

std::vector<std::vector<Tile>> init_grid(int width, int height, int starting_type)
{
    return std::vector<std::vector<Tile>>(height, std::vector<Tile>(width, Tile(starting_type)));
}


int main()
{
    std::ofstream file("C:\\dev\\grid_output.txt");

    for (int t : tile_types)
    {
        tile_counts[Tile::tile_ids.at(t)] = 0;
    }

    for (int i = 0; i < tile_types.size(); i++)
    {
        tile_type_to_index[tile_types[i]] = i;
    }

    //Minimal number of each tile type in the solution (0 index)
    min_counts = { { 0, 1 }, { 3, 10 }, { 4,9 } };

    int valid = 0;

    tile_counts[0] = dimension_x * dimension_y;

    int starting_type = tile_types[0];

    Grid grid = init_grid(dimension_x, dimension_y, starting_type);

    backtrack(0, dimension_x, dimension_y, grid, valid, file);

    if (valid >= 0)
    {
        std::cout << "Number of valid configurations: " << valid << std::endl;
    }
    else
    {
        std::cout << "No solutions found." << std::endl;
    }
    file.close();
}