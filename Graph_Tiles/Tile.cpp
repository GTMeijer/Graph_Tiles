﻿#include "Tile.h"

//0001 = left, 0010 = bottom, 0100 = right, 1000 = top
const std::unordered_map<int, const char*> Tile::tile_strings = {
    {0b0000, u8"·"}, {0b0001, u8"╴"}, {0b0010, u8"╷"}, {0b0011, u8"┐"},
    {0b0100, u8"╶"}, {0b0101, u8"─"}, {0b0110, u8"┌"}, {0b0111, u8"┬"},
    {0b1000, u8"╵"}, {0b1001, u8"┘"}, {0b1010, u8"│"}, {0b1011, u8"┤"},
    {0b1100, u8"└"}, {0b1101, u8"┴"}, {0b1110, u8"├"}, {0b1111, u8"┼"}
};

const std::unordered_map<int, int> Tile::tile_ids = {
    {0b0000, 0}, {0b0001, 1}, {0b0010, 1}, {0b0011, 3},
    {0b0100, 1}, {0b0101, 2}, {0b0110, 3}, {0b0111, 4},
    {0b1000, 1}, {0b1001, 3}, {0b1010, 2}, {0b1011, 4},
    {0b1100, 3}, {0b1101, 4}, {0b1110, 4}, {0b1111, 5}
};