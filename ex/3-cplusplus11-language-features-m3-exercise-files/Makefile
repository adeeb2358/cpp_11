FLAGS = -std=c++11 -Wall -Ibandit
TEST_FLAGS = -Wno-unused-variable -Wno-unused-but-set-variable -I.

.PHONY: all move-semantics-tests perfect-forwarding-tests constexpr-tests

all: precompile move-semantics-tests perfect-forwarding-tests constexpr-tests

precompile:
	g++ $(FLAGS) precompiled.h

move-semantics-tests:
	g++ $(FLAGS) $(TEST_FLAGS) -o run-$@ $@/test-suite.cpp

perfect-forwarding-tests:
	g++ $(FLAGS) $(TEST_FLAGS) -o run-$@ $@/test-suite.cpp

constexpr-tests:
	g++ $(FLAGS) $(TEST_FLAGS) -o run-$@ $@/test-suite.cpp

clean:
	rm run-*-tests
	rm precompiled.h.gch