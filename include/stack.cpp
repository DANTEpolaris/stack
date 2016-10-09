#ifndef stack_cpp
#define stack_cpp
#pragma once
#include <iostream>
template<typename T>
auto newcopy( const T * item, size_t size, size_t count) -> T* //strong
{
	T * buff = new T[size];
	try
	{
		std::copy(item, item + count, buff);
	}
	catch(...)
	{
		delete[] buff; 
		throw;
	}
		return buff;	
}

template <typename T>
class allocator
{
protected:
    allocator(size_t size = 0);
    ~allocator();
    auto swap(allocator & other) -> void;
    
    allocator(allocator const &) = delete;
    auto operator =(allocator const &) -> allocator & = delete;
    
    T * ptr_;
    size_t size_;
    size_t count_;
};

template<typename T>
allocator<T>::allocator(size_t size) :
	ptr_(static_cast<T *>(size == 0 ? nullptr : operator new(size * sizeof(T)))),
	size_(0),
	count_(size) {
}

template<typename T>
allocator<T>::~allocator() {
	delete ptr_;
}

template<typename T>
auto allocator<T>::swap(allocator & other) -> void {
	std::swap(ptr_, other.ptr_);
	std::swap(count_, other.count_);
	std::swap(size_, other.size_);
}


template <typename T>
class stack : protected allocator<T>
{
public:
	stack(); //noexcept
	stack(stack const &);  //strong
	~stack(); //noexcept
	size_t count() const;  //noexcept
	auto push(T const &) -> void; //strong
	void pop(); //strong
	const T& top(); //strong
	auto operator=(stack const & right)->stack &; //strong
	auto empty() const -> bool; //noexcept

};
template <typename T>
size_t stack<T>::count() const
{
	return allocator<T>::count_;
}
template <typename T>
stack<T>::stack():allocator(){}
{

}


template <typename T>
stack<T>::stack(const stack& item) : allocator<T>(item.size_){
	allocator<T>::ptr_ = newcopy(item.allocator<T>::ptr_, item.allocator<T>::count_, item.allocator<T>::size_);
	allocator<T>::count_ = item.allocator<T>::count_;
};

template <typename T>
stack<T>::~stack()
{
}

template<typename T>
auto stack<T>::push(T const & item) -> void {
	if (allocator<T>::count_ == allocator<T>::size_) {
		T * buff = newcopy(allocator<T>::ptr_, allocator<T>::count_,  allocator<T>::size_ = allocator<T>::size_ * 2 + (allocator<T>::count_ == 0));
		delete allocator<T>::ptr_;
		allocator<T>::ptr_ = buff;
		allocator<T>::size_ = allocator<T>::size_ * 2 + (allocator<T>::count_ == 0) ;
	}
	allocator<T>::ptr_[allocator<T>::count_] = item;
	++allocator<T>::count_;
}

template<typename T>
void stack<T>::pop() {
	if (allocator::count_ == 0) {
		throw std::logic_error("Stack is empty!");
	} else {
		allocator::count_--;
	}
}


template<typename T>
const T& stack<T>::top()
{
	if (allocator::count_ == 0) {
		throw ("Stack is empty!");
	}
	return allocator::ptr_[allocator::count_ - 1];
}

template<typename T>
auto stack<T>::operator=(stack const & right) -> stack & {
	if (this != &right) {
	T* buff = newcopy(right.allocator<T>::ptr_, right.allocator<T>::size_, right.allocator<T>::count_);
	delete[] allocator<T>::ptr_;
	allocator<T>::ptr = buff;
	allocator<T>::count_ = right.allocator<T>::count_;
	allocator<T>::size_ = right.allocator<T>::size_;
	}
	return *this;
}

template<typename T>
auto stack<T>::empty() const -> bool{
	if (allocator::count_ == 0){
		return true;
	} else{
		return false;
	}
}

#endif
