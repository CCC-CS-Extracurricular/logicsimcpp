# Compile project

GPPARGS = -Wall

all: logic syntax core

core:
	g++ ${GPPARGS} obj/*.o src/core.cpp -o logicsim

logic:
	mkdir -p obj
	g++ ${GPPARGS} -o obj/logic.o -c src/logic/logic.cpp
syntax:
	mkdir -p obj
	g++ ${GPPARGS} -o obj/syntax.o -c src/syntax/syntax.cpp

clean:
	rm -r obj/