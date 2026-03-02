\
set terminal svg ;\
set output "Wavefuncs.svg" ;\
set xlabel "x" ;\
set ylabel "f" ;\
set title "Hydrogen atom wave functions"   ;\
set grid 	;\
plot \
	"out.wavefuncs.data" using 1:2 with lines title "1s" ,\
	"out.wavefuncs.data" using 1:3 with lines title "2s" ,\
	"out.wavefuncs.data" using 1:4 with lines title "3s" ,\
	"out.wavefuncs.data" using 1:5 with lines title "4s" ,\

