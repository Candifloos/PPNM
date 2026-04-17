CXXFLAGS += -std=c++23 -Wextra
LDLIBS += -lstdc++ -lm
TIME = time --append --output $@ --format "$$nthreads %e"

# figs: Oscillator.svg Precession.svg TBO.svg ButterflyTBO.svg

Out.txt : main Makefile
	./main --test > "$@"

main: main.o matrix.o quad.o
main.o: matrix.h main.cc quad.o
matrix.o: matrix.h matrix.cc
quad.o: quad.h quad.cc

clean:
	$(RM) [Oo]ut* main *.o err log* *.svg *.log fit* *data*

erf.data : main Makefile
	>$@
	for a in 0.01, 0.001, 1e-4, 1e-5, 1e-6, 1e-7, 1e-8, 1e-9; do \
	./main --erf --eps 0 --acc $$a >> $@ ;\
	done

# VALUES := $(shell python3 -c "import numpy as np; print(' '.join(map(str, np.geomspace(0.01, 1e-9, 100))))") #from Gemini, looks peculiar probably since accuracy increases in steps (i.e. 0.1 & 0.01 have same accuracy corresponding to just 1 step?)
# erf.data : main Makefile
# 	>$@
# 	for a in $(VALUES); do \
# 	./main --eps 0 --acc $$a >> $@ ;\
# 	done


Erf.svg : erf.data
	@echo ' \
		set terminal svg background "white" ; \
		set output "$@" ; \
		set xlabel "acc" ; \
		set ylabel "delta erf" ; \
		set grid ; \
		set xrange [] reverse ;\
		set logscale x 10 ;\
		set logscale y 10 ;\
		plot \
			"$<" index 0 using 1:2 with linespoints title "Diff",\
	' | gnuplot
