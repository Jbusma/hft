# HFT Trading System

A modern C++20 implementation of a high-frequency trading system featuring a matching engine, order book management, and market data processing.

## Features

- Low-latency matching engine with price-time priority
- Lock-free order book implementation
- Template-based design with C++20 concepts
- Comprehensive test suite using Boost.Test
- Performance benchmarking with Google Benchmark

## Requirements

### macOS
```bash
# Install dependencies using Homebrew
brew install cmake boost tbb google-benchmark
```

### Linux (Ubuntu/Debian)
```bash
sudo apt-get install build-essential cmake libboost-all-dev libtbb-dev libbenchmark-dev
```

## Building & Testing

Always clean build after changes:
```bash
# Clean and rebuild
rm -rf build
mkdir build && cd build
cmake ..
make

# Run tests with detailed output
ctest --output-on-failure
```

Run specific test suites:
```bash
./tests/hft-tests --run_test=OrderBookTests
./tests/hft-tests --run_test=MatchingEngineTests
```

## Project Structure

```
hft-trading-system/
├── include/                 # Header files (.hpp)
│   ├── Concepts.hpp        # Type constraints
│   ├── MatchingEngine.hpp  # Order matching
│   ├── OrderBook.hpp       # Order management
│   ├── MarketDataFeed.hpp  # Market data handling
│   └── Utils.hpp           # Utilities
├── src/                    # Source files (.cpp)
├── tests/                  # Test suite
└── build/                  # Build artifacts (not in git)
```

## Example Usage

```cpp
#include "MatchingEngine.hpp"
#include "OrderBook.hpp"
#include "Utils.hpp"

// Initialize matching engine
hft::MatchingEngine<double, int64_t, uint64_t> engine;

// Set up fill callback
engine.set_fill_callback([](const uint64_t& id, double price, int64_t quantity) {
    std::cout << "Order " << id << " filled: " << quantity << " @ " << price << std::endl;
});

// Create and submit an order
typename hft::OrderBook<double, int64_t, uint64_t>::Order order{
    .id = hft::utils::generate_order_id(),
    .price = 100.50,
    .quantity = 100,
    .is_buy = true,
    .timestamp = hft::utils::current_time()
};

engine.handle_order(order);
```

## Contributing

1. Fork the repository
2. Create your feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add some amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

## License

MIT License - see the [LICENSE](LICENSE) file for details.
