set title "Espera na Fila M/D/1"
set xlabel "Fator de Utilização do Link"
set ylabel "Tempo Médio de Espera(s)"
set grid
mu = 0.0008 
f(x) = (x * mu) / (2 * (1 - x))
mi = 1250
g(x) = x / (2 * mi * (1 - x))
set style line 1 linecolor rgb "#00000000" linetype 1 linewidth 2
set style line 2 linecolor rgb "#22ffa500" linetype 1 linewidth 2
#set style line 2 linecolor rgb "#22bbbbbb" linetype 1 linewidth 2
set xrange [.01:1]
stats_file1 = "estats.log"
stats_file2 = "estats_sqrtN.log"
stats_file3 = "estats_t.log"
stats_file4 = "estats_sqrtN_t.log"
plot \
f(x) title 'valor teórico' with lines linestyle 1, \
stats_file4  using 1:2:4:5 title 'intervalo de confianca com sqrt(n) t student' with yerrorbar ls 1, \
#"50.log7" using  ($1/100):2  title "valores medidos" with points ls 2, \
#"80.log7" using  ($1/100):2 notitle with points ls 2, \
#"90.log7" using  ($1/100):2 notitle with points ls 2, \
#"95.log7" using  ($1/100):2 notitle with points ls 2
#stats_file1  using 1:2:4:5 title 'intervalo de confianca' with yerrorbar ls 1, \
#stats_file2  using 1:2:4:5 title 'intervalo de confianca com sqrt(n) normal' with yerrorbar ls 1, \
#stats_file3  using 1:2:4:5 title 'intervalo de confianca t student' with yerrorbar ls 2, \
#stats_file4  using 1:2:4:5 title 'intervalo de confianca com sqrt(n) t student' with yerrorbar ls 1, \
