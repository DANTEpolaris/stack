#include "stdafx.h"
#include <stdlib.h>
#include <iostream>
using namespace std;
template <typename T>
class stack
{
public:
	stack();
	stack(stack const &);
	~stack();
	size_t count() const;
	auto push(T const &) -> void;
	T pop();
	auto operator=(stack const & right)->stack &;
private:
	T * array_;
	size_t array_size_;
	size_t count_;
};

int main()
{

}

template <typename T>
size_t stack<T>::count() const
{
	return count_;
}
template <typename T>
stack<T>::stack()
{
	array_size_ = 0;
	array_ = new T[array_size_];
	count_ = 0;
}

template<typename T>
stack<T>::stack(stack const & item)
{
	array_size_ = item.array_size_;
	count_ = item.count_;
	T * buff = new T[array_size_];
	copy(array_, array_ + array_size_, buff);
	delete[] array_;
	array_ = buff;
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
		T * buff = new T[size];
		copy(array_, array_ + array_size_, buff);
		delete[] array_;
		array_ = buff;
		array_size_ = size;
	}
	++count_;
	array_[count_ - 1] = item;
}

template<typename T>
T stack<T>::pop() {
	if (count_ == 0) {
		throw std::logic_error("Stack is empty!");
	}
	return array_[--count_];
}
template<typename T>
auto stack<T>::operator=(stack const & right) -> stack & {
	if (this != &right) {
	delete[] array_;
	count_ = right.count_;
	array_size_ = right.array_size_;
	array_ = new T[array_size_];
	copy(right.array_, right.array_ + count_, array_);	}
	return *this;
}
