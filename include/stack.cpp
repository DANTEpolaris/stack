#ifndef stack_cpp
#define stack_cpp
#pragma once
#include <iostream>
using namespace std;
template<typename T>
auto newcopy( const T * item, size_t size, size_t count) -> T* //strong
{
	T * buff = new T[size];
	copy(item, item + count, buff);
	return buff;	
}
template <typename T>
class stack
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

private:
	T * array_;
	size_t array_size_;
	size_t count_;
};
template <typename T>
size_t stack<T>::count() const
{
	return count_;
}
template <typename T>
stack<T>::stack():array_ (nullptr), array_size_(0), count_(0)
{

}

template<typename T>
stack<T>::stack(stack const & item):array_size_(item.array_size_), count_(item.count_),array_(newcopy(item.array_, item.array_size_,item.count_))
{

}

template <typename T>
stack<T>::~stack()
{
	delete[] array_;
}

template<typename T>
auto stack<T>::push(T const & item) -> void {
	if (count_ == array_size_) {
		size_t size = array_size_ * 2 + (array_size_ == 0) ;
		T * buff = newcopy(array_, size, array_size_);
		delete[] array_;
		array_ = buff;
		array_size_ = size;
	}
	array_[count_] = item;
	++count_;
}


template<typename T>
void stack<T>::pop() {
	if (count_ == 0) {
		throw std::logic_error("Stack is empty!");
	} else {
		count_--;
	}
}


template<typename T>
const T& stack<T>::top(){
{
	if (count_ == 0) {
		throw ("Stack is empty!");
	}
	return array_[count_ - 1];
}

template<typename T>
auto stack<T>::operator=(stack const & right) -> stack & {
	if (this != &right) {
	T* buff = newcopy(right.array_, right.array_size_, right.count_);
	delete[] array_;
	array_ = buff;
	count_ = right.count_;
	array_size_ = right.array_size_;
	}
	return *this;
}

template<typename T>
auto stack<T>::empty() const -> bool{
	if (count_ == 0){
		return true;
	} else{
		return false;
	}
}

#endif
