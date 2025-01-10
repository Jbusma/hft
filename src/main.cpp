#include "MatchingEngine.hpp"
#include "OrderBook.hpp"
#include "Utils.hpp"
#include <iostream>

int main() {
    // Initialize the matching engine with double prices and int64_t quantities
    hft::MatchingEngine<double, int64_t, uint64_t> engine;
    
    // Set up callback for order fills
    engine.set_fill_callback([](const uint64_t& id, double price, int64_t quantity) {
        std::cout << "Order " << id << " filled: " << quantity << " @ " << price << std::endl;
    });

    // Example order
    typename hft::OrderBook<double, int64_t, uint64_t>::Order order{
        .id = hft::utils::generate_order_id(),
        .price = 100.50,
        .quantity = 100,
        .is_buy = true,
        .timestamp = hft::utils::current_time()
    };

    engine.handle_order(order);
    return 0;
} 