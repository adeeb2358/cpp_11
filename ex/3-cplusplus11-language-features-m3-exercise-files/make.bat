g++ -std=c++11 -Wall -Ibandit precompiled.h
g++ -std=c++11 -Wall -Wno-unused-variable -Wno-unused-but-set-variable -I. -Ibandit -o run-move-semantics-tests move-semantics-tests\test-suite.cpp
g++ -std=c++11 -Wall -Wno-unused-variable -Wno-unused-but-set-variable -I. -Ibandit -o run-perfect-forwarding-tests perfect-forwarding-tests\test-suite.cpp
g++ -std=c++11 -Wall -Wno-unused-variable -Wno-unused-but-set-variable -I. -Ibandit -o run-constexpr-tests constexpr-tests\test-suite.cpp
