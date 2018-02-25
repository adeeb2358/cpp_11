g++ -std=c++11 -Wall -Ibandit precompiled.h
g++ -std=c++11 -Wall -Wno-unused-variable -Wno-unused-but-set-variable -I. -Ibandit -o run-template-tests template-tests\test-suite.cpp
g++ -std=c++11 -Wall -Wno-unused-variable -Wno-unused-but-set-variable -I. -Ibandit -o run-class-tests class-tests\test-suite.cpp
g++ -std=c++11 -Wall -Wno-unused-variable -Wno-unused-but-set-variable -I. -Ibandit -o run-initialization-tests initialization-tests\test-suite.cpp
