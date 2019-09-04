set logscale y
set terminal pdfcairo font "Gill Sans,14" linewidth 1 rounded dashed
set key left bottom
set output "variter.pdf"
set xlabel "Iteration"
set ylabel "Error"
set format y "%.0e"
set style line 1 lc rgb '#0060ad' lt 1 lw 2 pt 7 ps 0.5 
set style line 2 lc rgb '#ef8a62' lt 1 lw 2 pt 26 ps 0.5 
set style line 3 lc rgb '#3CB371' lt 1 lw 2 pt 3 ps 0.5 
plot 'Apiter_sort.csv' u 2 ti "Ap" ls 2,\
 'Xiter_sort.csv' u 2 ti "x" ls 1,\
 'Riter_sort.csv' u 2 ti "r" ls 3

#reset
#set logscale y
#set terminal pdfcairo font "Gill Sans,18" linewidth 1 rounded dashed
#set output "Xiter.pdf"
#set xlabel "Iteration"
#set ylabel "Error"
#set format y "%.0e"
#plot 'Xiter_sort.csv' u 2 ti "x"

#reset
#set logscale y
#set terminal pdfcairo font "Gill Sans,18" linewidth 1 rounded dashed
#set output "Riter.pdf"
#set xlabel "Iteration"
#set ylabel "Error"
#set format y "%.0e"
#plot 'Riter_sort.csv' u 2 ti "r"


