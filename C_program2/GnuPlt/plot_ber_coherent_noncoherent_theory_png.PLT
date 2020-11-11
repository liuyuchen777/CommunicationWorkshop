set terminal postscript portrait enhanced "Helvetica" 12

set terminal png size 500,500
set output 'non-coherent.png'

set grid
set key right top
set xlabel "E_b/N_0 (dB)"
set ylabel "Average BER"
set logscale y
set xrange [0:11]
set yrange [0.0000001:1]
set format y "10^{%T}"
plot 0.5*erfc(2*sqrt(10**(x/10))*sin(pi/8)) t "Non-conherent Theory" w lp lt 1 pt 0,\
"noncoherent_with_shift.dat" u 1:2 t "Non-coherent Simulation" w lp pt 1
set terminal windows
set output