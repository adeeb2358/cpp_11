FLAGS = -std=c++11 -Wall -Ibandit
TEST_FLAGS = -Wno-unused-variable -Wno-unused-but-set-variable -I.

.PHONY: all enum-tests for-loop-tests literal-tests noexcept-tests nullptr-tests static-assert-tests

all: precompile enum-tests for-loop-tests literal-tests noexcept-tests nullptr-tests static-assert-tests

precompile:
	g++ $(FLAGS) precompiled.h

enum-tests:
	g++ $(FLAGS) $(TEST_FLAGS) -o run-$@ $@/test-suite.cpp

for-loop-tests:
	g++ $(FLAGS) $(TEST_FLAGS) -o run-$@ $@/test-suite.cpp

literal-tests:
	g++ $(FLAGS) $(TEST_FLAGS) -o run-$@ $@/test-suite.cpp

noexcept-tests:
	g++ $(FLAGS) $(TEST_FLAGS) -o run-$@ $@/test-suite.cpp

nullptr-tests:
	g++ $(FLAGS) $(TEST_FLAGS) -o run-$@ $@/test-suite.cpp

static-assert-tests:
	g++ $(FLAGS) $(TEST_FLAGS) -o run-$@ $@/test-suite.cpp

clean:
	rm run-*-tests
	rm precompiled.h.gch