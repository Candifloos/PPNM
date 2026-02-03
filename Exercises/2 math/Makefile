CXXFLAGS = -Wall -Wextra -Werror -std=c++23 -O
LDLIBS = -lstdc++ -lm		#if we use c++ code we need to specify it as the standard linker is c & (&math)
#LINK.o = $(CXX) $(LDFLAGS) #or use a c++ linker (can also do both)

Out.txt: main
	./main > Out.txt

main: main.o sfuns.o
#	$(CXX) $(LDFLAGS) main.o hello.o sfuns.o $(LDLIBS) -o main	#links the files, unnecessary (is built in already)
#	$(CXX) $(LDFLAGS) $^ $(LDLIBS) -o main						#equivalent to above (automatic variables)

main.o: main.cc sfuns.h				#interface in .h file, implementation in .cc

sfuns.o: sfuns.cc sfuns.h


.PHONY: clean
clean:
	$(RM) main main.o Out.txt *.o
