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
plot "log_r1" using ($1/100):2 title "valores medidos" with lines linestyle 2, \
"log_r2" using ($1/100):2 title '' with lines linestyle 2, \
"log_r3" using ($1/100):2 title '' with lines linestyle 2, \
"log_r4" using ($1/100):2 title '' with lines linestyle 2, \
"log_r5" using ($1/100):2 title '' with lines linestyle 2, \
"log_r6" using ($1/100):2 title '' with lines linestyle 2, \
"log_r7" using ($1/100):2 title '' with lines linestyle 2, \
"log_r8" using ($1/100):2 title '' with lines linestyle 2, \
"log_r9" using ($1/100):2 title '' with lines linestyle 2, \
"log_r10" using ($1/100):2 title '' with lines linestyle 2, \
"log_r11" using ($1/100):2 title '' with lines linestyle 2, \
"log_r12" using ($1/100):2 title '' with lines linestyle 2, \
"log_r13" using ($1/100):2 title '' with lines linestyle 2, \
"log_r14" using ($1/100):2 title '' with lines linestyle 2, \
"log_r15" using ($1/100):2 title '' with lines linestyle 2, \
"log_r16" using ($1/100):2 title '' with lines linestyle 2, \
"log_r17" using ($1/100):2 title '' with lines linestyle 2, \
"log_r18" using ($1/100):2 title '' with lines linestyle 2, \
"log_r19" using ($1/100):2 title '' with lines linestyle 2, \
"log_r20" using ($1/100):2 title '' with lines linestyle 2, \
"log_r21" using ($1/100):2 title '' with lines linestyle 2, \
"log_r22" using ($1/100):2 title '' with lines linestyle 2, \
"log_r23" using ($1/100):2 title '' with lines linestyle 2, \
"log_r24" using ($1/100):2 title '' with lines linestyle 2, \
"log_r25" using ($1/100):2 title '' with lines linestyle 2, \
"log_r26" using ($1/100):2 title '' with lines linestyle 2, \
"log_r27" using ($1/100):2 title '' with lines linestyle 2, \
"log_r28" using ($1/100):2 title '' with lines linestyle 2, \
"log_r29" using ($1/100):2 title '' with lines linestyle 2, \
"log_r30" using ($1/100):2 title '' with lines linestyle 2, \
"log_r31" using ($1/100):2 title '' with lines linestyle 2, \
"log_r32" using ($1/100):2 title '' with lines linestyle 2, \
"log_r33" using ($1/100):2 title '' with lines linestyle 2, \
"log_r34" using ($1/100):2 title '' with lines linestyle 2, \
"log_r35" using ($1/100):2 title '' with lines linestyle 2, \
"log_r36" using ($1/100):2 title '' with lines linestyle 2, \
"log_r37" using ($1/100):2 title '' with lines linestyle 2, \
"log_r38" using ($1/100):2 title '' with lines linestyle 2, \
"log_r39" using ($1/100):2 title '' with lines linestyle 2, \
"log_r40" using ($1/100):2 title '' with lines linestyle 2, \
"log_r41" using ($1/100):2 title '' with lines linestyle 2, \
"log_r42" using ($1/100):2 title '' with lines linestyle 2, \
"log_r43" using ($1/100):2 title '' with lines linestyle 2, \
"log_r44" using ($1/100):2 title '' with lines linestyle 2, \
"log_r45" using ($1/100):2 title '' with lines linestyle 2, \
"log_r46" using ($1/100):2 title '' with lines linestyle 2, \
"log_r47" using ($1/100):2 title '' with lines linestyle 2, \
"log_r48" using ($1/100):2 title '' with lines linestyle 2, \
"log_r49" using ($1/100):2 title '' with lines linestyle 2, \
"log_r50" using ($1/100):2 title '' with lines linestyle 2, \
"log_r51" using ($1/100):2 title '' with lines linestyle 2, \
"log_r52" using ($1/100):2 title '' with lines linestyle 2, \
"log_r53" using ($1/100):2 title '' with lines linestyle 2, \
"log_r54" using ($1/100):2 title '' with lines linestyle 2, \
"log_r55" using ($1/100):2 title '' with lines linestyle 2, \
"log_r56" using ($1/100):2 title '' with lines linestyle 2, \
"log_r57" using ($1/100):2 title '' with lines linestyle 2, \
"log_r58" using ($1/100):2 title '' with lines linestyle 2, \
"log_r59" using ($1/100):2 title '' with lines linestyle 2, \
"log_r60" using ($1/100):2 title '' with lines linestyle 2, \
"log_r61" using ($1/100):2 title '' with lines linestyle 2, \
"log_r62" using ($1/100):2 title '' with lines linestyle 2, \
"log_r63" using ($1/100):2 title '' with lines linestyle 2, \
"log_r64" using ($1/100):2 title '' with lines linestyle 2, \
"log_r65" using ($1/100):2 title '' with lines linestyle 2, \
"log_r66" using ($1/100):2 title '' with lines linestyle 2, \
"log_r67" using ($1/100):2 title '' with lines linestyle 2, \
"log_r68" using ($1/100):2 title '' with lines linestyle 2, \
"log_r69" using ($1/100):2 title '' with lines linestyle 2, \
"log_r70" using ($1/100):2 title '' with lines linestyle 2, \
"log_r71" using ($1/100):2 title '' with lines linestyle 2, \
"log_r72" using ($1/100):2 title '' with lines linestyle 2, \
"log_r73" using ($1/100):2 title '' with lines linestyle 2, \
"log_r74" using ($1/100):2 title '' with lines linestyle 2, \
"log_r75" using ($1/100):2 title '' with lines linestyle 2, \
"log_r76" using ($1/100):2 title '' with lines linestyle 2, \
"log_r77" using ($1/100):2 title '' with lines linestyle 2, \
"log_r78" using ($1/100):2 title '' with lines linestyle 2, \
"log_r79" using ($1/100):2 title '' with lines linestyle 2, \
"log_r80" using ($1/100):2 title '' with lines linestyle 2, \
"log_r81" using ($1/100):2 title '' with lines linestyle 2, \
"log_r82" using ($1/100):2 title '' with lines linestyle 2, \
"log_r83" using ($1/100):2 title '' with lines linestyle 2, \
"log_r84" using ($1/100):2 title '' with lines linestyle 2, \
"log_r85" using ($1/100):2 title '' with lines linestyle 2, \
"log_r86" using ($1/100):2 title '' with lines linestyle 2, \
"log_r87" using ($1/100):2 title '' with lines linestyle 2, \
"log_r88" using ($1/100):2 title '' with lines linestyle 2, \
"log_r89" using ($1/100):2 title '' with lines linestyle 2, \
"log_r90" using ($1/100):2 title '' with lines linestyle 2, \
"log_r91" using ($1/100):2 title '' with lines linestyle 2, \
"log_r92" using ($1/100):2 title '' with lines linestyle 2, \
"log_r93" using ($1/100):2 title '' with lines linestyle 2, \
"log_r94" using ($1/100):2 title '' with lines linestyle 2, \
"log_r95" using ($1/100):2 title '' with lines linestyle 2, \
"log_r96" using ($1/100):2 title '' with lines linestyle 2, \
"log_r97" using ($1/100):2 title '' with lines linestyle 2, \
"log_r98" using ($1/100):2 title '' with lines linestyle 2, \
"log_r99" using ($1/100):2 title '' with lines linestyle 2, \
"log_r100" using ($1/100):2 title '' with lines linestyle 2, \
f(x) title 'valor teórico' with lines linestyle 1 
