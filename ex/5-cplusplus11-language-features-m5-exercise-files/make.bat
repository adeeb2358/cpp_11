g++ -std=c++11 -Wall -Ibandit precompiled.h
g++ -std=c++11 -Wall -Wno-unused-variable -Wno-unused-but-set-variable -I. -Ibandit -o run-alignment-tests alignment-tests\test-suite.cpp
g++ -std=c++11 -Wall -Wno-unused-variable -Wno-unused-but-set-variable -I. -Ibandit -o run-attribute-tests attribute-tests\test-suite.cpp
g++ -std=c++11 -Wall -Wno-unused-variable -Wno-unused-but-set-variable -I. -Ibandit -o run-explicit-operators-tests explicit-operators-tests\test-suite.cpp
g++ -std=c++11 -Wall -Wno-unused-variable -Wno-unused-but-set-variable -I. -Ibandit -o run-inline-namespace-tests inline-namespace-tests\test-suite.cpp
g++ -std=c++11 -Wall -Wno-unused-variable -Wno-unused-but-set-variable -I. -Ibandit -o run-sizeof-tests sizeof-tests\test-suite.cpp
g++ -std=c++11 -Wall -Wno-unused-variable -Wno-unused-but-set-variable -I. -Ibandit -o run-thread-local-tests thread-local-tests\test-suite.cpp
g++ -std=c++11 -Wall -Wno-unused-variable -Wno-unused-but-set-variable -I. -Ibandit -o run-union-tests union-tests\test-suite.cpp
