#ifndef stack_cpp
#define stack_cpp
#pragma once
#include <iostream>
#include <memory>

class bitset
{
public:
	explicit
		bitset(size_t size) /*strong*/;

	bitset(bitset const & other) = delete;
	auto operator =(bitset const & other)->bitset & = delete;

	bitset(bitset && other) = delete;
	auto operator =(bitset && other)->bitset & = delete;

	auto set(size_t index) /*strong*/ -> void;
	auto reset(size_t index) /*strong*/ -> void;
	auto test(size_t index) /*strong*/ -> bool;

	auto size() /*noexcept*/ -> size_t;
	auto counter() /*noexcept*/ -> size_t;

private:
	std::unique_ptr<bool[]>  ptr_;
	size_t size_;
	size_t counter_;
};

bitset::bitset(size_t size) : ptr_(std::make_unique<bool[]>(size)), size_(size), counter_(0) {}

auto bitset::set(size_t index) -> void
{ if (index >= 0 && index < size_) {
	ptr_[index] = true; 
	++counter_; }
	else throw; 
}

auto bitset::reset(size_t index) -> void
{	if (index >= 0 && index < size_)
	{
		ptr_[index] = false;
		--counter_;
	}
else throw;
}

auto bitset::test(size_t index) -> bool
{
	if (index >= 0 && index < size_)
	{
		return !ptr_[index];
	}
	else throw;
	
}
auto bitset::size() -> size_t
{
	return size_;
}

auto bitset::counter() -> size_t
{
	return counter_;
}
	

template <typename T>
class allocator
{
public:
	explicit
		allocator(std::size_t size = 0) /*strong*/;
	allocator(allocator const & other) /*strong*/;
	auto operator =(allocator const & other)->allocator & = delete;
	~allocator();

	auto resize() /*strong*/ -> void;

	auto construct(T * ptr, T const & value) /*strong*/ -> void;
	auto destroy(T * ptr) /*noexcept*/ -> void;

	auto get() /*noexcept*/ -> T *;
	auto get() const /*noexcept*/ -> T const *;

	auto count() const /*noexcept*/ -> size_t;
	auto full() const /*noexcept*/ -> bool;
	auto empty() const /*noexcept*/ -> bool;
private:
	auto destroy(T * first, T * last) /*noexcept*/ -> void;
	auto swap(allocator & other) /*noexcept*/ -> void;


	T * ptr_;
	size_t size_;
	std::unique_ptr<bitset> map_;
};

template <typename T>
class stack
{
public:
	explicit
		stack(size_t size = 0);
	auto operator =(stack const & other) /*strong*/ -> stack &;

	auto empty() const /*noexcept*/ -> bool;
	auto count() const /*noexcept*/ -> size_t;

	auto push(T const & value) /*strong*/ -> void;
	auto pop() /*strong*/ -> void;
	auto top() /*strong*/ -> T &;
	auto top() const /*strong*/ -> T const &;

private:
	allocator<T> allocator_;

	auto throw_is_empty() const -> void;
};

template <typename T>
allocator<T>::allocator(size_t size) : ptr_((T*)(operator new(size*sizeof(T)))), size_(size), map_(std::make_unique<bitset>(size)) {};

template<typename T>
inline allocator<T>::allocator(allocator const & other) :
	ptr_((T *)(other.size_ == 0 ? nullptr : operator new(other.size_ * sizeof(T)))),
	size_(other.size_),
	map_(other.map_) {
	for (size_t i = 0; i < other.count_; ++i) {
		if (map_.test(i)) {
			this->construct(this->ptr_ + i, other.ptr_[i]);
		}
	}
}
template<typename T>
allocator<T>::~allocator() { operator delete(ptr_); }

template<typename T>
auto allocator<T>::resize() -> void
{
	allocator<T> buff(size_ * 2 + (size_ == 0));
	for (size_t i = 0; i < size_; i++) construct(buff.ptr_ + i, ptr_[i]);
	this->swap(buff);
}

template<typename T>
auto allocator<T>::construct(T * ptr, T const & value)->void {
	if (ptr >= ptr_&&ptr < ptr_ + size_&&map_->test(ptr - ptr_)) {
		new(ptr)T(value);
		map_->set(ptr - ptr_);
	}
	else throw("error");
}

template<typename T>
auto allocator<T>::destroy(T * ptr) -> void {
	ptr->~T();
	map_->reset(ptr-ptr_);
}


template<typename T>
auto allocator<T>::destroy(T * first, T * last) -> void
{
	for (; first != last; ++first) {
		destroy(&*first);
	}
}

template<typename T>
auto allocator<T>::get()-> T* {
	return ptr_; 
}

template<typename T>
auto allocator<T>::get() const -> T const * { 
	return ptr_; 
}

template<typename T>
auto allocator<T>::count() const -> size_t
{
	return map_->counter_;
}

template<typename T>
auto allocator<T>::full() const -> bool
{
	return map_->counter_==size_;
}

template<typename T>
auto allocator<T>::empty() const -> bool
{
	return map_->counter_==0;
}

template<typename T>
auto allocator<T>::swap(allocator & other) -> void {
	std::swap(ptr_, other.ptr_);
	std::swap(map_, other.map_);
	std::swap(size_, other.size_);
}

template <typename T>
size_t stack<T>::count() const
{
	return allocator_.count_;
}
template <typename T>
stack<T>::stack(size_t size) :allocator<T>(size) {}

template <typename T>
void stack<T>::push(T const &item) {
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
	else throw_is_empty();
}
template<typename T>
const T& stack<T>::top()
{
	if (allocator<T>::count_ == 0) {
		throw_is_empty();
	}
	return allocator<T>::ptr_[allocator<T>::count_ - 1];
}

template<typename T>
auto stack<T>::top() const -> T const &
{
	if (allocator<T>::count_ == 0) {
		throw_is_empty();
	}
	return allocator<T>::ptr_[allocator<T>::count_ - 1];
}

template<typename T>
auto stack<T>::throw_is_empty() const -> void
{
		throw("Stack is empty!");
}

template<typename T>
auto stack<T>::operator=(stack const & right) -> stack & {
	if (this != &right) {
		stack<T> temp(right.size_);
		while (temp.count_ < right.count_) {
			construct(temp.ptr_ + temp.count_, right.ptr_[temp.count_]);
			++temp.count_;
		}
		this->swap(temp);
	}
	return *this;
}


template<typename T>
auto stack<T>::empty() const -> bool {
	return allocator_.empty;

#endif
