CXXFLAGS += -std=c++23 -Wall -Wextra #-DLONG_DOUBLE
LDLIBS += -lstdc++ -lm
TIME = time --append --output $@ --format #"$$nthreads %e %U"


Out.txt:main ; ./$< > $@

main: main.o matrix.o
main.o: matrix.h main.cc		#matrix.h matrix.cc
matrix.o: matrix.h matrix.cc

clean:
	$(RM) [Oo]ut* main *.o err log* *.svg *.log
test:
	echo $(findstring clang++,$(CXX)) 
	echo $(CXX)
	echo $(CXXFLAGS)

out.times.data : main #Makefile
	>$@									#laver ny fil, main appender så hvis den ikke overwrites her bliver den bare længere & længere
	>log.main
	for N in $$(seq 100 20 500); do \
	./main --size $$N --output $@ ;\
	done

Times.svg: out.times.data Makefile
	echo '\
	set terminal svg ;\
	set output "$@" ;\
	set xlabel "Matrix size N" ;\
	set ylabel "Time [s]" ;\
	set title "Matrix decomposition"   ;\
	set grid 	;\
	a = 1e-3; b = 1e-5 ;\
	f(x) = a + b*x*x*x ;\
	fit f(x) "$<" using 1:2 via a,b ;\
	plot \
		"$<" using 1:2 with linespoints title "times" ,\
		f(x) title "a * bx^3 fit" ;\
	' | tee log.times_fig.gp | gnuplot
