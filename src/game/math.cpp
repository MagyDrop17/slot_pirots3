//
// Created by dvale on 26/08/2025.
//
#include "game/math.hpp"
#include <algorithm>
#include <cmath>
#include <cstdlib>

static inline int payout_len(const Paytable& pt, int len) {
    if (len >= 7) return pt.p7;
    if (len == 6) return pt.p6;
    if (len == 5) return pt.p5;
    if (len == 4) return pt.p4;
    if (len == 3) return pt.p3;
    return 0;
}

int64_t evaluate_payout(const Grid& g, const Paytable& pt) {
    int64_t coins = 0;
    // horizontal
    for (int r=0; r<ROWS; ++r) {
        int c=0;
        while (c < COLS) {
            char s = g[r][c];
            if (s=='X'||s=='T'||s=='P') { ++c; continue; }
            int start = c;
            while (c+1<COLS && g[r][c+1]==s) ++c;
            int len = c - start + 1;
            if (len >= 3) coins += payout_len(pt, len);
            ++c;
        }
    }
    // vertical
    for (int c=0; c<COLS; ++c) {
        int r=0;
        while (r < ROWS) {
            char s = g[r][c];
            if (s=='X'||s=='T'||s=='P') { ++r; continue; }
            int start = r;
            while (r+1<ROWS && g[r+1][c]==s) ++r;
            int len = r - start + 1;
            if (len >= 3) coins += payout_len(pt, len);
            ++r;
        }
    }
    return coins;
}

double simulate_rtp(uint64_t spins, int64_t bet_per_spin, const Paytable& pt) {
    Grid g{};
    int64_t total_bet = 0, total_win = 0;
    for (uint64_t i=0; i<spins; ++i) {
        generate_grid(g);
        total_bet += bet_per_spin;
        total_win += evaluate_payout(g, pt);
    }
    return total_bet ? double(total_win)/double(total_bet) : 0.0;
}

Paytable scale_paytable(const Paytable& pt, double observed_rtp, double target_rtp) {
    if (observed_rtp <= 0.0) return pt;
    const double k = target_rtp / observed_rtp;
    auto sc = [k](int v){ return std::max(1, int(std::llround(v * k))); };
    Paytable out = pt;
    out.p3 = sc(pt.p3); out.p4 = sc(pt.p4); out.p5 = sc(pt.p5);
    out.p6 = sc(pt.p6); out.p7 = sc(pt.p7);
    return out;
}

void calibrate_to_target(Paytable& pt, double target_rtp) {
    const int64_t bet = 100;
    const uint64_t N  = 1'000'000;   // increase if you want tighter error bars
    const double r1 = simulate_rtp(N, bet, pt);
    Paytable sc = scale_paytable(pt, r1, target_rtp);
    const double r2 = simulate_rtp(N, bet, sc);
    if (std::abs(r2 - target_rtp) < std::abs(r1 - target_rtp)) pt = sc;
}

