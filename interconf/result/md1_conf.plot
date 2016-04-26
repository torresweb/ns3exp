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
plot \
f(x) title 'valor teórico' with lines linestyle 1, \
"estats.log"  using 1:5:4:5 title 'intervalo de confianca' with yerrorbar ls 1, \
"estats.log"  using 1:2      title 'medias das simulacoes' with point ls 3, \
"50.log7" using  ($1/100):2  title "valores medidos" with points ls 6, \
"80.log7" using  ($1/100):2 notitle with points ls 6, \
"90.log7" using  ($1/100):2 notitle with points ls 6, \
"95.log7" using  ($1/100):2 notitle with points ls 6
