# Compile project

GPPARGS = -Wall

all: logic syntax core

debug: GPPARGS += -g
debug: all

logic:
	mkdir -p obj
	g++ ${GPPARGS} -o obj/logic.o -c src/logic/logic.cpp
syntax:
	mkdir -p obj
	g++ ${GPPARGS} -o obj/syntax.o -c src/syntax/syntax.cpp

core:
	g++ ${GPPARGS} obj/*.o src/core.cpp -o logicsim

clean:
	rm -r obj/