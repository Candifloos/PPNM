CXXFLAGS = -Wall -Wextra -Werror -std=c++23 -O
LDLIBS = -lstdc++ -lm		#if we use c++ code we need to specify it as the standard linker is c & (&math)
#LINK.o = $(CXX) $(LDFLAGS) #or use a c++ linker (can also do both)

Out.txt: main
	./main > Out.txt

main: main.o vec.o

main.o: main.cc vec.h				#interface in .h file, implementation in .cc

vec.o: vec.cc vec.h


.PHONY: clean
clean:
	$(RM) main main.o Out.txt *.o
