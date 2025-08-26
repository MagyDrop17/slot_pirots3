//
// Created by dvale on 26/08/2025.
//
#include "game/grid.hpp"
#include <numeric>

const std::array<char, 7> SYMBOLS = {'R','G','B','Y','X','T','P'};
const std::array<int, 7>  WEIGHTS = { 20, 20, 20, 20,  5, 10,  5 };

char random_symbol() {
    static std::mt19937 rng{std::random_device{}()};
    int total = std::accumulate(WEIGHTS.begin(), WEIGHTS.end(), 0);
    std::uniform_int_distribution<int> d(0, total - 1);
    int r = d(rng);
    for (size_t i=0; i<WEIGHTS.size(); ++i) {
        if (r < WEIGHTS[i]) return SYMBOLS[i];
        r -= WEIGHTS[i];
    }
    return 'R';
}

void generate_grid(Grid& g) {
    for (auto& row : g)
        for (auto& cell : row)
            cell = random_symbol();
}