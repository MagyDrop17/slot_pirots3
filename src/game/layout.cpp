//
// Created by dvale on 26/08/2025.
//
#include "game/layout.hpp"
#include <algorithm>
#include <cmath>

#include "game/grid.hpp"

static constexpr float SIDE_MARGIN = 8.f;
static constexpr float TOP_MARGIN  = 8.f;
static constexpr float UI_HEIGHT   = 80.f;

Layout recompute_layout(const sf::Vector2u& winSz) {
    Layout L{};
    const float W = static_cast<float>(winSz.x);
    const float H = static_cast<float>(winSz.y);

    const float availW = std::max(0.f, W - 2*SIDE_MARGIN);
    const float availH = std::max(0.f, H - (TOP_MARGIN + UI_HEIGHT));

    const float cellByW = availW / COLS;
    const float cellByH = availH / ROWS;
    L.cell = std::max(8.f, std::floor(std::min(cellByW, cellByH)));

    const float gridW = L.cell * COLS;
    const float gridH = L.cell * ROWS;
    L.gridLeft = std::floor((W - gridW) * 0.5f);
    L.gridTop  = TOP_MARGIN + std::floor((availH - gridH) * 0.5f);

    const sf::Vector2f btnSize(120.f, 40.f);
    const sf::Vector2f btnPos(std::floor((W - btnSize.x) * 0.5f),
                              std::floor(H - UI_HEIGHT * 0.5f - btnSize.y * 0.5f));
    L.spinRect = {btnPos.x, btnPos.y, btnSize.x, btnSize.y};

    L.textPx = static_cast<unsigned>(std::clamp(L.cell * 0.35f, 10.f, 48.f));
    return L;
}

void on_resize(sf::RenderWindow& win, unsigned w, unsigned h) {
    sf::FloatRect visible(0.f, 0.f, static_cast<float>(w), static_cast<float>(h));
    win.setView(sf::View(visible));
}
