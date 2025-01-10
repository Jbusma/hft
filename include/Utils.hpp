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

} // namespace hft::utils 