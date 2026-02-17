\
set terminal svg ;\
set output "fig_lngamma.svg" ;\
set xlabel "x" ;\
set ylabel "y" ;\
set title "lngamma function" ;\
set grid 	;\
plot\
	"data_gamma.txt" using 1:3 with points title "Approx points", \
	log(gamma(x)) with lines \

