#include <benchmark/benchmark.h>
#include "OrderBook.hpp"
#include "MatchingEngine.hpp"

static void BM_OrderBookAdd(benchmark::State& state) {
    hft::OrderBook<double, int64_t, uint64_t> book;
    uint64_t order_id = 0;

    for (auto _ : state) {
        typename hft::OrderBook<double, int64_t, uint64_t>::Order order{
            .id = ++order_id,
            .price = 100.0,
            .quantity = 100,
            .is_buy = true,
            .timestamp = hft::utils::current_time()
        };
        book.add_order(order);
    }
}
BENCHMARK(BM_OrderBookAdd);

static void BM_OrderMatching(benchmark::State& state) {
    hft::MatchingEngine<double, int64_t, uint64_t> engine;
    uint64_t order_id = 0;

    for (auto _ : state) {
        typename hft::OrderBook<double, int64_t, uint64_t>::Order order{
            .id = ++order_id,
            .price = 100.0,
            .quantity = 100,
            .is_buy = (order_id % 2 == 0),
            .timestamp = hft::utils::current_time()
        };
        engine.handle_order(order);
    }
}
BENCHMARK(BM_OrderMatching);

BENCHMARK_MAIN(); 