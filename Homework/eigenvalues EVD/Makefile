CXXFLAGS += -std=c++23 -Wall -Wextra #-DLONG_DOUBLE
LDLIBS += -lstdc++ -lm
TIME = time --append --output $@ --format #"$$nthreads %e %U"

Out.txt:main ; ./$< > $@
figs: Drconvergence.svg Rmaxconvergence.svg Wavefuncs.svg

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
	./main --testoff --wavefuncout $@ --rmax 25 --dr 0.2;\

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

