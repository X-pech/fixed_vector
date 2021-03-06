//
// Created by xpech on 06.10.18.
//

#ifndef FIXED_VECTOR_FIXED_VECTOR_H
#define FIXED_VECTOR_FIXED_VECTOR_H

#include <iostream>
#include <type_traits>

template <typename T, size_t S>
class fixed_vector {
private:
    typedef T* iterator;
    typedef const T* const_iterator;
    typedef std::reverse_iterator<iterator> reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
    typename std::aligned_storage<sizeof(T), alignof(T)>::type data_[S];
public:
    fixed_vector() = default;
    fixed_vector(fixed_vector const& rhs) : size_(rhs.size_) {
        reinit_cpy_data(reinterpret_cast<const T*>(rhs.data_));
    }

    fixed_vector & operator=(fixed_vector const& rhs) {
        destruct_data();
        size_ = rhs.size_;
        reinit_cpy_data(reinterpret_cast<const T*>(rhs.data_));
        return *this;
    }

    ~fixed_vector() {
        destruct_data();
    }

    T& operator[](size_t index) {
        return get(index);
    }

    T const& operator[](size_t index) const {
        return get(index);
    }

    size_t size() const {
        return size_;
    }

    size_t capacity() const {
        return capacity_;
    }

    bool empty() const {
        return (size_ == 0);
    }
    void push_back(T const& val) {
        if (size_ == capacity_) {
            throw std::length_error("fixed_vector is on full capacity!");
        }
        new (reinterpret_cast<T*>(data_ + size_)) T(val);
        size_++;
    }

    void pop_back() {
        if (size_ == 0) {
            throw std::length_error("Attempted to use pop_back on empty fixed_vector");
        }
        size_--;
        reinterpret_cast<T*>(data_ + size_)->~T();
    }

    T& back() {
        return get(size_ - 1);
    }

    T const& back() const {
        return get(size_ - 1);
    }

    T& front() {
        return get(0);
    }

    T const& front() const {
        return get(size_ - 1);
    }

    void swap(fixed_vector &v) {
        swap(reinterpret_cast<T*>(data_), reinterpret_cast<T*>(v.data_));
        swap(size_, v.size_);
    }

    void clear() {
        destruct_data();
        size_ = 0;
    }

    iterator begin() {
        return reinterpret_cast<T*>(data_);
    }

    const_iterator begin() const {
        return reinterpret_cast<const T*>(data_);
    }

    iterator end() {
        return reinterpret_cast<T*>(data_) + size_;
    }

    const_iterator end() const {
        return reinterpret_cast<const T*>(data_) + size_;
    }

    reverse_iterator rbegin() {
        return reverse_iterator(reinterpret_cast<T*>(data_) + size_);
    }

    const_reverse_iterator rbegin() const {
        return const_reverse_iterator(reinterpret_cast<const T*>(data_) + size_);
    }

    reverse_iterator rend() {
        return reverse_iterator(reinterpret_cast<T*>(data_));
    }

    const_reverse_iterator rend() const {
        return const_reverse_iterator(reinterpret_cast<const T*>(data_));
    }

    iterator erase(const_iterator ci) {
        auto index = static_cast<size_t>(ci - begin());
        bcot(index);
        (reinterpret_cast<T*>(data_ + index))->~T();
        // BUT IT LOOKS NOT OKAY LOL
        //std::copy((void*)(reinterpret_cast<T*>(data_) + index + 1), (void*)(end()), (void*)(reinterpret_cast<T*>(data_) + index));
        for (size_t i = index; i < size_ - 1; i++) {
            new (reinterpret_cast<T*>(data_ + i)) T(*reinterpret_cast<T*>(data_ + i + 1));
            reinterpret_cast<T*>(data_ + i + 1)->~T();
        }

        size_--;
        return iterator(ci);
    }

    iterator erase(const_iterator cb, const_iterator ce) {
        auto index_b = static_cast<size_t>(cb - begin());
        auto len = static_cast<size_t>(ce - cb);
        bcot(index_b);
        bcot(index_b + len - 1);

        for (size_t i = len; i < index_b + len; i++) {
            reinterpret_cast<T*>(data_ + i)->~T();
        }
        //std::copy((void*)(reinterpret_cast<T*>(data_) + index_b + len), (void*)(end()), (void*)(reinterpret_cast<T*>(data_) + index_b));
        for (size_t i = index_b; i < size_ - len; i++) {
            new (reinterpret_cast<T*>(data_) + i) T(*reinterpret_cast<T*>(data_ + i + len));
            reinterpret_cast<T*>(data_ + i + len)->~T();
        }

        size_ -= len;
        return iterator(cb);
    }

    iterator insert(const_iterator ci, T const &val) {
        if (size_ == capacity_) {
            throw std::length_error("Tried to insert element to full fixed_vector");
        }

        auto index = static_cast<size_t>(ci - begin());
        if (index != size_)
            bcot(index);

        for (size_t i = size_; i > index; i--) {
            new (reinterpret_cast<T*>(data_ + i)) T(*reinterpret_cast<T*>(data_ + i - 1));
            reinterpret_cast<T*>(data_ + i - 1)->~T();
        }

        new (reinterpret_cast<T*>(data_ + index)) T(val);

        size_++;
        return iterator(ci);
    }

private:

    void destruct_data() {
        for (size_t i = 0; i < size_; i++) {
            reinterpret_cast<T*>(data_ + i)->~T();
        }
    }

    void reinit_cpy_data(const T* rdata) {
        for (size_t i = 0; i < size_; i++) {
            new (reinterpret_cast<T*>(data_ + i)) T(rdata[i]);
        }
    }

    void bcot(const size_t &index) const {
        if (index < 0 || index >= size_) {
            throw std::out_of_range("Trying to access element of fixed_vector that is out of bound!");
        }
    }

    T& get(const size_t &index) {
        bcot(index);
        return *reinterpret_cast<T*>(data_ + index);
    }

    T const & get(const size_t &index) const {
        bcot(index);
        return *reinterpret_cast<T*>(data_ + index);
    }

    size_t size_ {};
    const size_t capacity_ = S;
    //T* reinterpret_cast<T*>(data_) = reinterpret_cast<T*>(operator new(S * sizeof(T)));
};

template <typename T, size_t S>
void swap(fixed_vector<T, S> &l, fixed_vector<T, S> &r) {
    l.swap(r);
}


#endif //FIXED_VECTOR_FIXED_VECTOR_H
