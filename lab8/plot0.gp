
set terminal png size 1200,800
set output 'plot.png'


set title "Время чтения элемента массива"
set xlabel "Размер массива (KB)"
set ylabel "Время чтения элемента массива (мс)"
set grid


set xtics rotate by -45


set style data lines

plot "out_s.txt" using ($1/1024):2 title 'Прямой обход' with lines linecolor rgb "blue" lw 2, \
     "out_rev.txt" using ($1/1024):2 title 'Обратный обход' with lines linecolor rgb "green" lw 2, \
     "out_ran.txt" using ($1/1024):2 title 'Случайный обход' with lines linecolor rgb "red" lw 2
replot
set output
