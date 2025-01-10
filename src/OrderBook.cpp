#include "OrderBook.hpp"
#include <stdexcept>

namespace hft {

template<Price P, Quantity Q, OrderId ID>
void OrderBook<P, Q, ID>::add_order(Order order) {
    auto& price_map = order.is_buy ? bids_ : asks_;
    price_map[order.price] += order.quantity;
    orders_[order.id] = std::move(order);
}

template<Price P, Quantity Q, OrderId ID>
void OrderBook<P, Q, ID>::cancel_order(const ID& order_id) {
    auto it = orders_.find(order_id);
    if (it == orders_.end()) {
        throw std::runtime_error("Order not found");
    }

    auto& order = it->second;
    auto& price_map = order.is_buy ? bids_ : asks_;
    price_map[order.price] -= order.quantity;
    
    if (price_map[order.price] == 0) {
        price_map.erase(order.price);
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
    auto& price_map = order.is_buy ? bids_ : asks_;
    
    price_map[order.price] -= order.quantity;
    price_map[order.price] += new_quantity;
    
    if (price_map[order.price] == 0) {
        price_map.erase(order.price);
    }
    
    order.quantity = new_quantity;
}

} // namespace hft 