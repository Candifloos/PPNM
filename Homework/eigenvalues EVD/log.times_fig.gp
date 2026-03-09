\
set terminal svg ;\
set output "Times_rmax_convergence.svg" ;\
set xlabel "Threads" ;\
set ylabel "Runtime" ;\
set title "Rmax Convergence"   ;\
set grid 	;\
plot \
	"out.rmax.times" using 1:2 with linespoints ,\

