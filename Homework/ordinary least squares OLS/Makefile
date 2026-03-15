CXXFLAGS += -std=c++23 -Wall -Wextra #-DLONG_DOUBLE
LDLIBS += -lstdc++ -lm
TIME = time --append --output $@ --format "$$nthreads %e"

Out.txt:main ; ./$< > $@

main: main.o matrix.o qr.o ols.o
main.o: matrix.h main.cc
matrix.o: matrix.h matrix.cc
qr.o : qr.h qr.cc
ols.o : ols.h ols.cc

clean:
	$(RM) [Oo]ut* main *.o err log* *.svg *.log fit*

# Fit.svg : data.txt fitparams.txt
# 	@echo ' \
# 		set terminal svg background "white" ; \
# 		set output "$@" ; \
# 		set xlabel "Time [days]" ; \
# 		set ylabel "log Activity" ; \
# 		set grid ; \
# 		line1 = system("sed -n 1p fitparams.txt") ; \
# 		line2 = system("sed -n 2p fitparams.txt") ; \
# 		b = word(line1, 1) + 0.0 ; \
# 		db = word(line1, 2) + 0.0 ; \
# 		a = word(line2, 1) + 0.0 ; \
# 		da = word(line2, 2) + 0.0 ; \
# 		f(x) = -a * x + b ; \
# 		f_low(x) = -(a+da) * x + (b-db) ; \
# 		f_high(x) = -(a-da) * x + (b+db) ; \
# 		plot \
# 			"$<" using 1:2:3 with yerrorbars title "Data", \
# 			f(x) with lines lc rgb "red" lw 2 title "Best Fit", \
# 			f_low(x) with lines lc rgb "blue" dt 2 title "Uncertainty Bound", \
# 			f_high(x) with lines lc rgb "blue" dt 2 notitle ; \
# 	' | gnuplot

Fit.svg : data.txt fitparams.txt
	@echo ' \
		set terminal svg background "white" ; \
		set output "$@" ; \
		set xlabel "Time [days]" ; \
		set ylabel "log Activity" ; \
		set grid ; \
		line1 = system("sed -n 1p fitparams.txt") ; \
		line2 = system("sed -n 2p fitparams.txt") ; \
		b = word(line1, 1) + 0.0 ; \
		db = word(line1, 2) + 0.0 ; \
		a = word(line2, 1) + 0.0 ; \
		da = word(line2, 2) + 0.0 ; \
		f(x) = -a * x + b ; \
		f_low(x) = -(a+da) * x + (b-db) ; \
		f_high(x) = -(a-da) * x + (b+db) ; \
		set style fill transparent solid 0.2 noborder ; \
		plot \
			"$<" using 1:2:3 with yerrorbars pt 7 lc rgb "black" title "Data", \
			f(x) with lines lc rgb "red" lw 2 title "Best Fit", \
			"+" using 1:(f_low($$1)):(f_high($$1)) with filledcurves lc rgb "red" title "1-sigma band" ; \
	' | gnuplot

#dt giver stiplet linje





