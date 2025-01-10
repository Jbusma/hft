#include "MarketDataFeed.hpp"
#include <thread>
#include <atomic>

namespace hft {

// Implementation of MarketDataFeed methods
MarketDataFeed::MarketDataFeed() : running_(false) {}

MarketDataFeed::~MarketDataFeed() {
    stop();
}

void MarketDataFeed::start() {
    running_ = true;
    worker_ = std::thread([this]() { run(); });
}

void MarketDataFeed::stop() {
    running_ = false;
    if (worker_.joinable()) {
        worker_.join();
    }
}

void MarketDataFeed::run() {
    while (running_) {
        process_next_message();
        std::this_thread::yield();
    }
}

void MarketDataFeed::process_next_message() {
    // Implementation for processing market data messages
}

} // namespace hft 