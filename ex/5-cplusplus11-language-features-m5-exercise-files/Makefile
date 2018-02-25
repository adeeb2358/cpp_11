FLAGS = -std=c++11 -Wall -Ibandit
TEST_FLAGS = -Wno-unused-variable -Wno-unused-but-set-variable -I.

.PHONY: all alignment-tests attribute-tests explicit-operators-tests inline-namespace-tests sizeof-tests thread-local-tests union-tests

all: precompile alignment-tests attribute-tests explicit-operators-tests inline-namespace-tests sizeof-tests thread-local-tests union-tests

precompile:
	g++ $(FLAGS) precompiled.h

alignment-tests:
	g++ $(FLAGS) $(TEST_FLAGS) -o run-$@ $@/test-suite.cpp

attribute-tests:
	g++ $(FLAGS) $(TEST_FLAGS) -o run-$@ $@/test-suite.cpp

explicit-operators-tests:
	g++ $(FLAGS) $(TEST_FLAGS) -o run-$@ $@/test-suite.cpp

inline-namespace-tests:
	g++ $(FLAGS) $(TEST_FLAGS) -o run-$@ $@/test-suite.cpp

sizeof-tests:
	g++ $(FLAGS) $(TEST_FLAGS) -o run-$@ $@/test-suite.cpp

thread-local-tests:
	g++ $(FLAGS) $(TEST_FLAGS) -o run-$@ $@/test-suite.cpp

union-tests:
	g++ $(FLAGS) $(TEST_FLAGS) -o run-$@ $@/test-suite.cpp

clean:
	rm run-*-tests
	rm precompiled.h.gch