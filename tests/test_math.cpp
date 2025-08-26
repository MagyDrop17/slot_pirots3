//
// Created by dvale on 26/08/2025.
//

#include "game/math.hpp"
#include <iostream>

int main() {
    Paytable pt; // default
    const int64_t bet = 100;
    const uint64_t N = 2'000'000;

    double r1 = simulate_rtp(N, bet, pt);
    std::cout << "Observed RTP before scale: " << r1 << "\n";

    Paytable sc = scale_paytable(pt, r1, 0.98);
    double r2 = simulate_rtp(N, bet, sc);
    std::cout << "Observed RTP after scale:  " << r2 << "\n";

    std::cout << "Paytable used: p3="<<sc.p3<<" p4="<<sc.p4<<" p5="<<sc.p5
              <<" p6="<<sc.p6<<" p7="<<sc.p7<<"\n";
    return 0;
}