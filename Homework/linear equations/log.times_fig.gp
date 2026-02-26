\
set terminal svg ;\
set output "Times.svg" ;\
set xlabel "Matrix size N" ;\
set ylabel "Time [s]" ;\
set title "Matrix decomposition"   ;\
set grid 	;\
a = 1e-3; b = 1e-5 ;\
f(x) = a + b*x*x*x ;\
fit f(x) "out.times.data" using 1:2 via a,b ;\
plot \
	"out.times.data" using 1:2 with linespoints title "times" ,\
	f(x) title "a * bx^3 fit" ;\

