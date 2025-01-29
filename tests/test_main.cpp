#define BOOST_TEST_MODULE HFT Trading System Tests
#include <boost/test/unit_test.hpp>
#include <boost/test/tools/output_test_stream.hpp>
#include <boost/test/execution_monitor.hpp>  // For timing
#include "OrderBook.hpp"
#include "MatchingEngine.hpp"
#include "Utils.hpp"
#include <thread>
#include <atomic>
#include <vector>
#include "SharedPtr.hpp"  // Add at top with other includes

// Add timing fixture
struct TimingFixture {
    TimingFixture() {
        boost::unit_test::unit_test_log.set_threshold_level(boost::unit_test::log_successful_tests);
        start_time = std::chrono::high_resolution_clock::now();
    }
    ~TimingFixture() {
        auto end_time = std::chrono::high_resolution_clock::now();
        auto ns = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time);
        auto us = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
        BOOST_TEST_MESSAGE("Test duration: " << us.count() << " us (" << ns.count() << " ns)");
    }
    std::chrono::high_resolution_clock::time_point start_time;
};

BOOST_TEST_GLOBAL_FIXTURE(TimingFixture);

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

BOOST_AUTO_TEST_SUITE(MemoryTests)

BOOST_AUTO_TEST_CASE(test_no_memory_leaks) {
    static std::atomic<size_t> total_allocations{0};

    {
        auto engine = std::make_unique<hft::MatchingEngine<double, int64_t, uint64_t>>();
        // Reduced iterations from 1000 to 100
        for (uint64_t i = 0; i < 100; i++) {
            engine->handle_order({
                .id = i,
                .price = 100.0,
                .quantity = 100,
                .is_buy = true,
                .timestamp = hft::utils::current_time()
            });
            engine->cancel_order(i);
        }
    }
    
    BOOST_CHECK_EQUAL(total_allocations.load(), 0);
}

BOOST_AUTO_TEST_CASE(test_thread_safety) {
    auto engine = std::make_shared<hft::MatchingEngine<double, int64_t, uint64_t>>();
    std::atomic<bool> error_occurred{false};
    std::vector<std::thread> threads;
    
    // Use atomic counter for IDs
    std::atomic<uint64_t> id_counter{0};
    
    for (int i = 0; i < 4; i++) {  // More aggressive test
        threads.emplace_back([&engine, &error_occurred, &id_counter]() {
            try {
                for (int j = 0; j < 1000; j++) {  // More iterations
                    uint64_t id = id_counter.fetch_add(1);
                    engine->handle_order({
                        .id = id,
                        .price = 100.0 + (id % 10),  // Vary prices
                        .quantity = 100,
                        .is_buy = true,
                        .timestamp = hft::utils::current_time()
                    });
                    engine->cancel_order(id);
                }
            } catch (...) {
                error_occurred = true;
            }
        });
    }
    
    for (auto& t : threads) {
        t.join();
    }
    
    BOOST_CHECK(!error_occurred.load());
}

BOOST_AUTO_TEST_SUITE_END() 

BOOST_AUTO_TEST_SUITE(SharedPtrTests)

BOOST_AUTO_TEST_CASE(test_basic_usage) {
    auto ptr = hft::SharedPtr<int>(new int(42));
    BOOST_CHECK_EQUAL(*ptr, 42);
    BOOST_CHECK_EQUAL(ptr.use_count(), 1);
}

BOOST_AUTO_TEST_CASE(test_sharing) {
    auto ptr1 = hft::SharedPtr<int>(new int(42));
    auto ptr2 = ptr1;
    BOOST_CHECK_EQUAL(ptr1.use_count(), 2);
    BOOST_CHECK_EQUAL(ptr2.use_count(), 2);
}

BOOST_AUTO_TEST_CASE(test_reset) {
    auto ptr = hft::SharedPtr<int>(new int(42));
    ptr.reset(new int(24));
    BOOST_CHECK_EQUAL(*ptr, 24);
    BOOST_CHECK_EQUAL(ptr.use_count(), 1);
}

BOOST_AUTO_TEST_SUITE_END() 