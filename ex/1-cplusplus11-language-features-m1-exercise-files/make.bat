g++ -std=c++11 -Wall -Ibandit precompiled.h
g++ -std=c++11 -Wall -Wno-unused-variable -Wno-unused-but-set-variable -I. -Ibandit -o run-type-inference-tests type-inference-tests\test-suite.cpp
g++ -std=c++11 -Wall -Wno-unused-variable -Wno-unused-but-set-variable -I. -Ibandit -o run-lambda-tests lambda-tests\test-suite.cpp
