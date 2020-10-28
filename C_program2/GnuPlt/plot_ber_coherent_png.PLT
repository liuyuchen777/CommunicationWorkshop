set terminal postscript portrait enhanced "Helvetica" 12

# set the output to a eps file
# set output "awgn.eps"

# set the output to a png file
set terminal png size 500,500
set output 'awgn.png'

set grid
set key right top
# set size 0.8,0.4
set xlabel "E_b/N_0 (dB)"
set ylabel "Average BER"
set logscale y
set xrange [0:10]
set yrange [0.0000001:1]
set format y "10^{%T}"
plot 0.5*erfc(sqrt(exp(x*log(10)/10))) t "Theory" w lp lt 1 pt 0,\
"awgn.dat" u 1:2 t "Simulation With Gray" w lp pt 1
set terminal windows
set output