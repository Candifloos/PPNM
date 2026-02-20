CXXFLAGS += -Wall -Wextra -std=c++23 -O
LDLIBS = -lstdc++ -lm -lpthread		#if we use c++ code we need to specify it as the standard linker is c & (&math)
TIME = time --append --output $@ --format "$$nthreads %e %U"



all: Times.svg


main: main.o sfuns.o

main.o: main.cc sfuns.h				#interface in .h file, implementation in .cc

sfuns.o: sfuns.cc sfuns.h


# TIME = time --format "$$nthreads %e %U" --append --output $@
N = 2e9
out.times : main #Makefile
	>$@
	>log.main
	for nthreads in 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16; do \
		$(TIME) ./main --threads $$nthreads --terms $(N) >>log.main ;\
	done


Times.svg: out.times Makefile
	echo '\
	set terminal svg ;\
	set output "$@" ;\
	set xlabel "Threads" ;\
	set ylabel "Time [s]" ;\
	set title "Harmonic sum with $(N) terms "   ;\
	set grid 	;\
	a = 1; b = 4 ;\
	f(x) = a + b/x ;\
	fit f(x) "$<" using 1:2 via a,b ;\
	plot \
		"$<" using 1:2 with linespoints title "times" ,\
		f(x) title "1/n fit" ;\
	' | tee log.times_fig.gp | gnuplot



.PHONY: clean
clean:
	$(RM) main main.o Out.txt *.o data* log* *.log *.svg out.times
