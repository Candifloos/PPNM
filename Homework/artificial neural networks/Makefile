CXXFLAGS += -std=c++23 -Wextra
LDLIBS += -lstdc++ -lm


# Out.txt : main Makefile
# 	./main < higgs.data --standard 1> "$@" 2> higgs_fit.data

# Out.txt : main Makefile
# 	./main  1> "$@"

figs : Annfit.svg AnnDerivatives.svg AnnAntiDerivative.svg

main: main.o matrix.o min.o qr.o ann.o
main.o: matrix.h main.cc min.o
matrix.o: matrix.h matrix.cc
min.o: min.h min.cc matrix.h
qr.o: qr.cc qr.h matrix.h
ann.o: ann.cc ann.h min.h

clean:
	$(RM) [Oo]ut* main *.o err log* *.svg *.log fit* 

fit.data : main
	./main >$@


Annfit.svg : fit.data Makefile
	 	@echo ' \
 		set terminal svg background "white" ; \
 		set output "$@" ; \
 		set xlabel "x" ; \
 		set ylabel "f" ; \
		set title "ANN prediction" ;\
 		set grid ; \
 		plot \
 			"$<" index 0 using 1:2 with lines title "Function values",\
 			"$<" index 0 using 1:3 every 3 with points title "ANN prediction",\
 	' | gnuplot

AnnDerivatives.svg : fit.data Makefile
	 	@echo ' \
 		set terminal svg background "white" ; \
 		set output "$@" ; \
 		set xlabel "x" ; \
 		set ylabel "f" ; \
		set title "Derivatives" ;\
 		set grid ; \
 		plot \
			"$<" index 0 using 1:2 with lines title "Function values",\
 			"$<" index 0 using 1:4 with lines title "1st order derivative",\
 			"$<" index 0 using 1:5 every 3 with points title "ANN 1st order derivative",\
			"$<" index 0 using 1:6 with lines title "Analytic 2nd order derivative",\
 			"$<" index 0 using 1:7 every 3 with points title "ANN 2nd order derivative",\
 	' | gnuplot

AnnAntiDerivative.svg : fit.data Makefile
	 	@echo ' \
 		set terminal svg background "white" ; \
 		set output "$@" ; \
 		set xlabel "x" ; \
 		set ylabel "f" ; \
		set title "Anti-Derivative" ;\
 		set grid ; \
 		plot \
			"$<" index 0 using 1:2 with lines title "Function values",\
 			"$<" index 0 using 1:8 with lines title "Trapezoid",\
 			"$<" index 0 using 1:9 every 3 with points title "ANN anti-derivative",\
 	' | gnuplot

#1:x  2:g(x)  3:ann(x)  4:g'(x)  5:ann'(x)  6:g''(x)  7:ann''(x)  8:g_int(x)  9:ann_int(x)

