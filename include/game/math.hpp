//
// Created by dvale on 26/08/2025.
//

#ifndef SLOT_PIROTS3_MATH_HPP
#define SLOT_PIROTS3_MATH_HPP

#pragma once
#include "grid.hpp"
#include <cstdint>

struct Paytable {
    int p3 = 5, p4 = 15, p5 = 40, p6 = 120, p7 = 300;
};

int64_t evaluate_payout(const Grid& g, const Paytable& pt);

double simulate_rtp(uint64_t spins, int64_t bet_per_spin, const Paytable& pt);
Paytable scale_paytable(const Paytable& pt, double observed_rtp, double target_rtp);
void    calibrate_to_target(Paytable& pt, double target_rtp = 0.98);


#endif //SLOT_PIROTS3_MATH_HPP