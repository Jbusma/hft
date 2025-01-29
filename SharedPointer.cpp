#include <iostream>
#include <cstddef>
#include <cassert>

template<typename T>
class SharedPtr {
private:
    T* ptr_;
    size_t* ref_count_;

public:
    explicit SharedPtr(T* ptr = nullptr) : ptr_(ptr), ref_count_(ptr ? new size_t(1) : nullptr) {}

    SharedPtr(const SharedPtr& other) : ptr_(other.ptr_), ref_count_(other.ref_count_) {
        if (ref_count_) {
            ++(*ref_count_);
        }
    }

    SharedPtr& operator=(const SharedPtr& other) {
        if (this != &other) {
            release();
            ptr_ = other.ptr_;
            ref_count_ = other.ref_count_;
            if (ref_count_) {
                ++(*ref_count_);
            }
        }
        return *this;
    }

    ~SharedPtr() { release(); }

    T& operator*() const { return *ptr_; }
    T* operator->() const { return ptr_; }
    T* get() const { return ptr_; }
    size_t use_count() const { return ref_count_ ? *ref_count_ : 0; }

    void reset(T* ptr = nullptr) {
        release();
        ptr_ = ptr;
        ref_count_ = ptr ? new size_t(1) : nullptr;
    }

private:
    void release() {
        if (ref_count_) {
            --(*ref_count_);
            if (*ref_count_ == 0) {
                delete ptr_;
                delete ref_count_;
            }
        }
        ptr_ = nullptr;
        ref_count_ = nullptr;
    }
};

// Test code
int main() {
    // Test basic usage
    SharedPtr<int> ptr1(new int(42));
    assert(*ptr1 == 42);
    assert(ptr1.use_count() == 1);

    // Test sharing
    SharedPtr<int> ptr2 = ptr1;
    assert(ptr1.use_count() == 2);
    assert(ptr2.use_count() == 2);
    assert(*ptr2 == 42);

    // Test reset
    ptr1.reset(new int(24));
    assert(*ptr1 == 24);
    assert(ptr1.use_count() == 1);
    assert(ptr2.use_count() == 1);

    std::cout << "All tests passed!" << std::endl;
    return 0;
} 