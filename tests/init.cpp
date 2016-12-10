#include <stack.hpp>
#include <catch.hpp>
#include <thread>
#include <iostream>
using namespace std;

SCENARIO("count", "[count]"){
  stack<int> s;
  s.push(1);
  REQUIRE(s.count()==1);
}

SCENARIO("push", "[push]"){
  stack<int> s;
  s.push(1);
  REQUIRE(s.count()==1);
}

SCENARIO("pop", "[pop]"){
  stack<int> s;
  s.push(1);
  REQUIRE(*(s.pop())==1);	
  REQUIRE(s.count()==0);
}

SCENARIO("prisv", "[prisv]"){
  stack<int> s;
  s.push(1);
  stack<int> s2;
  s2=s;
  REQUIRE(s.count()==1);
}

SCENARIO("empty", "[empty]"){
  stack<int> s1, s2;
  s1.push(1);
  REQUIRE(!s1.empty());
  REQUIRE(s2.empty());
}
SCENARIO("Stack: top and pop threads", "threads") {
	stack<size_t> st;
	st.push(0);
	std::thread t1(&stack<size_t>::push, &st, 1);
	t1.join();
	REQUIRE(st.count() == 2);
	REQUIRE(*st.pop() == 1);
	REQUIRE(st.empty() == false);
	std::thread t2(&stack<size_t>::pop, &st);
	t2.join();
	REQUIRE(st.count() == 0);
	REQUIRE(st.empty());
}
