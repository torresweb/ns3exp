set title "Espera na Fila M/D/1"
set xlabel "Fator de Utilização do Link"
set ylabel "Tempo Médio de Espera(s)"
set grid
mu = 0.0008 
f(x) = (x * mu) / (2 * (1 - x))
mi = 1250
g(x) = x / (2 * mi * (1 - x))
set style line 1 linecolor rgb '#0060ad' linetype 1 linewidth 2
set style line 2 linecolor rgb '#cc181f' linetype 1 linewidth 2
set style line 3 linecolor rgb '#ff018ee' linetype 1 linewidth 2
set style line 4 lc rgb '#cc181f' pt 7 ps 1.5 lt 1 lw 1 
set xrange [.01:.99]
#plot f(x) title 'valor teórico' with lines linestyle 1, g(x) with lines linestyle 2, "waiting.log" title 'valor medido' with lines linestyle 3
plot f(x) title 'valor teórico' with lines linestyle 1, "waiting.log" title 'valor medido' with lines linestyle 2
#plot f(x) title 'valor teórico' with lines linestyle 1, "waiting_partial.log" title 'valor medido' w p ls 4
# 0,9/((2*1250)*(1-0,9))
# 0,9*0,0008/(2*(1-0,9))