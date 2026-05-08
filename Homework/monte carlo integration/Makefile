CXXFLAGS += -std=c++23 -Wextra
LDLIBS += -lstdc++ -lm


Out.txt : main Makefile
	./main --standard > "$@"

figs : Circle_error.svg Halton_error.svg

main: main.o matrix.o mc.o
main.o: matrix.h main.cc mc.o
matrix.o: matrix.h matrix.cc
mc.o: mc.h mc.cc

clean:
	$(RM) [Oo]ut* main *.o err log* *.svg *.log fit* *data*



# circle.data : main Makefile
# 	>$@
# 	for N in 100, 150, 200, 300, 400, 500, 750, 1e3, 5000, 1e4, 50000, 1e5, 1e6, 1e7; do \
# 	./main --circ --N_circ $$N >> $@ ;\
# 	done
VALUES := $(shell python3 -c "import numpy as np; print(' '.join(map(str, np.geomspace(1000, 1e6, 50))))") #from Gemini
circle.data : main Makefile
	>$@
	for N in $(VALUES); do \
	./main --circ --N_circ $$N --seed $$N >> $@ ;\
	done
#gives $$N as seed as well to make sure that the chosen points are not the same for different iterations	

Circle_error.svg : circle.data #Makefile
	 	@echo ' \
 		set terminal svg background "white" ; \
 		set output "$@" ; \
 		set xlabel "N Samples" ; \
 		set ylabel "Integral error" ; \
		set title "plainMC error on circle integration" ;\
 		set grid ; \
		set logscale x 10 ;\
		set xrange [750: 1.5*1e6] ;\
		set logscale y 10 ;\
		a = 1 ;\
		f(x) = a/sqrt(x);\
		fit f(x) "$<" using 1:4 via a ;\
 		plot \
			f(x) title "1/sqrt(N) fit on actual error" ,\
 			"$<" index 0 using 1:4 with linespoints title "Actual error",\
 			"$<" index 0 using 1:3 with linespoints title "Estimated error";\
 	' | gnuplot

halton.data : main Makefile
	>$@
	for N in $(VALUES); do \
	./main --halton --N_halton $$N >> $@ ;\
	done

Halton_error.svg : halton.data #Makefile
	 	@echo ' \
 		set terminal svg background "white" ; \
 		set output "$@" ; \
 		set xlabel "N Samples" ; \
 		set ylabel "Integral error" ; \
		set title "Halton error on circle integration" ;\
 		set grid ; \
		set logscale x 10 ;\
		set xrange [750: 1.5*1e6] ;\
		set logscale y 10 ;\
		a = 1 ;\
		b = 1 ;\
		f(x) = a/sqrt(x);\
		fit f(x) "$<" using 1:4 via a ;\
		g(x) = b*log(x)*log(x) / x ;\
		fit g(x) "$<" using 1:4 via b ;\
 		plot \
			f(x) title "1/sqrt(N) fit on actual error" ,\
 			"$<" index 0 using 1:4 with linespoints title "Actual error",\
 			"$<" index 0 using 1:3 with linespoints title "Estimated error",\
			g(x) title "ln(N)^2/N fit on actual error" ;\
 	' | gnuplot