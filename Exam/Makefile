CXXFLAGS += -std=c++23 -Wextra
LDLIBS += -lstdc++ -lm


Out.txt : main Makefile
	./main 1> "$@" 2> "plot.data"

figs : AdaptiveLM_comparison.svg AdaptiveLM_comparison2.svg

main: main.o matrix.o min.o qr.o
main.o: matrix.h main.cc min.o
matrix.o: matrix.h matrix.cc
min.o: min.h min.cc matrix.h
qr.o: qr.cc qr.h matrix.h

clean:
	$(RM) [Oo]ut* main *.o err log* *.svg *.log fit* *.data

#Plots work fine, just doesnt really show anything that the table doesnt
AdaptiveLM_comparison.svg : plot.data Makefile
	 	@echo ' \
 		set terminal svg background "white" ; \
 		set output "$@" ; \
 		set xlabel "\lambda_0" ; \
 		set ylabel "N_{evals}" ; \
		set title "Rosenbrock, started at (-100,100)" ;\
 		set grid ; \
		set logscale x ;\
 		plot \
 			"$<" index 0 using 1:2 with points title "Simple its",\
 			"$<" index 0 using 1:3 with points title "Simple QR",\
 			"$<" index 0 using 1:4 with points title "Adaptive its",\
 			"$<" index 0 using 1:5 with points title "Adaptive QR",\
 	' | gnuplot


AdaptiveLM_comparison2.svg : plot.data Makefile
	 	@echo ' \
 		set terminal svg background "white" ; \
 		set output "$@" ; \
 		set xlabel "\lambda_0" ; \
 		set ylabel "N_{evals}" ; \
		set title "Rosenbrock, started at (0.9,1.1)" ;\
 		set grid ; \
		set logscale x ;\
 		plot \
 			"$<" index 1 using 1:2 with points title "Simple its",\
 			"$<" index 1 using 1:3 with points title "Simple QR",\
 			"$<" index 1 using 1:4 with points title "Adaptive its",\
 			"$<" index 1 using 1:5 with points title "Adaptive QR",\
 	' | gnuplot
