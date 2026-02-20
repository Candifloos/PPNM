\
set terminal svg ;\
set output "Fig_gamma.svg" ;\
set xlabel "x" ;\
set ylabel "y" ;\
set title "Gamma function" ;\
set grid 	;\
plot [] [-5:5]\
	"data_gamma.txt" using 1:2 with points title "Approx points", \
	gamma(x) with lines \

