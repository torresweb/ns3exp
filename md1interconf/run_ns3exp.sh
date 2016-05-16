#/bin/bash

# u = fator utilização
# r = run seed

rm -f *.log7
rm -f estats*.log

for u in `echo 50 80 90 95`; do 
	for r in `seq 1 7`; do 
		/home/emula/ns3/build/scratch/ns3exp//ns3exp --u=$u --r=$r >> $u.log7; 
	done; 
	./gera_stats.py $u.log7 1 0 >> estats_sqrtN.log
	./gera_stats.py $u.log7 0 0 >> estats.log
	./gera_stats.py $u.log7 1 1 >> estats_sqrtN_t.log
	./gera_stats.py $u.log7 0 1 >> estats_t.log
done




