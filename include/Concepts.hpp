#pragma once

#include <concepts>
#include <type_traits>
#include <chrono>

namespace hft {

// Price concept - requires arithmetic operations
template<typename T>
concept Price = std::is_arithmetic_v<T>;

// Quantity concept - requires integral type
template<typename T>
concept Quantity = std::is_integral_v<T>;

// Timestamp concept - requires chrono compatibility
template<typename T>
concept Timestamp = requires(T t) {
    { t.time_since_epoch() } -> std::convertible_to<std::chrono::nanoseconds>;
};

// Order ID concept
template<typename T>
concept OrderId = std::is_integral_v<T> || std::is_same_v<T, std::string>;

} // namespace hft 