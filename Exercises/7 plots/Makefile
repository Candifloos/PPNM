CXXFLAGS = -Wall -Wextra -Werror -std=c++23 -O
LDLIBS = -lstdc++ -lm		#if we use c++ code we need to specify it as the standard linker is c & (&math)

all: data figs


main: main.o sfuns.o

main.o: main.cc sfuns.h				#interface in .h file, implementation in .cc

sfuns.o: sfuns.cc sfuns.h


data: data_erf.txt data_gamma.txt

data_erf.txt: main Makefile
	./main -xmin -2 -xmax 2 -erf > "$@"

data_gamma.txt: main Makefile
	./main -xmin -5 -xmax 5 -gamma -dx 0.0625 > "$@"


figs: fig_erf.svg fig_gamma.svg fig_lngamma.svg			#ved ikke om der er en pænere måde at gøre det på

fig_erf.svg: data_erf.txt Makefile
	echo '\
	set terminal svg ;\
	set output "$@" ;\
	set xlabel "x" ;\
	set ylabel "y" ;\
	set title "Error function" ;\
	set grid 	;\
	plot [] [-1.5 : 1.5]\
		"$<" using 1:2 with points title "Approx points" ,\
		erf(x) with lines \
	' | tee log.fig_erf.gp | gnuplot

fig_gamma.svg: data_gamma.txt Makefile
	echo '\
	set terminal svg ;\
	set output "$@" ;\
	set xlabel "x" ;\
	set ylabel "y" ;\
	set title "Gamma function" ;\
	set grid 	;\
	plot [] [-5:5]\
		"$<" using 1:2 with points title "Approx points", \
		gamma(x) with lines \
	' | tee log.fig_gamma.gp | gnuplot

fig_lngamma.svg: data_gamma.txt Makefile
	echo '\
	set terminal svg ;\
	set output "$@" ;\
	set xlabel "x" ;\
	set ylabel "y" ;\
	set title "lngamma function" ;\
	set grid 	;\
	plot\
		"$<" using 1:3 with points title "Approx points", \
		log(gamma(x)) with lines \
	' | tee log.fig_lngamma.gp | gnuplot


.PHONY: clean
clean:
	$(RM) main main.o Out.txt *.o data* log* fig*
