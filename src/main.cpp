#include "MatchingEngine.hpp"
#include "OrderBook.hpp"
#include "Utils.hpp"
#include <iostream>
#include <memory>

int main() {
    // Use smart pointers for automatic cleanup
    auto engine = std::make_unique<hft::MatchingEngine<double, int64_t, uint64_t>>();
    
    engine->set_fill_callback([](const uint64_t& id, double price, int64_t quantity) {
        std::cout << "Order " << id << " filled: " << quantity << " @ " << price << std::endl;
    });

    typename hft::OrderBook<double, int64_t, uint64_t>::Order order{
        .id = hft::utils::generate_order_id(),
        .price = 100.50,
        .quantity = 100,
        .is_buy = true,
        .timestamp = hft::utils::current_time()
    };

    engine->handle_order(order);
    return 0;  // Smart pointer automatically cleans up
} 