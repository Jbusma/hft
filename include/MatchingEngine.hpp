#pragma once

#include "OrderBook.hpp"
#include <queue>
#include <functional>
#include <mutex>

namespace hft {

template<Price P, Quantity Q, OrderId ID>
class MatchingEngine {
public:
    using OrderCallback = std::function<void(const ID&, P, Q)>;
    
    void set_fill_callback(OrderCallback callback) {
        fill_callback_ = std::move(callback);
    }

    void handle_order(typename OrderBook<P, Q, ID>::Order order);
    void cancel_order(const ID& order_id);

private:
    OrderBook<P, Q, ID> order_book_;
    OrderCallback fill_callback_;
    std::mutex engine_mutex_;
    
    void match_order(const typename OrderBook<P, Q, ID>::Order& order);
};

template<Price P, Quantity Q, OrderId ID>
void MatchingEngine<P, Q, ID>::handle_order(typename OrderBook<P, Q, ID>::Order order) {
    order_book_.add_order(order);
    
    // Then try to match
    try {
        if (order.is_buy) {
            P best_ask = order_book_.best_ask();
            if (best_ask <= order.price) {
                if (fill_callback_) {
                    fill_callback_(order.id, best_ask, order.quantity);
                }
            }
        } else {
            P best_bid = order_book_.best_bid();
            if (best_bid >= order.price) {
                if (fill_callback_) {
                    fill_callback_(order.id, best_bid, order.quantity);
                }
            }
        }
    } catch (const std::runtime_error&) {
        // No matching orders available
    }
}

template<Price P, Quantity Q, OrderId ID>
void MatchingEngine<P, Q, ID>::cancel_order(const ID& order_id) {
    order_book_.cancel_order(order_id);
}

template<Price P, Quantity Q, OrderId ID>
void MatchingEngine<P, Q, ID>::match_order(const typename OrderBook<P, Q, ID>::Order& order) {
    std::lock_guard<std::mutex> lock(engine_mutex_);
    // Placeholder for future matching logic
}

} // namespace hft 