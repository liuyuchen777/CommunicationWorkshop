set terminal postscript portrait enhanced "Helvetica" 12

set terminal png size 800,500
set output 'select.jpg'

set grid
set key right bottom
set xlabel "E_b/N_0 (dB)"
set ylabel "Average BER"
set logscale y
set xrange [0:30]
set yrange [0.0000001:1]
set format y "10^{%T}"
plot 0.5*(1-sqrt((10**((x-3)/10))/(10**((x-3)/10)+1))) t "Rayleigh Non-coherent Theory" w lp lt 2 pt 0,\
"select_fd0.dat" u 1:2 t "Selective Fading, fd=0 Hz" w p pt 3,\
"select_fd100.dat" u 1:2 t "Selective Fading, fd=100 Hz" w p pt 6,\
"select_fd1000.dat" u 1:2 t "Selective Fading, fd=1000 Hz" w p pt 8

set terminal windows
set output