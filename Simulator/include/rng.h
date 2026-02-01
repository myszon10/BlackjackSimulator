#pragma once
#include <random>
#include <cstdint>
#include <thread>
#include <functional>

namespace RNG {

    inline std::mt19937_64& Engine() {
        thread_local std::mt19937_64 eng{ [] {
            std::random_device rd;
            std::seed_seq seq{
                rd(), rd(), rd(), rd(),
                (unsigned)reinterpret_cast<std::uintptr_t>(&rd),
                (unsigned)std::hash<std::thread::id>{}(std::this_thread::get_id())
            };
            return std::mt19937_64(seq);
        }() };

        return eng;
    }

    inline int Int(int lo, int hi) {
        std::uniform_int_distribution<int> dist(lo, hi);
        return dist(Engine());
    }

    inline double Real(double lo = 0.0, double hi = 1.0) {
        std::uniform_real_distribution<double> dist(lo, hi);
        return dist(Engine());
    }

    inline bool Chance(double p) {
        return Real() < p;
    }
}
