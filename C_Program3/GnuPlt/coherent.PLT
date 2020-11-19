set terminal postscript portrait enhanced "Helvetica" 12

set output 'r_coherent.eps'

set grid
set key right bottom
set key box
set size 0.8,0.4
set xlabel "E_b/N_0 (dB)"
set ylabel "Average BER"
set logscale y
set xrange [0:30]
set yrange [0.0000001:1]
set format y "10^{%T}"
plot 0.5*(1-sqrt((10**((x)/10))/(10**((x)/10)+1))) t "Coherent Theory" w lp lt 2 pt 0,\
"Co_Rayleigh_fd0.dat" u 1:2 t "Coherent Simulation fd=0 Hz" w p pt 4,\
"Co_Rayleigh_fd1000.dat" u 1:2 t "Coherent Simulation fd=1000 Hz" w p pt 6 
set terminal windows
set output