#pragma once

#include "Concepts.hpp"
#include <atomic>
#include <thread>
#include <functional>
#include <queue>

namespace hft {

template<Price P, Quantity Q>
struct MarketUpdate {
    P price;
    Q quantity;
    bool is_buy;
    std::chrono::nanoseconds timestamp;
};

class MarketDataFeed {
public:
    template<Price P, Quantity Q>
    using UpdateCallback = std::function<void(const MarketUpdate<P, Q>&)>;
    
    MarketDataFeed();
    ~MarketDataFeed();

    template<Price P, Quantity Q>
    void subscribe(UpdateCallback<P, Q> callback);
    
    void start();
    void stop();

private:
    void run();
    void process_next_message();

    std::atomic<bool> running_;
    std::thread worker_;
    std::vector<std::function<void()>> callbacks_;
};

} // namespace hft 