FLAGS = -std=c++11 -Wall -Ibandit
TEST_FLAGS = -Wno-unused-variable -Wno-unused-but-set-variable -I.

.PHONY: all template-tests class-tests initialization-tests

all: precompile template-tests class-tests initialization-tests

precompile:
	g++ $(FLAGS) precompiled.h

template-tests:
	g++ $(FLAGS) $(TEST_FLAGS) -o run-$@ $@/test-suite.cpp

class-tests:
	g++ $(FLAGS) $(TEST_FLAGS) -o run-$@ $@/test-suite.cpp

initialization-tests:
	g++ $(FLAGS) $(TEST_FLAGS) -o run-$@ $@/test-suite.cpp

clean:
	rm run-*-tests
	rm precompiled.h.gch