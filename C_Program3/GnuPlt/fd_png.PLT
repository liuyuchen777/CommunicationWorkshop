set terminal postscript portrait enhanced "Helvetica" 12

#set terminal png size 800,500
set output 'with_fd.eps'

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
plot 0.5*(1-sqrt((10**((x-3)/10))/(10**((x-3)/10)+1))) t "Rayleigh Theory" w lp lt 1 pt 0,\
"Non_Rayleigh_fd0.dat" u 1:2 t "Rayleigh Simulation fd=0 Hz" w p pt 1,\
"Non_Rayleigh_fd200.dat" u 1:2 t "Rayleigh Simualtion fd=200 Hz" w p pt 2,\
"Non_Rayleigh_fd500.dat" u 1:2 t "Rayleigh Simualtion fd=500 Hz" w p pt 2,\
"Non_Rayleigh_fd1000.dat" u 1:2 t "Rayleigh Simulation fd=1000 Hz" w p pt 3 
set terminal windows
set output