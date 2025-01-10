#include "MatchingEngine.hpp"

namespace hft {

template<Price P, Quantity Q, OrderId ID>
void MatchingEngine<P, Q, ID>::handle_order(typename OrderBook<P, Q, ID>::Order order) {
    match_order(order);
    if (order.quantity > 0) {
        order_book_.add_order(std::move(order));
    }
}

template<Price P, Quantity Q, OrderId ID>
void MatchingEngine<P, Q, ID>::cancel_order(const ID& order_id) {
    order_book_.cancel_order(order_id);
}

template<Price P, Quantity Q, OrderId ID>
void MatchingEngine<P, Q, ID>::match_order(const typename OrderBook<P, Q, ID>::Order& order) {
    // Implement matching logic here
    // This is a simplified version - real implementation would be more complex
    if (order.is_buy) {
        P best_ask = order_book_.best_ask();
        if (best_ask <= order.price) {
            // Match found - execute trade
            if (fill_callback_) {
                fill_callback_(order.id, best_ask, order.quantity);
            }
        }
    } else {
        P best_bid = order_book_.best_bid();
        if (best_bid >= order.price) {
            // Match found - execute trade
            if (fill_callback_) {
                fill_callback_(order.id, best_bid, order.quantity);
            }
        }
    }
}

} // namespace hft 