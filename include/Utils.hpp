#pragma once

#include <chrono>
#include <random>
#include <atomic>

namespace hft::utils {

// High-precision clock utilities
inline std::chrono::nanoseconds current_time() {
    return std::chrono::high_resolution_clock::now().time_since_epoch();
}

// Thread-safe unique ID generation
inline uint64_t generate_order_id() {
    static std::atomic<uint64_t> next_id{0};
    return ++next_id;
}

// Price level utilities
template<typename P>
P tick_size() {
    if constexpr (std::is_floating_point_v<P>) {
        return P{0.01};  // 1 cent for floating point prices
    } else {
        return P{1};     // 1 tick for integer prices
    }
}

// Round price to nearest tick
template<typename P>
P round_to_tick(P price) {
    P tick = tick_size<P>();
    return std::round(price / tick) * tick;
}

} // namespace hft::utils 