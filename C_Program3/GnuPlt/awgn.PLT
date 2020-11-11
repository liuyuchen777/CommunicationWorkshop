set terminal postscript portrait enhanced "Helvetica" 12

# set the output to a eps file
set output "without_shift.eps"

# set the output to a png file
# set terminal png size 500,500
# set output 'without_shift.png'

set grid
set key right top
set key box
set size 0.8,0.4
set xlabel "E_b/N_0 (dB)"
set ylabel "Average BER"
set logscale y
set xrange [0:10]
set yrange [0.0000001:1]
set format y "10^{%T}"
plot 0.5*erfc(2*sqrt(10**(x/10))*sin(pi/8)) t "Noncoherent Theory" w lp lt 1 pt 0,\
"noncoherent_without_shift.dat" u 1:2 t "Noncoherent Simulation" w lp lt 3 pt 1,\
"coherent_without_shift.dat" u 1:2 t "Coherent Simulation" w lp lt 6 pt 5
set terminal windows
set output