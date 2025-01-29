#pragma once
#include <cstddef>  // For size_t
#include <atomic>

namespace hft {

template<typename T>
class SharedPtr {
private:
    alignas(64) T* ptr_;
    std::atomic<size_t>* ref_count_;

public:
    // Constructor
    explicit SharedPtr(T* ptr = nullptr) : ptr_(ptr), ref_count_(ptr ? new std::atomic<size_t>(1) : nullptr) {}

    // Copy constructor
    SharedPtr(const SharedPtr& other) : ptr_(other.ptr_), ref_count_(other.ref_count_) {
        if (ref_count_) {
            ref_count_->fetch_add(1, std::memory_order_relaxed);
        }
    }

    // Assignment operator
    SharedPtr& operator=(const SharedPtr& other) {
        if (this != &other) {
            release();
            ptr_ = other.ptr_;
            ref_count_ = other.ref_count_;
            if (ref_count_) {
                ref_count_->fetch_add(1, std::memory_order_relaxed);
            }
        }
        return *this;
    }

    // Destructor
    ~SharedPtr() {
        release();
    }

    // Access operators
    T& operator*() const { return *ptr_; }
    T* operator->() const { return ptr_; }

    // Utility functions
    T* get() const { return ptr_; }
    size_t use_count() const { return ref_count_ ? ref_count_->load() : 0; }

    void reset(T* ptr = nullptr) {
        release();
        ptr_ = ptr;
        ref_count_ = ptr ? new std::atomic<size_t>(1) : nullptr;
    }

private:
    void release() {
        if (ref_count_) {
            ref_count_->fetch_sub(1, std::memory_order_acq_rel);
            if (*ref_count_ == 0) {
                delete ptr_;
                delete ref_count_;
            }
        }
        ptr_ = nullptr;
        ref_count_ = nullptr;
    }
};

} // namespace hft 