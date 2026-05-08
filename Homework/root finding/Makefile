CXXFLAGS += -std=c++23 -Wextra
LDLIBS += -lstdc++ -lm


Out.txt : main Makefile
	./main --standard > "$@"

figs : Hydrogen_WF.svg Convergence_ode_prec.svg Convergence_rmax.svg Convergence_rmin.svg

main: main.o matrix.o rf.o qr.o ode.o
main.o: matrix.h main.cc rf.o
matrix.o: matrix.h matrix.cc
rf.o: rf.h rf.cc matrix.h
qr.o: qr.cc qr.h matrix.h
ode.o: ode.cc ode.h

clean:
	$(RM) [Oo]ut* main *.o err log* *.svg *.log fit* *data*


hydrogen.data : main #Makefile
	>$@
	./main --wavefunc >> $@ ;\

Hydrogen_WF.svg : hydrogen.data Makefile
	 	@echo ' \
 		set terminal svg background "white" ; \
 		set output "$@" ; \
 		set xlabel "r" ; \
 		set ylabel "\psi" ; \
		set title "s-wave wave function for r_{max} = 8" ;\
 		set grid ; \
		f(x) = x*exp(-x) ;\
 		plot \
			f(x) with points title "Analytic s-wave" ,\
 			"$<" index 0 using 1:2 with lines title "Calculated s-wave";\
 	' | gnuplot


VALUES := $(shell python3 -c "import numpy as np; print(' '.join(map(str, np.geomspace(1e-4, 5e-1, 10))))") #from Gemini
r_min.data : main #Makefile
	>$@ 
	for rmin in $(VALUES); do \
	./main --r_min_conv --r_min $$rmin --r_max 12 --ODE_acc 1e-3 --ODE_eps 1e-3 >> $@ ;\
	done

Convergence_rmin.svg : r_min.data Makefile
	 	@echo ' \
 		set terminal svg background "white" ; \
 		set output "$@" ; \
 		set xlabel "r_{min}" ; \
 		set ylabel "E-E_{exact}" ; \
		set title "Convergence of estimated E as function of r_{min}" ;\
 		set grid ; \
		set logscale x 10 ;\
		set logscale y 10 ;\
		set xrange reverse ;\
 		plot \
 			"$<" index 0 using 1:2 with linespoints title "Energy difference",\
 	' | gnuplot

r_max.data : main #Makefile
	>$@ 
	for rmax in 1,2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15; do \
	./main --r_max_conv --r_max $$rmax --ODE_acc 1e-3 --ODE_eps 1e-3 >> $@ ;\
	done

Convergence_rmax.svg : r_max.data Makefile
	 	@echo ' \
 		set terminal svg background "white" ; \
 		set output "$@" ; \
 		set xlabel "r_{max}" ; \
 		set ylabel "E-E_{exact}" ; \
		set title "Convergence of estimated E as function of r_{max}" ;\
 		set grid ; \
		set logscale y 10 ;\
 		plot \
 			"$<" index 0 using 1:2 with linespoints title "Energy difference",\
 	' | gnuplot

acc_VALUES := $(shell python3 -c "import numpy as np; print(' '.join(map(str, np.geomspace(1e-5, 1, 10))))") #from Gemini
ode_precision.data : main #Makefile
	>$@ 
	for acc in $(acc_VALUES); do \
	./main --ODE_prec_conv --ODE_acc $$acc --ODE_eps $$acc >> $@ ;\
	done

Convergence_ode_prec.svg : ode_precision.data Makefile
	 	@echo ' \
 		set terminal svg background "white" ; \
 		set output "$@" ; \
 		set xlabel "acc & eps for ODE" ; \
 		set ylabel "E-E_{exact}" ; \
		set title "Convergence of estimated E as function of ODE acc=eps" ;\
 		set grid ; \
		set logscale y 10 ;\
		set logscale x 10 ;\
		set xrange reverse ;\
 		plot \
 			"$<" index 0 using 1:2 with linespoints title "Energy difference",\
 	' | gnuplot




# 		set xrange reverse ;\
# 		set logscale x 10 ;\
# circle.data : main Makefile
# 	>$@
# 	for N in 100, 150, 200, 300, 400, 500, 750, 1e3, 5000, 1e4, 50000, 1e5, 1e6, 1e7; do \
# 	./main --circ --N_circ $$N >> $@ ;\
# 	done
# VALUES := $(shell python3 -c "import numpy as np; print(' '.join(map(str, np.geomspace(1000, 1e6, 50))))") #from Gemini
# circle.data : main Makefile
# 	>$@
# 	for N in $(VALUES); do \
# 	./main --circ --N_circ $$N --seed $$N >> $@ ;\
# 	done
# #gives $$N as seed as well to make sure that the chosen points are not the same for different iterations	

# Circle_error.svg : circle.data #Makefile
# 	 	@echo ' \
#  		set terminal svg background "white" ; \
#  		set output "$@" ; \
#  		set xlabel "N Samples" ; \
#  		set ylabel "Integral error" ; \
# 		set title "plainMC error on circle integration" ;\
#  		set grid ; \
# 		set logscale x 10 ;\
# 		set xrange [750: 1.5*1e6] ;\
# 		set logscale y 10 ;\
# 		a = 1 ;\
# 		f(x) = a/sqrt(x);\
# 		fit f(x) "$<" using 1:4 via a ;\
#  		plot \
# 			f(x) title "1/sqrt(N) fit on actual error" ,\
#  			"$<" index 0 using 1:4 with linespoints title "Actual error",\
#  			"$<" index 0 using 1:3 with linespoints title "Estimated error";\
#  	' | gnuplot

# halton.data : main Makefile
# 	>$@
# 	for N in $(VALUES); do \
# 	./main --halton --N_halton $$N >> $@ ;\
# 	done

# Halton_error.svg : halton.data #Makefile
# 	 	@echo ' \
#  		set terminal svg background "white" ; \
#  		set output "$@" ; \
#  		set xlabel "N Samples" ; \
#  		set ylabel "Integral error" ; \
# 		set title "Halton error on circle integration" ;\
#  		set grid ; \
# 		set logscale x 10 ;\
# 		set xrange [750: 1.5*1e6] ;\
# 		set logscale y 10 ;\
# 		a = 1 ;\
# 		b = 1 ;\
# 		f(x) = a/sqrt(x);\
# 		fit f(x) "$<" using 1:4 via a ;\
# 		g(x) = b*log(x)*log(x) / x ;\
# 		fit g(x) "$<" using 1:4 via b ;\
#  		plot \
# 			f(x) title "1/sqrt(N) fit on actual error" ,\
#  			"$<" index 0 using 1:4 with linespoints title "Actual error",\
#  			"$<" index 0 using 1:3 with linespoints title "Estimated error",\
# 			g(x) title "ln(N)^2/N fit on actual error" ;\
#  	' | gnuplot