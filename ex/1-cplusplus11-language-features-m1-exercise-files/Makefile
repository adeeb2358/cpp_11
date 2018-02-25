FLAGS = -std=c++11 -Wall -Ibandit
TEST_FLAGS = -Wno-unused-variable -Wno-unused-but-set-variable -I.

.PHONY: all lambda-tests type-inference-tests

all: precompile lambda-tests type-inference-tests

precompile:
	g++ $(FLAGS) precompiled.h

lambda-tests:
	g++ $(FLAGS) $(TEST_FLAGS) -o run-$@ $@/test-suite.cpp

type-inference-tests:
	g++ $(FLAGS) $(TEST_FLAGS) -o run-$@ $@/test-suite.cpp


clean:
	rm run-*-tests
	rm precompiled.h.gch