CXXFLAGS += -std=c++23 -Wall -Wextra #-DLONG_DOUBLE
LDLIBS += -lstdc++ -lm
TIME = time --append --output $@ --format "$$nthreads %e"

Out.txt:main ; ./$< > $@
figs: Linspline.svg Quadspline.svg Cubespline.svg

main: main.o matrix.o spline.o
main.o: matrix.h main.cc
matrix.o: matrix.h matrix.cc
spline.o: spline.h spline.cc
clean:
	$(RM) [Oo]ut* main *.o err log* *.svg *.log fit* *data*

Linspline.svg : lindata.txt #Makefile
	@echo ' \
		set terminal svg background "white" ; \
		set output "$@" ; \
		set xlabel "x" ; \
		set ylabel "cos(x)" ; \
		set grid ; \
		plot \
			"$<" index 0 using 1:2 with points title "Samples",\
			"$<" index 1 using 1:2 with lines title "Linear interpolator" ,\
			"$<" index 1 using 1:3 with lines title "Interpolator anti-derivative" ;\
	' | gnuplot

Quadspline.svg : quaddata.txt Makefile
	@echo ' \
		set terminal svg background "white" ; \
		set output "$@" ; \
		set xlabel "x" ; \
		set ylabel "cos(x)" ; \
		set grid ; \
		plot \
			"$<" index 0 using 1:2 with points title "Samples",\
			"$<" index 1 using 1:2 with lines title "Quadratic interpolator" ,\
			"$<" index 1 using 1:3 with lines title "Interpolator anti-derivative" ,\
			"$<" index 1 using 1:4 with lines title "Interpolator derivative" ;\
	' | gnuplot

Cubespline.svg : cubedata.txt Makefile
	@echo ' \
		set terminal svg background "white" ; \
		set output "$@" ; \
		set xlabel "x" ; \
		set ylabel "cos(x)" ; \
		set grid ; \
		plot \
			"$<" index 0 using 1:2 with points lw 3 title "Samples",\
			"$<" index 1 using 1:2 with lines lw 3 lc "cyan" title "Cubic interpolator" ,\
			"$<" index 0 using 1:2 smooth csplines lw 3 lt 2 dt 2 lc "red" title "Gnuplot Built-in Spline", \
			"$<" index 1 using 1:3 with lines lw 3 title "Interpolator anti-derivative" ,\
			"$<" index 1 using 1:4 with lines lw 3 title "Interpolator derivative" ;\
	' | gnuplot
	
