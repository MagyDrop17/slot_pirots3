//
// Created by dvale on 26/08/2025.
//

#ifndef SLOT_PIROTS3_GRID_HPP
#define SLOT_PIROTS3_GRID_HPP

#pragma once
#include <array>
#include <random>

constexpr int ROWS = 7;
constexpr int COLS = 6;

extern const std::array<char, 7> SYMBOLS;
extern const std::array<int, 7>  WEIGHTS;

using Grid = std::array<std::array<char, COLS>, ROWS>;

char random_symbol();
void generate_grid(Grid& g);

#endif //SLOT_PIROTS3_GRID_HPP