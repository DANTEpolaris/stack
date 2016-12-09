#include "stack.hpp"
#include <catch.hpp>
#include <iostream>
#include <thread>
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
  REQUIRE(s.top()==1);
}

SCENARIO("pop", "[pop]"){
  stack<int> s;
  s.push(1); s.pop();
  REQUIRE(s.count()==0);
}

SCENARIO("cop", "[cop]"){
  stack<int> s;
  s.push(1);
  stack<int> s2=s;
  REQUIRE(s.count()==1);
  REQUIRE(s.top()==1);
}

SCENARIO("top", "[top]"){
  stack<int> s;
  s.push(1);
  REQUIRE(s.top()==1);
}

SCENARIO("empty", "[empty]"){
  stack<int> s1, s2;
  s1.push(1);
  REQUIRE(!s1.empty());
  REQUIRE(s2.empty());
}
SCENARIO("threads", "[threads]"){
  stack<int> s;
  std::thread t1(&stack::push, s, 1);
  std::thread t2(&stack::pop, s);
  t1.join();
  t2.join();
  REQUIRE(s.count()==0);
}

