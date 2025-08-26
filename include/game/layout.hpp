//
// Created by dvale on 26/08/2025.
//

#ifndef SLOT_PIROTS3_LAYOUT_HPP
#define SLOT_PIROTS3_LAYOUT_HPP

#pragma once
#include <SFML/Graphics.hpp>

struct Layout {
    float cell = 0.f;
    float gridLeft = 0.f;
    float gridTop = 0.f;
    sf::FloatRect spinRect{};
    unsigned textPx = 24;
};

Layout recompute_layout(const sf::Vector2u& winSz);
void on_resize(sf::RenderWindow& win, unsigned w, unsigned h);


#endif //SLOT_PIROTS3_LAYOUT_HPP