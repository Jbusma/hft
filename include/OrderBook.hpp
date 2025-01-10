#pragma once

#include "Concepts.hpp"
#include <map>
#include <unordered_map>
#include <memory>

namespace hft {

template<Price P, Quantity Q, OrderId ID>
class OrderBook {
public:
    struct Order {
        ID id;
        P price;
        Q quantity;
        bool is_buy;
        std::chrono::nanoseconds timestamp;
    };

    // Core operations
    void add_order(Order order);
    void cancel_order(const ID& order_id);
    void modify_order(const ID& order_id, Q new_quantity);
    
    // View operations
    P best_bid() const;
    P best_ask() const;
    Q volume_at_price(P price) const;

private:
    std::map<P, Q, std::greater<P>> bids_;  // Price-time priority
    std::map<P, Q, std::less<P>> asks_;     // Price-time priority
    std::unordered_map<ID, Order> orders_;  // Quick order lookup
};

} // namespace hft 