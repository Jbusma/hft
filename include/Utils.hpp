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

// Add likely/unlikely macros
#ifdef __GNUC__
#define likely(x)       __builtin_expect(!!(x), 1)
#define unlikely(x)     __builtin_expect(!!(x), 0)
#else
#define likely(x)       (x)
#define unlikely(x)     (x)
#endif

// Prefetch instructions for market data
inline void prefetch(const void* addr) {
    __builtin_prefetch(addr);
}

} // namespace hft::utils 