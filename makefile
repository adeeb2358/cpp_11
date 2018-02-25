#variables for compilation
#sudo apt-get install mpich
#sudo apt-get install libboost-mpi-dev  
#-lboost_mpi -lboost_serialization as linker options

#CC                  = mpic++
CC 					= g++ 
FILE_EXTENSION 		= cpp
SRC_DIR             = ""
OBJ_DIR             = libs
MAIN_EXE            = final
MAIN_EXE_NAME  		= main 
MAIN_EXE_FILE		= $(MAIN_EXE)/main
LOG_FILE  			= output.txt
REDIRECT_COMMAND 	= 2>&1 | tee -a
SRC_FILES           = $(wildcard *.cpp)
OBJ_FILES           = $(patsubst %.cpp,%.o,$(SRC_FILES))
OBJ_FILES_ASM 		= $(patsubst %.cpp,%.s,$(SRC_FILES))
OBJ_FILES_WITH_PATH = $(patsubst %.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES))

#variables for binary directory creation
MKDIR_P             = mkdir -p
MAKE_OBJ_DIR        = if [ ! -d "$(OBJ_DIR)/" ]; then  $(MKDIR_P) $(OBJ_DIR); fi; 
MAKE_MAIN_EXE_DIR   = if [ ! -d "$(MAIN_EXE)/" ]; then $(MKDIR_P) $(MAIN_EXE); fi;

#variables for debugging
CCFLAGS             = -g -DEBUG -pthread -mavx -fopenmp  -lboost_mpi -lboost_serialization
#CCFLAGS             = -g -DEBUG -pthread   -lboost_mpi -lboost_serialization
#-msse3
CORE_FILE 			= core

ASMFLAGS 			= -S  -mavx -fopenmp 
ASM_DIR 			= asm
MAKE_ASM_DIR 		= if [ ! -d "$(ASM_DIR)/" ]; then $(MKDIR_P) $(ASM_DIR); fi;
ASM_FILES_WITH_PATH = $(patsubst %.cpp,$(ASM_DIR)/%.s,$(SRC_FILES))
#variables for git commiting
MAKE_FILE_PATH 		:= $(abspath $(lastword $(MAKEFILE_LIST)))
CURRENT_DIR 		:= $(notdir $(patsubst %/,%,$(dir $(MAKE_FILE_PATH))))

add:
	@ git init
	@ git remote add origin http://www.github.com/adeeb2358/$(CURRENT_DIR)
git:
	@ make clean
	@ git init
	@ git add .
	@ git commit -m"new updates"
	@ git push -f origin master

build-and-run: compile run

all:compile

compile:directory main

run1:
	@ mpirun -n 4  ./$(MAIN_EXE_FILE)

run:
	@ ulimit -c unlimited #generate core files in ubuntu
	@ terminator -e ./$(MAIN_EXE_FILE) 

directory:
	@ $(RM) $(LOG_FILE)
	@ touch $(LOG_FILE) 
	@ $(MAKE_MAIN_EXE_DIR)
	@ $(MAKE_OBJ_DIR) 
	@ $(MAKE_ASM_DIR)


main:build_objects build_main #build_asm
	
build_asm:$(OBJ_FILES_ASM)

$(OBJ_FILES_ASM):	
	@ echo "making asm" $*.cpp $(REDIRECT_COMMAND) $(LOG_FILE)
	@ $(CC) $(ASMFLAGS)  $*.$(FILE_EXTENSION) -o $(ASM_DIR)/$@   $(REDIRECT_COMMAND)  $(LOG_FILE)

build_objects:$(OBJ_FILES)

build_main:
	
	@ echo "building main" $^ $(REDIRECT_COMMAND) $(LOG_FILE)
	@ $(CC)  -o $(MAIN_EXE_FILE) $(OBJ_FILES_WITH_PATH)  $(CCFLAGS) $(REDIRECT_COMMAND) $(LOG_FILE)

$(OBJ_FILES):
	
	@ echo "compiling" $*.cpp $(REDIRECT_COMMAND) $(LOG_FILE)
	@ $(CC) $(CCFLAGS) -c  $*.$(FILE_EXTENSION) -o $(OBJ_DIR)/$@   $(REDIRECT_COMMAND)  $(LOG_FILE)

.PHONY:	clean
	
clean:
	@ echo "cleaning object files"
	@ $(RM) -rf $(OBJ_FILES_WITH_PATH)
	@ echo "cleaning main exe file"
	@ $(RM) -rf $(MAIN_EXE_FILE)
	@ echo "cleaning log file"
	@ $(RM) -rf $(LOG_FILE)
	@ echo "cleaning core file"
	@ $(RM) -rf $(CORE_FILE)
	@ echo "cleaning asm files"
	@ $(RM) -rf $(ASM_FILES_WITH_PATH)

create: 
	@ touch main.cpp
	@ touch bigHeader.h
	@ touch $(CURRENT_DIR).cpp
	@ touch $(CURRENT_DIR).h
