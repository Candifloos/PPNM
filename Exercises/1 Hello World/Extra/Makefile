CXXFLAGS = -Wall -Wextra -Werror -std=c++23 -O
LDLIBS = -lstdc++ -lm
Out.txt: main
	./main > Out.txt

main: main.o hello.o

main.o: main.cc hello.h

hello.o: hello.h hello.cc

.PHONY: clean
clean:
	$(RM) main main.o Out.txt *.o
