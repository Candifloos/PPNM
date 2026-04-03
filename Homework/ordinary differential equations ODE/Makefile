CXXFLAGS += -std=c++23 -Wextra
LDLIBS += -lstdc++ -lm
TIME = time --append --output $@ --format "$$nthreads %e"

figs: Oscillator.svg Precession.svg TBO.svg ButterflyTBO.svg

main: main.o matrix.o ode.o
main.o: matrix.h main.cc ode.o
matrix.o: matrix.h matrix.cc
ode.o: ode.h ode.cc

clean:
	$(RM) [Oo]ut* main *.o err log* *.svg *.log fit* *data*


oscillator.data : main Makefile
	./main --oscillator --xmax 20 > "$@"

precession.data : main Makefile
	./main --precession --u0 1 0 --max_step 0.01 > "$@"
	@echo "" >> $@
	@echo "" >> $@
	./main --precession --u0 1 -0.5 --xmax 200 >> "$@"
	@echo "" >> $@
	@echo "" >> $@
	./main --precession --u0 1 -0.5 --releps 0.01 --xmax 200 >> "$@"

tbo.data : main Makefile
	./main --tbo --xmax 6.3252 > "$@"

tboButterfly.data : main Makefile	#Slight drift can be reduced by reducing eps, acc but increases time quite a lot
	./main --butterfly --xmax 50 --eps 1e-4 --acc 1e-4  > "$@"

Oscillator.svg : oscillator.data
	@echo ' \
		set terminal svg background "white" ; \
		set output "$@" ; \
		set xlabel "x" ; \
		set ylabel "y" ; \
		set grid ; \
		plot \
			"$<" index 0 using 1:2 with lines title "theta",\
			"$<" index 0 using 1:3 with lines title "omega",\
	' | gnuplot

Precession.svg : precession.data Makefile
	@echo ' \
		set terminal svg background "white" ; \
		set output "$@" ; \
		set xlabel "x" ; \
		set ylabel "y" ; \
		set size ratio -1 ; \
		set grid ; \
		plot \
			"$<" index 0 using (1/$$2)*cos($$1):(1/$$2)*sin($$1) with lines title "Newtonian circular" ,\
			"$<" index 1 using (1/$$2)*cos($$1):(1/$$2)*sin($$1) with lines title "Newtonian elliptic" ,\
			"$<" index 2 using (1/$$2)*cos($$1):(1/$$2)*sin($$1) with lines title "Relativistic" \
	' | gnuplot

TBO.svg : tbo.data
	@echo ' \
		set terminal svg background "white" ; \
		set output "$@" ; \
		set xlabel "x" ; \
		set ylabel "y" ; \
		set grid ; \
		plot \
			"$<" index 0 using 2:3 with lines lw 3 title "Body 1",\
			"$<" index 0 using 4:5 with lines lw 3 lt 2 dt 2 title "Body 2",\
			"$<" index 0 using 6:7 with lines lw 3 lt 4 dt 4 title "Body 3",\
	' | gnuplot

ButterflyTBO.svg : tboButterfly.data
	@echo ' \
		set terminal svg background "white" ; \
		set output "$@" ; \
		set xlabel "x" ; \
		set ylabel "y" ; \
		set grid ; \
		plot \
			"$<" index 0 using 2:3 with lines title "Body 1",\
			"$<" index 0 using 4:5 with lines title "Body 2",\
			"$<" index 0 using 6:7 with lines title "Body 3",\
	' | gnuplot