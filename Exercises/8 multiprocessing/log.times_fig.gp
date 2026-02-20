\
set terminal svg ;\
set output "Times.svg" ;\
set xlabel "Threads" ;\
set ylabel "Time [s]" ;\
set title "Harmonic sum with 2e9 terms "   ;\
set grid 	;\
a = 1; b = 4 ;\
f(x) = a + b/x ;\
fit f(x) "out.times" using 1:2 via a,b ;\
plot \
	"out.times" using 1:2 with linespoints title "times" ,\
	f(x) title "1/n fit" ;\

