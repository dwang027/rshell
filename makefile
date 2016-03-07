COMPILE = g++
FLAGS = -g -Wall -Werror -ansi -pedantic
SOURCES = src/assgn02.cpp

all:
    mkdir -p bin
    $(COMPILE) $(FLAGS) $(SOURCES) -o bin/rshell
    
rshell:
    mkdir -p bin
    $(COMPILE) $(FLAGS) $(SOURCES) -o bin/rshell
    
clean:
    rm bin/rshell
    rm -rf bin
