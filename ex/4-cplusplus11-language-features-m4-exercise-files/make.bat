g++ -std=c++11 -Wall -Ibandit precompiled.h
g++ -std=c++11 -Wall -Wno-unused-variable -Wno-unused-but-set-variable -I. -Ibandit -o run-enum-tests enum-tests\test-suite.cpp
g++ -std=c++11 -Wall -Wno-unused-variable -Wno-unused-but-set-variable -I. -Ibandit -o run-for-loop-tests for-loop-tests\test-suite.cpp
g++ -std=c++11 -Wall -Wno-unused-variable -Wno-unused-but-set-variable -I. -Ibandit -o run-literal-tests literal-tests\test-suite.cpp
g++ -std=c++11 -Wall -Wno-unused-variable -Wno-unused-but-set-variable -I. -Ibandit -o run-noexcept-tests noexcept-tests\test-suite.cpp
g++ -std=c++11 -Wall -Wno-unused-variable -Wno-unused-but-set-variable -I. -Ibandit -o run-nullptr-tests nullptr-tests\test-suite.cpp
g++ -std=c++11 -Wall -Wno-unused-variable -Wno-unused-but-set-variable -I. -Ibandit -o run-static-assert-tests static-assert-tests\test-suite.cpp
