\
set terminal svg ;\
set output "fig_erf.svg" ;\
set xlabel "x" ;\
set ylabel "y" ;\
set title "Error function" ;\
set grid 	;\
plot [] [-1.5 : 1.5]\
	"data_erf.txt" using 1:2 with points title "Approx points" ,\
	erf(x) with lines \

