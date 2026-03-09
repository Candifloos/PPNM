CXXFLAGS += -std=c++23 -Wall -Wextra #-DLONG_DOUBLE
LDLIBS += -lstdc++ -lm
TIME = time --append --output $@ --format "$$nthreads %e"

Out.txt:main ; ./$< > $@
figs: Drconvergence.svg Rmaxconvergence.svg Wavefuncs.svg Times_dr_convergence.svg Times_rmax_convergence.svg

main: main.o matrix.o evd.o
main.o: matrix.h main.cc		#matrix.h matrix.cc
matrix.o: matrix.h matrix.cc
evd.o: evd.h evd.cc

clean:
	$(RM) [Oo]ut* main *.o err log* *.svg *.log






out.drs.data : main
	>$@									#laver ny fil, main appender så hvis den ikke overwrites her bliver den bare længere & længere
# 	>log.main
	for deltar in $$(seq 0.05 0.05 1); do \
	./main --dr $$deltar --output $@ --testoff;\
	done

out.rmaxs.data : main
	>$@									#laver ny fil, main appender så hvis den ikke overwrites her bliver den bare længere & længere
# 	>log.main
	for rmax in $$(seq 0.5 0.5 12); do \
	./main --rmax $$rmax --output $@ --testoff;\
	done

out.wavefuncs.data : main
# 	>$@									#laver ny fil, main appender så hvis den ikke overwrites her bliver den bare længere & længere
	./main --testoff --wavefuncout $@ --rmax 30 --dr 0.2;\

out.dr.times : main #Makefile
	>$@
	>log.main
	for nthreads in 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16; do \
		$(TIME) ./main --threads $$nthreads --testoff --run_dr_conv --conv_start 0.1 --conv_end 1 --conv_step 0.005 >> log.main ;\
	done

out.rmax.times : main #Makefile
	>$@
	>log.main
	for nthreads in 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16; do \
		$(TIME) ./main --threads $$nthreads --testoff --run_rmax_conv --conv_start 3 --conv_end 10 --conv_step 0.1 >> log.main ;\
	done

Times_dr_convergence.svg : out.dr.times
	echo '\
	set terminal svg ;\
	set output "$@" ;\
	set xlabel "Threads" ;\
	set ylabel "Runtime" ;\
	set title "dr Convergence run"   ;\
	set grid 	;\
	plot \
		"$<" using 1:2 with linespoints ,\
	' | tee log.times_fig.gp | gnuplot


Times_rmax_convergence.svg : out.rmax.times
	echo '\
	set terminal svg ;\
	set output "$@" ;\
	set xlabel "Threads" ;\
	set ylabel "Runtime" ;\
	set title "Rmax Convergence"   ;\
	set grid 	;\
	plot \
		"$<" using 1:2 with linespoints ,\
	' | tee log.times_fig.gp | gnuplot

Wavefuncs.svg: out.wavefuncs.data
	echo '\
	set terminal svg ;\
	set output "$@" ;\
	set xlabel "x" ;\
	set ylabel "f" ;\
	set title "Hydrogen atom wave functions"   ;\
	set grid 	;\
	plot \
		"$<" using 1:2 with lines title "1s" ,\
		"$<" using 1:3 with lines title "2s" ,\
		"$<" using 1:4 with lines title "3s" ,\
		"$<" using 1:5 with lines title "4s" ,\
	' | tee log.times_fig.gp | gnuplot

Drconvergence.svg: out.drs.data
	echo '\
	set terminal svg ;\
	set output "$@" ;\
	set xlabel "dr" ;\
	set ylabel "Energy [E_h]" ;\
	set title "Hydrogen atom energy convergence"   ;\
	set grid 	;\
	set xrange [] reverse ;\
	set yrange [-0.51 : -0.4] ;\
	plot \
		"$<" using 2:3 with linespoints title "energy" ,\
	' | tee log.times_fig.gp | gnuplot

Rmaxconvergence.svg: out.rmaxs.data
	echo '\
	set terminal svg ;\
	set output "$@" ;\
	set xlabel "rmax" ;\
	set ylabel "Energy [E_h]" ;\
	set title "Hydrogen atom energy convergence"   ;\
	set grid 	;\
	plot [] [-0.6 : -0.1]\
		"$<" using 1:3 with linespoints title "energy" ,\
	' | tee log.times_fig.gp | gnuplot