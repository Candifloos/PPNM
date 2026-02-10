CXXFLAGS = -Wall -Wextra -Werror -std=c++23 -O
LDLIBS = -lstdc++ -lm		#if we use c++ code we need to specify it as the standard linker is c & (&math)
#LINK.o = $(CXX) $(LDFLAGS) #or use a c++ linker (can also do both)


main: main.o

main.o: main.cc 				#interface in .h file, implementation in .cc


N = 10
input.txt : Makefile
# 	>$@
# 	for i in $(seq $(N)); do echo "-n " $$i >> $@; done
	>$@
	for i in `seq $(N)`; do echo $$i >> $@; done


.PHONY: clean
clean:
	$(RM) main main.o Out.txt *.o out.txt input.txt
