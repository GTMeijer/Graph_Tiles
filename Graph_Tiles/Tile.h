#pragma once

#include <string>
#include <array>
#include <unordered_map>
#include <map>

class Tile
{
public:

    static const std::unordered_map<int, const char*> tile_strings;

    explicit Tile(int exits) : exits(exits)
    {

    }

    uint8_t exits;

    uint8_t rotate_tile(int times_clockwise_90)
    {
        times_clockwise_90 %= 4;
        for (int i = 0; i < times_clockwise_90; ++i) {
            // Rotate bits right with wrap-around
            exits = ((exits >> 1) | ((exits & 1) << 3)) & 0b1111;
        }
        return exits;
    }

    std::string tile_to_string() const
    {
        auto it = tile_strings.find(exits);
        return it != tile_strings.end() ? it->second : "?";
    }
};

