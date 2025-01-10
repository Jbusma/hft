#pragma once

#include "OrderBook.hpp"
#include <queue>
#include <functional>

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
    
    void match_order(const typename OrderBook<P, Q, ID>::Order& order);
};

} // namespace hft 