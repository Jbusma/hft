#pragma once

#include "Concepts.hpp"
#include <unordered_map>
#include <memory>
#include <boost/container/flat_map.hpp>
#include <mutex>

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
    boost::container::flat_map<P, Q, std::greater<P>> bids_;  // Price-time priority
    boost::container::flat_map<P, Q, std::less<P>> asks_;     // Price-time priority
    std::unordered_map<ID, Order> orders_;  // Quick order lookup
    mutable std::mutex book_mutex_;
};

template<Price P, Quantity Q, OrderId ID>
void OrderBook<P, Q, ID>::add_order(Order order) {
    std::lock_guard<std::mutex> lock(book_mutex_);
    if (order.is_buy) {
        bids_[order.price] += order.quantity;
    } else {
        asks_[order.price] += order.quantity;
    }
    orders_[order.id] = std::move(order);
}

template<Price P, Quantity Q, OrderId ID>
void OrderBook<P, Q, ID>::cancel_order(const ID& order_id) {
    std::lock_guard<std::mutex> lock(book_mutex_);
    auto it = orders_.find(order_id);
    if (it == orders_.end()) {
        throw std::runtime_error("Order not found");
    }

    auto& order = it->second;
    if (order.is_buy) {
        bids_[order.price] -= order.quantity;
        if (bids_[order.price] == 0) {
            bids_.erase(order.price);
        }
    } else {
        asks_[order.price] -= order.quantity;
        if (asks_[order.price] == 0) {
            asks_.erase(order.price);
        }
    }
    
    orders_.erase(it);
}

template<Price P, Quantity Q, OrderId ID>
void OrderBook<P, Q, ID>::modify_order(const ID& order_id, Q new_quantity) {
    auto it = orders_.find(order_id);
    if (it == orders_.end()) {
        throw std::runtime_error("Order not found");
    }

    auto& order = it->second;
    if (order.is_buy) {
        bids_[order.price] -= order.quantity;
        bids_[order.price] += new_quantity;
        if (bids_[order.price] == 0) {
            bids_.erase(order.price);
        }
    } else {
        asks_[order.price] -= order.quantity;
        asks_[order.price] += new_quantity;
        if (asks_[order.price] == 0) {
            asks_.erase(order.price);
        }
    }
    
    order.quantity = new_quantity;
}

template<Price P, Quantity Q, OrderId ID>
P OrderBook<P, Q, ID>::best_bid() const {
    std::lock_guard<std::mutex> lock(book_mutex_);
    if (bids_.empty()) {
        throw std::runtime_error("No bids available");
    }
    return bids_.begin()->first;
}

template<Price P, Quantity Q, OrderId ID>
P OrderBook<P, Q, ID>::best_ask() const {
    if (asks_.empty()) {
        throw std::runtime_error("No asks available");
    }
    return asks_.begin()->first;
}

template<Price P, Quantity Q, OrderId ID>
Q OrderBook<P, Q, ID>::volume_at_price(P price) const {
    if (auto it = bids_.find(price); it != bids_.end()) {
        return it->second;
    }
    if (auto it = asks_.find(price); it != asks_.end()) {
        return it->second;
    }
    return 0;
}

} // namespace hft 