set title "MM1 - Espera na Fila"
set xlabel "Fator de Utilizacao do Link"
set ylabel "Tempo Médio de Espera(s)"
set grid
mi = 0.0008 
f(x) = (x * mi) / (2 * (1 - x))
set style line 1 linecolor rgb '#0060ad' linetype 1 linewidth 2
set style line 2 linecolor rgb '#dd181f' linetype 1 linewidth 2
set xrange [.01:.99]
plot f(x) title 'valor teórico' with lines linestyle 1, "waiting.log" title 'valor medido' with lines linestyle 2