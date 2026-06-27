CXXFLAGS += -std=c++23 -Wextra
LDLIBS += -lstdc++ -lm


Out.txt : main Makefile
	./main < higgs.data --standard 1> "$@" 2> higgs_fit.data

figs : Higgs.svg

main: main.o matrix.o min.o qr.o
main.o: matrix.h main.cc min.o
matrix.o: matrix.h matrix.cc
min.o: min.h min.cc matrix.h
qr.o: qr.cc qr.h matrix.h

clean:
	$(RM) [Oo]ut* main *.o err log* *.svg *.log fit* 





Higgs.svg : higgs.data higgs_fit.data Makefile
	 	@echo ' \
 		set terminal svg background "white" ; \
 		set output "$@" ; \
 		set xlabel "E" ; \
 		set ylabel "signal" ; \
		set title "Higgs boson measurements" ;\
 		set grid ; \
 		plot \
 			"$<" index 0 using 1:2:3 with errorbars title "Data points",\
			"higgs_fit.data" using 1:2 with lines title "Fit" ;\
 	' | gnuplot

#  			"$<" index 0 using 1:2 with points title "Data points",\

# hydrogen.data : main #Makefile
# 	>$@
# 	./main --wavefunc >> $@ ;\

# Hydrogen_WF.svg : hydrogen.data Makefile
# 	 	@echo ' \
#  		set terminal svg background "white" ; \
#  		set output "$@" ; \
#  		set xlabel "r" ; \
#  		set ylabel "\psi" ; \
# 		set title "s-wave wave function for r_{max} = 8" ;\
#  		set grid ; \
# 		f(x) = x*exp(-x) ;\
#  		plot \
# 			f(x) with points title "Analytic s-wave" ,\
#  			"$<" index 0 using 1:2 with lines title "Calculated s-wave";\
#  	' | gnuplot


# VALUES := $(shell python3 -c "import numpy as np; print(' '.join(map(str, np.geomspace(1e-4, 5e-1, 10))))") #from Gemini
# r_min.data : main #Makefile
# 	>$@ 
# 	for rmin in $(VALUES); do \
# 	./main --r_min_conv --r_min $$rmin --r_max 12 --ODE_acc 1e-3 --ODE_eps 1e-3 >> $@ ;\
# 	done

# Convergence_rmin.svg : r_min.data Makefile
# 	 	@echo ' \
#  		set terminal svg background "white" ; \
#  		set output "$@" ; \
#  		set xlabel "r_{min}" ; \
#  		set ylabel "E-E_{exact}" ; \
# 		set title "Convergence of estimated E as function of r_{min}" ;\
#  		set grid ; \
# 		set logscale x 10 ;\
# 		set logscale y 10 ;\
# 		set xrange reverse ;\
#  		plot \
#  			"$<" index 0 using 1:2 with linespoints title "Energy difference",\
#  	' | gnuplot

# r_max.data : main #Makefile
# 	>$@ 
# 	for rmax in 1,2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15; do \
# 	./main --r_max_conv --r_max $$rmax --ODE_acc 1e-3 --ODE_eps 1e-3 >> $@ ;\
# 	done

# Convergence_rmax.svg : r_max.data Makefile
# 	 	@echo ' \
#  		set terminal svg background "white" ; \
#  		set output "$@" ; \
#  		set xlabel "r_{max}" ; \
#  		set ylabel "E-E_{exact}" ; \
# 		set title "Convergence of estimated E as function of r_{max}" ;\
#  		set grid ; \
# 		set logscale y 10 ;\
#  		plot \
#  			"$<" index 0 using 1:2 with linespoints title "Energy difference",\
#  	' | gnuplot

# acc_VALUES := $(shell python3 -c "import numpy as np; print(' '.join(map(str, np.geomspace(1e-5, 1, 10))))") #from Gemini
# ode_precision.data : main #Makefile
# 	>$@ 
# 	for acc in $(acc_VALUES); do \
# 	./main --ODE_prec_conv --ODE_acc $$acc --ODE_eps $$acc >> $@ ;\
# 	done

# Convergence_ode_prec.svg : ode_precision.data Makefile
# 	 	@echo ' \
#  		set terminal svg background "white" ; \
#  		set output "$@" ; \
#  		set xlabel "acc & eps for ODE" ; \
#  		set ylabel "E-E_{exact}" ; \
# 		set title "Convergence of estimated E as function of ODE acc=eps" ;\
#  		set grid ; \
# 		set logscale y 10 ;\
# 		set logscale x 10 ;\
# 		set xrange reverse ;\
#  		plot \
#  			"$<" index 0 using 1:2 with linespoints title "Energy difference",\
#  	' | gnuplot

