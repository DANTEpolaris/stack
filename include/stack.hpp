#ifndef stack_cpp
#define stack_cpp
#pragma once
#include <iostream>

template <typename T1, typename T2>
void construct(T1 * ptr, T2 const & value) {
    new(ptr) T1 (value);
}

template <typename T>
void destroy(T * ptr) noexcept
{
    ptr->~T();
}

template <typename FwdIter>
void destroy(FwdIter first, FwdIter last) noexcept
{
    for (; first != last; ++first) {
        destroy(&*first);
    }
}




template <typename T>
class allocator
{
public:
    explicit
    allocator( std::size_t size = 0 ) /*strong*/;
    allocator( allocator const & other ) /*strong*/;
    auto operator =( allocator const & other ) -> allocator & = delete;
    ~allocator();
    
    auto resize() /*strong*/ -> void;
    
    auto construct(T * ptr, T const & value ) /*strong*/ -> void;
    auto destroy( T * ptr ) /*noexcept*/ -> void;
    
    auto get() /*noexcept*/ -> T *;
    auto get() const /*noexcept*/ -> T const *;
    
    auto count() const /*noexcept*/ -> size_t;
    auto full() const /*noexcept*/ -> bool;
    auto empty() const /*noexcept*/ -> bool;
private:
    auto destroy( T * first, T * last ) /*noexcept*/ -> void;
    auto swap( allocator & other ) /*noexcept*/ -> void;

    
    T * ptr_;
    size_t size_;
    std::unique_ptr<bitset> map_;
};

template <typename T>
class stack
{
public:
    explicit
    stack( size_t size = 0 );
    auto operator =( stack const & other ) /*strong*/ -> stack &;
    
    auto empty() const /*noexcept*/ -> bool;
    auto count() const /*noexcept*/ -> size_t;
    
    auto push( T const & value ) /*strong*/ -> void;
    auto pop() /*strong*/ -> void;
    auto top() /*strong*/ -> T &;
    auto top() const /*strong*/ -> T const &;
    
private:
    allocator<T> allocator_;
    
    auto throw_is_empty() const -> void;
};

template <typename T>
allocator<T>::allocator(size_t size) : ptr_((T*)(operator new(size*sizeof(T)))), size_(size), count_(0){};

template<typename T>
allocator<T>::~allocator(){ operator delete(ptr_); }

template<typename T>
auto allocator<T>::swap(allocator & other) -> void {
	std::swap(ptr_, other.ptr_);
	std::swap(count_, other.count_);
	std::swap(size_, other.size_);
}

template <typename T>
size_t stack<T>::count() const
{
	return allocator<T>::count_;
}
template <typename T>
stack<T>::stack(size_t size):allocator<T>(size){}

template <typename T>
stack<T>::stack(const stack& item) : allocator<T>(item.size_){
	for (size_t i = 0; i < item.count_; i++) construct(allocator<T>::ptr_ + i, item.ptr_[i]);
	allocator<T>::count_ = item.count_;
};

template <typename T>
stack<T>::~stack()
{
destroy(allocator<T>::ptr_, allocator<T>::ptr_ + allocator<T>::count_);
}


template <typename T>
void stack<T>::push(T const &item){
	if (allocator<T>::count_ == allocator<T>::size_) {
		size_t array_size = allocator<T>::size_ * 2 + (allocator<T>::size_ == 0);
		stack<T> temp(array_size);
		while (temp.count() < allocator<T>::count_) temp.push(allocator<T>::ptr_[temp.count()]); 
		this->swap(temp);
	}
	construct(allocator<T>::ptr_ + allocator<T>::count_, item);
	++allocator<T>::count_;
}

template<typename T>

void stack<T>::pop()
{
	if (allocator<T>::count_> 0) {
		--allocator<T>::count_;
	}
	else throw ("Stack is empty");
}
template<typename T>
const T& stack<T>::top()
{
	if (allocator<T>::count_ == 0) {
		throw ("Stack is empty!");
	}
	return allocator<T>::ptr_[allocator<T>::count_ - 1];
}

template<typename T>
auto stack<T>::operator=(stack const & right) -> stack & {
	if (this != &right) {
	stack<T> temp (right.size_);
	while (temp.count_ < right.count_){
		construct(temp.ptr_ + temp.count_, right.ptr_[temp.count_]);
		++temp.count_;
	}	
	this -> swap(temp);
	}
	return *this;
}


template<typename T>
auto stack<T>::empty() const -> bool{
	if (allocator<T>::count_ == 0){
		return true;
	} else{
		return false;
	}
}

#endif
