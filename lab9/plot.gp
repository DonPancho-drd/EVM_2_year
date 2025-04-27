
set terminal png size 1200,800
set output 'plot1.png'

set xlabel 'Число фрагментов'
set ylabel 'Время чтения элемента'
set grid

set xtics 2
set ytics 2

set style data lines

# Построение графиков из файлов
plot 'l1.txt' using 1:2 with lines title 'L3' linestyle 1  linecolor rgb "blue" lw 2, \
#   'l2.txt' using 1:2 with lines title 'L2'  linestyle 2 linecolor rgb "green" lw 2, \
#   l3.txt' using 1:2 with lines title 'L3' linestyle 3 linecolor rgb "red" lw 2
# Сохранение и выход
set output
