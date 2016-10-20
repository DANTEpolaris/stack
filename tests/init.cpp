#include "stack.hpp"
#include <catch.hpp>

SCENARIO("push") {
	stack<size_t> a;
	a.push(1);
	REQUIRE(a.count() == 1);
}
