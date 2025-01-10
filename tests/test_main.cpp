#define BOOST_TEST_MODULE HFT Trading System Tests
#include <boost/test/unit_test.hpp>
#include "OrderBook.hpp"
#include "MatchingEngine.hpp"

BOOST_AUTO_TEST_SUITE(OrderBookTests)

BOOST_AUTO_TEST_CASE(test_add_order) {
    hft::OrderBook<double, int64_t, uint64_t> book;
    
    typename hft::OrderBook<double, int64_t, uint64_t>::Order order{
        .id = 1,
        .price = 100.0,
        .quantity = 100,
        .is_buy = true,
        .timestamp = std::chrono::nanoseconds(0)
    };
    
    book.add_order(order);
    BOOST_CHECK_EQUAL(book.volume_at_price(100.0), 100);
}

BOOST_AUTO_TEST_CASE(test_cancel_order) {
    hft::OrderBook<double, int64_t, uint64_t> book;
    
    typename hft::OrderBook<double, int64_t, uint64_t>::Order order{
        .id = 1,
        .price = 100.0,
        .quantity = 100,
        .is_buy = true,
        .timestamp = std::chrono::nanoseconds(0)
    };
    
    book.add_order(order);
    book.cancel_order(1);
    BOOST_CHECK_EQUAL(book.volume_at_price(100.0), 0);
}

BOOST_AUTO_TEST_CASE(test_modify_order) {
    hft::OrderBook<double, int64_t, uint64_t> book;
    
    typename hft::OrderBook<double, int64_t, uint64_t>::Order order{
        .id = 1,
        .price = 100.0,
        .quantity = 100,
        .is_buy = true,
        .timestamp = std::chrono::nanoseconds(0)
    };
    
    book.add_order(order);
    book.modify_order(1, 150);
    BOOST_CHECK_EQUAL(book.volume_at_price(100.0), 150);
}

BOOST_AUTO_TEST_CASE(test_best_prices) {
    hft::OrderBook<double, int64_t, uint64_t> book;
    
    // Add buy order
    typename hft::OrderBook<double, int64_t, uint64_t>::Order buy_order{
        .id = 1,
        .price = 99.0,
        .quantity = 100,
        .is_buy = true,
        .timestamp = std::chrono::nanoseconds(0)
    };
    
    // Add sell order
    typename hft::OrderBook<double, int64_t, uint64_t>::Order sell_order{
        .id = 2,
        .price = 101.0,
        .quantity = 100,
        .is_buy = false,
        .timestamp = std::chrono::nanoseconds(0)
    };
    
    book.add_order(buy_order);
    book.add_order(sell_order);
    
    BOOST_CHECK_EQUAL(book.best_bid(), 99.0);
    BOOST_CHECK_EQUAL(book.best_ask(), 101.0);
}

BOOST_AUTO_TEST_SUITE_END() 

BOOST_AUTO_TEST_SUITE(MatchingEngineTests)

BOOST_AUTO_TEST_CASE(test_matching) {
    hft::MatchingEngine<double, int64_t, uint64_t> engine;
    bool fill_occurred = false;
    
    engine.set_fill_callback([&fill_occurred](const uint64_t& id, double price, int64_t quantity) {
        fill_occurred = true;
    });
    
    // First add a buy order
    typename hft::OrderBook<double, int64_t, uint64_t>::Order buy_order{
        .id = 1,
        .price = 100.0,
        .quantity = 100,
        .is_buy = true,
        .timestamp = std::chrono::nanoseconds(0)
    };
    
    // Then add matching sell order
    typename hft::OrderBook<double, int64_t, uint64_t>::Order sell_order{
        .id = 2,
        .price = 100.0,
        .quantity = 100,
        .is_buy = false,
        .timestamp = std::chrono::nanoseconds(1)
    };
    
    // Add buy order first to establish the bid
    engine.handle_order(buy_order);
    // Then add sell order which should trigger a match
    engine.handle_order(sell_order);
    
    BOOST_CHECK(fill_occurred);
}

BOOST_AUTO_TEST_SUITE_END() 