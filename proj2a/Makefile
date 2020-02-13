#NAME: Senyang Jiang
#EMAIL: senyangjiang@yahoo.com
#ID: 505111806

ID=505111806
LAB=lab2a
OUTPUT = lab2_add.csv lab2_list.csv
GRAPHS = lab2_add-1.png lab2_add-2.png lab2_add-3.png lab2_add-4.png lab2_add-5.png lab2_list-1.png lab2_list-2.png lab2_list-3.png lab2_list-4.png
SCRIPTS = lab2_add.gp lab2_list.gp
TARBALL=$(LAB)-$(ID).tar.gz

build:
	@gcc -Wall -Wextra lab2_add.c -o lab2_add -pthread
	@gcc -Wall -Wextra lab2_list.c SortedList.c -o lab2_list -pthread
tests: build
#without yield
	@ > lab2_add.csv
	@./lab2_add --threads=2 --iterations=100 >> lab2_add.csv
	@./lab2_add --threads=2 --iterations=1000 >> lab2_add.csv
	@./lab2_add --threads=2 --iterations=10000 >> lab2_add.csv
	@./lab2_add --threads=2 --iterations=100000 >> lab2_add.csv
	@./lab2_add --threads=4 --iterations=100 >> lab2_add.csv
	@./lab2_add --threads=4 --iterations=1000 >> lab2_add.csv
	@./lab2_add --threads=4 --iterations=10000 >> lab2_add.csv
	@./lab2_add --threads=4 --iterations=100000 >> lab2_add.csv
	@./lab2_add --threads=8 --iterations=100 >> lab2_add.csv
	@./lab2_add --threads=8 --iterations=1000 >> lab2_add.csv
	@./lab2_add --threads=8 --iterations=10000 >> lab2_add.csv
	@./lab2_add --threads=8 --iterations=100000 >> lab2_add.csv
	@./lab2_add --threads=12 --iterations=100 >> lab2_add.csv
	@./lab2_add --threads=12 --iterations=1000 >> lab2_add.csv
	@./lab2_add --threads=12 --iterations=10000 >> lab2_add.csv
	@./lab2_add --threads=12 --iterations=100000 >> lab2_add.csv
#yield
	@./lab2_add --threads=2 --iterations=10 --yield >> lab2_add.csv
	@./lab2_add --threads=2 --iterations=20 --yield >> lab2_add.csv
	@./lab2_add --threads=2 --iterations=40 --yield >> lab2_add.csv
	@./lab2_add --threads=2 --iterations=80 --yield >> lab2_add.csv
	@./lab2_add --threads=2 --iterations=100 --yield >> lab2_add.csv
	@./lab2_add --threads=2 --iterations=1000 --yield >> lab2_add.csv
	@./lab2_add --threads=2 --iterations=10000 --yield >> lab2_add.csv
	@./lab2_add --threads=2 --iterations=100000 --yield >> lab2_add.csv
	@./lab2_add --threads=4 --iterations=10 --yield >> lab2_add.csv
	@./lab2_add --threads=4 --iterations=20 --yield >> lab2_add.csv
	@./lab2_add --threads=4 --iterations=40 --yield >> lab2_add.csv
	@./lab2_add --threads=4 --iterations=80 --yield >> lab2_add.csv
	@./lab2_add --threads=4 --iterations=100 --yield >> lab2_add.csv
	@./lab2_add --threads=4 --iterations=1000 --yield >> lab2_add.csv
	@./lab2_add --threads=4 --iterations=10000 --yield >> lab2_add.csv
	@./lab2_add --threads=4 --iterations=100000 --yield >> lab2_add.csv
	@./lab2_add --threads=8 --iterations=10 --yield >> lab2_add.csv
	@./lab2_add --threads=8 --iterations=20 --yield >> lab2_add.csv
	@./lab2_add --threads=8 --iterations=40 --yield >> lab2_add.csv
	@./lab2_add --threads=8 --iterations=80 --yield >> lab2_add.csv
	@./lab2_add --threads=8 --iterations=100 --yield >> lab2_add.csv
	@./lab2_add --threads=8 --iterations=1000 --yield >> lab2_add.csv
	@./lab2_add --threads=8 --iterations=10000 --yield >> lab2_add.csv
	@./lab2_add --threads=8 --iterations=100000 --yield >> lab2_add.csv
	@./lab2_add --threads=12 --iterations=10 --yield >> lab2_add.csv
	@./lab2_add --threads=12 --iterations=20 --yield >> lab2_add.csv
	@./lab2_add --threads=12 --iterations=40 --yield >> lab2_add.csv
	@./lab2_add --threads=12 --iterations=80 --yield >> lab2_add.csv
	@./lab2_add --threads=12 --iterations=100 --yield >> lab2_add.csv
	@./lab2_add --threads=12 --iterations=1000 --yield >> lab2_add.csv
	@./lab2_add --threads=12 --iterations=10000 --yield >> lab2_add.csv
	@./lab2_add --threads=12 --iterations=100000 --yield >> lab2_add.csv
#plotting cost per operation vs. number of operations
	@./lab2_add --threads=1 --iterations=100 >> lab2_add.csv
	@./lab2_add --threads=1 --iterations=1000 >> lab2_add.csv
	@./lab2_add --threads=1 --iterations=10000 >> lab2_add.csv
	@./lab2_add --threads=1 --iterations=100000 >> lab2_add.csv
	@./lab2_add --threads=1 --iterations=1000000 >> lab2_add.csv
	@./lab2_add --threads=2 --iterations=100 >> lab2_add.csv
	@./lab2_add --threads=2 --iterations=1000 >> lab2_add.csv
	@./lab2_add --threads=2 --iterations=10000 >> lab2_add.csv
	@./lab2_add --threads=2 --iterations=100000 >> lab2_add.csv
	@./lab2_add --threads=2 --iterations=1000000 >> lab2_add.csv
	@./lab2_add --threads=4 --iterations=100 >> lab2_add.csv
	@./lab2_add --threads=4 --iterations=1000 >> lab2_add.csv
	@./lab2_add --threads=4 --iterations=10000 >> lab2_add.csv
	@./lab2_add --threads=4 --iterations=100000 >> lab2_add.csv
	@./lab2_add --threads=4 --iterations=1000000 >> lab2_add.csv

	@./lab2_add --threads=1 --iterations=100 --yield >> lab2_add.csv
	@./lab2_add --threads=1 --iterations=1000 --yield >> lab2_add.csv
	@./lab2_add --threads=1 --iterations=10000 --yield >> lab2_add.csv
	@./lab2_add --threads=1 --iterations=100000 --yield >> lab2_add.csv
	@./lab2_add --threads=1 --iterations=1000000 --yield >> lab2_add.csv
	@./lab2_add --threads=2 --iterations=100 --yield >> lab2_add.csv
	@./lab2_add --threads=2 --iterations=1000 --yield >> lab2_add.csv
	@./lab2_add --threads=2 --iterations=10000 --yield >> lab2_add.csv
	@./lab2_add --threads=2 --iterations=100000 --yield >> lab2_add.csv
	@./lab2_add --threads=2 --iterations=1000000 --yield >> lab2_add.csv
	@./lab2_add --threads=4 --iterations=100 --yield >> lab2_add.csv
	@./lab2_add --threads=4 --iterations=1000 --yield >> lab2_add.csv
	@./lab2_add --threads=4 --iterations=10000 --yield >> lab2_add.csv
	@./lab2_add --threads=4 --iterations=100000 --yield >> lab2_add.csv
	@./lab2_add --threads=4 --iterations=1000000 --yield >> lab2_add.csv
#confirm that synchronization eliminate race conditions
	@./lab2_add --threads=2 --iterations=10000 --yield --sync=m >> lab2_add.csv
	@./lab2_add --threads=4 --iterations=10000 --yield --sync=m >> lab2_add.csv
	@./lab2_add --threads=8 --iterations=10000 --yield --sync=m >> lab2_add.csv
	@./lab2_add --threads=12 --iterations=10000 --yield --sync=m >> lab2_add.csv
	@./lab2_add --threads=2 --iterations=1000 --yield --sync=s >> lab2_add.csv
	@./lab2_add --threads=4 --iterations=1000 --yield --sync=s >> lab2_add.csv
	@./lab2_add --threads=8 --iterations=1000 --yield --sync=s >> lab2_add.csv
	@./lab2_add --threads=12 --iterations=1000 --yield --sync=s >> lab2_add.csv
	@./lab2_add --threads=2 --iterations=10000 --yield --sync=c >> lab2_add.csv
	@./lab2_add --threads=4 --iterations=10000 --yield --sync=c >> lab2_add.csv
	@./lab2_add --threads=8 --iterations=10000 --yield --sync=c >> lab2_add.csv
	@./lab2_add --threads=12 --iterations=10000 --yield --sync=c >> lab2_add.csv
# test all four (no yield) versions for a range of number of threads (1,2,4,8,12)
	@./lab2_add --threads=1 --iterations=10000 >> lab2_add.csv
	@./lab2_add --threads=2 --iterations=10000 >> lab2_add.csv
	@./lab2_add --threads=4 --iterations=10000 >> lab2_add.csv
	@./lab2_add --threads=8 --iterations=10000 >> lab2_add.csv
	@./lab2_add --threads=12 --iterations=10000 >> lab2_add.csv
	@./lab2_add --threads=1 --iterations=10000 --sync=m >> lab2_add.csv
	@./lab2_add --threads=2 --iterations=10000 --sync=m >> lab2_add.csv
	@./lab2_add --threads=4 --iterations=10000 --sync=m >> lab2_add.csv
	@./lab2_add --threads=8 --iterations=10000 --sync=m >> lab2_add.csv
	@./lab2_add --threads=12 --iterations=10000 --sync=m >> lab2_add.csv
	@./lab2_add --threads=1 --iterations=10000 --sync=s >> lab2_add.csv
	@./lab2_add --threads=2 --iterations=10000 --sync=s >> lab2_add.csv
	@./lab2_add --threads=4 --iterations=10000 --sync=s >> lab2_add.csv
	@./lab2_add --threads=8 --iterations=10000 --sync=s >> lab2_add.csv
	@./lab2_add --threads=12 --iterations=10000 --sync=s >> lab2_add.csv
	@./lab2_add --threads=1 --iterations=10000 --sync=c >> lab2_add.csv
	@./lab2_add --threads=2 --iterations=10000 --sync=c >> lab2_add.csv
	@./lab2_add --threads=4 --iterations=10000 --sync=c >> lab2_add.csv
	@./lab2_add --threads=8 --iterations=10000 --sync=c >> lab2_add.csv
	@./lab2_add --threads=12 --iterations=10000 --sync=c >> lab2_add.csv
#time per operation vs number of iterations(lab2_list-1.png)
	@ > lab2_list.csv
	@./lab2_list --threads=1 --iterations=10 >> lab2_list.csv
	@./lab2_list --threads=1 --iterations=100 >> lab2_list.csv
	@./lab2_list --threads=1 --iterations=1000 >> lab2_list.csv
	@./lab2_list --threads=1 --iterations=10000 >> lab2_list.csv
	@./lab2_list --threads=1 --iterations=20000 >> lab2_list.csv
#conflicts without yield(lab2_list-2.png)
	@-./lab2_list --threads=2 --iterations=1 >> lab2_list.csv
	@-./lab2_list --threads=2 --iterations=10 >> lab2_list.csv
	@-./lab2_list --threads=2 --iterations=100 >> lab2_list.csv
	@-./lab2_list --threads=2 --iterations=1000 >> lab2_list.csv
	@-./lab2_list --threads=4 --iterations=1 >> lab2_list.csv
	@-./lab2_list --threads=4 --iterations=10 >> lab2_list.csv
	@-./lab2_list --threads=4 --iterations=100 >> lab2_list.csv
	@-./lab2_list --threads=4 --iterations=1000 >> lab2_list.csv
	@-./lab2_list --threads=8 --iterations=1 >> lab2_list.csv
	@-./lab2_list --threads=8 --iterations=10 >> lab2_list.csv
	@-./lab2_list --threads=8 --iterations=100 >> lab2_list.csv
	@-./lab2_list --threads=8 --iterations=1000 >> lab2_list.csv
	@-./lab2_list --threads=12 --iterations=1 >> lab2_list.csv
	@-./lab2_list --threads=12 --iterations=10 >> lab2_list.csv
	@-./lab2_list --threads=12 --iterations=100 >> lab2_list.csv
	@-./lab2_list --threads=12 --iterations=1000 >> lab2_list.csv
#conflicts inserts, deletes, inserts + look ups, look ups + deletes,  with yields(lab2_list-2.png)
	@-./lab2_list --threads=2 --iterations=1 --yield=i >> lab2_list.csv
	@-./lab2_list --threads=2 --iterations=2 --yield=i >> lab2_list.csv
	@-./lab2_list --threads=2 --iterations=4 --yield=i >> lab2_list.csv
	@-./lab2_list --threads=2 --iterations=8 --yield=i >> lab2_list.csv
	@-./lab2_list --threads=2 --iterations=16 --yield=i >> lab2_list.csv
	@-./lab2_list --threads=2 --iterations=32 --yield=i >> lab2_list.csv
	@-./lab2_list --threads=4 --iterations=1 --yield=i >> lab2_list.csv
	@-./lab2_list --threads=4 --iterations=2 --yield=i >> lab2_list.csv
	@-./lab2_list --threads=4 --iterations=4 --yield=i >> lab2_list.csv
	@-./lab2_list --threads=4 --iterations=8 --yield=i >> lab2_list.csv
	@-./lab2_list --threads=4 --iterations=16 --yield=i >> lab2_list.csv
	@-./lab2_list --threads=4 --iterations=32 --yield=i >> lab2_list.csv
	@-./lab2_list --threads=8 --iterations=1 --yield=i >> lab2_list.csv
	@-./lab2_list --threads=8 --iterations=2 --yield=i >> lab2_list.csv
	@-./lab2_list --threads=8 --iterations=4 --yield=i >> lab2_list.csv
	@-./lab2_list --threads=8 --iterations=8 --yield=i >> lab2_list.csv
	@-./lab2_list --threads=8 --iterations=16 --yield=i >> lab2_list.csv
	@-./lab2_list --threads=8 --iterations=32 --yield=i >> lab2_list.csv
	@-./lab2_list --threads=12 --iterations=1 --yield=i >> lab2_list.csv
	@-./lab2_list --threads=12 --iterations=2 --yield=i >> lab2_list.csv
	@-./lab2_list --threads=12 --iterations=4 --yield=i >> lab2_list.csv
	@-./lab2_list --threads=12 --iterations=8 --yield=i >> lab2_list.csv
	@-./lab2_list --threads=12 --iterations=16 --yield=i >> lab2_list.csv
	@-./lab2_list --threads=12 --iterations=32 --yield=i >> lab2_list.csv

	@-./lab2_list --threads=2 --iterations=1 --yield=d >> lab2_list.csv
	@-./lab2_list --threads=2 --iterations=2 --yield=d >> lab2_list.csv
	@-./lab2_list --threads=2 --iterations=4 --yield=d >> lab2_list.csv
	@-./lab2_list --threads=2 --iterations=8 --yield=d >> lab2_list.csv
	@-./lab2_list --threads=2 --iterations=16 --yield=d >> lab2_list.csv
	@-./lab2_list --threads=2 --iterations=32 --yield=d >> lab2_list.csv
	@-./lab2_list --threads=4 --iterations=1 --yield=d >> lab2_list.csv
	@-./lab2_list --threads=4 --iterations=2 --yield=d >> lab2_list.csv
	@-./lab2_list --threads=4 --iterations=4 --yield=d >> lab2_list.csv
	@-./lab2_list --threads=4 --iterations=8 --yield=d >> lab2_list.csv
	@-./lab2_list --threads=4 --iterations=16 --yield=d >> lab2_list.csv
	@-./lab2_list --threads=4 --iterations=32 --yield=d >> lab2_list.csv
	@-./lab2_list --threads=8 --iterations=1 --yield=d >> lab2_list.csv
	@-./lab2_list --threads=8 --iterations=2 --yield=d >> lab2_list.csv
	@-./lab2_list --threads=8 --iterations=4 --yield=d >> lab2_list.csv
	@-./lab2_list --threads=8 --iterations=8 --yield=d >> lab2_list.csv
	@-./lab2_list --threads=8 --iterations=16 --yield=d >> lab2_list.csv
	@-./lab2_list --threads=8 --iterations=32 --yield=d >> lab2_list.csv
	@-./lab2_list --threads=12 --iterations=1 --yield=d >> lab2_list.csv
	@-./lab2_list --threads=12 --iterations=2 --yield=d >> lab2_list.csv
	@-./lab2_list --threads=12 --iterations=4 --yield=d >> lab2_list.csv
	@-./lab2_list --threads=12 --iterations=8 --yield=d >> lab2_list.csv
	@-./lab2_list --threads=12 --iterations=16 --yield=d >> lab2_list.csv
	@-./lab2_list --threads=12 --iterations=32 --yield=d >> lab2_list.csv

	@-./lab2_list --threads=2 --iterations=1 --yield=il >> lab2_list.csv
	@-./lab2_list --threads=2 --iterations=2 --yield=il >> lab2_list.csv
	@-./lab2_list --threads=2 --iterations=4 --yield=il >> lab2_list.csv
	@-./lab2_list --threads=2 --iterations=8 --yield=il >> lab2_list.csv
	@-./lab2_list --threads=2 --iterations=16 --yield=il >> lab2_list.csv
	@-./lab2_list --threads=2 --iterations=32 --yield=il >> lab2_list.csv
	@-./lab2_list --threads=4 --iterations=1 --yield=il >> lab2_list.csv
	@-./lab2_list --threads=4 --iterations=2 --yield=il >> lab2_list.csv
	@-./lab2_list --threads=4 --iterations=4 --yield=il >> lab2_list.csv
	@-./lab2_list --threads=4 --iterations=8 --yield=il >> lab2_list.csv
	@-./lab2_list --threads=4 --iterations=16 --yield=il >> lab2_list.csv
	@-./lab2_list --threads=4 --iterations=32 --yield=il >> lab2_list.csv
	@-./lab2_list --threads=8 --iterations=1 --yield=il >> lab2_list.csv
	@-./lab2_list --threads=8 --iterations=2 --yield=il >> lab2_list.csv
	@-./lab2_list --threads=8 --iterations=4 --yield=il >> lab2_list.csv
	@-./lab2_list --threads=8 --iterations=8 --yield=il >> lab2_list.csv
	@-./lab2_list --threads=8 --iterations=16 --yield=il >> lab2_list.csv
	@-./lab2_list --threads=8 --iterations=32 --yield=il >> lab2_list.csv
	@-./lab2_list --threads=12 --iterations=1 --yield=il >> lab2_list.csv
	@-./lab2_list --threads=12 --iterations=2 --yield=il >> lab2_list.csv
	@-./lab2_list --threads=12 --iterations=4 --yield=il >> lab2_list.csv
	@-./lab2_list --threads=12 --iterations=8 --yield=il >> lab2_list.csv
	@-./lab2_list --threads=12 --iterations=16 --yield=il >> lab2_list.csv
	@-./lab2_list --threads=12 --iterations=32 --yield=il >> lab2_list.csv

	@-./lab2_list --threads=2 --iterations=1 --yield=dl >> lab2_list.csv
	@-./lab2_list --threads=2 --iterations=2 --yield=dl >> lab2_list.csv
	@-./lab2_list --threads=2 --iterations=4 --yield=dl >> lab2_list.csv
	@-./lab2_list --threads=2 --iterations=8 --yield=dl >> lab2_list.csv
	@-./lab2_list --threads=2 --iterations=16 --yield=dl >> lab2_list.csv
	@-./lab2_list --threads=2 --iterations=32 --yield=dl >> lab2_list.csv
	@-./lab2_list --threads=4 --iterations=1 --yield=dl >> lab2_list.csv
	@-./lab2_list --threads=4 --iterations=2 --yield=dl >> lab2_list.csv
	@-./lab2_list --threads=4 --iterations=4 --yield=dl >> lab2_list.csv
	@-./lab2_list --threads=4 --iterations=8 --yield=dl >> lab2_list.csv
	@-./lab2_list --threads=4 --iterations=16 --yield=dl >> lab2_list.csv
	@-./lab2_list --threads=4 --iterations=32 --yield=dl >> lab2_list.csv
	@-./lab2_list --threads=8 --iterations=1 --yield=dl >> lab2_list.csv
	@-./lab2_list --threads=8 --iterations=2 --yield=dl >> lab2_list.csv
	@-./lab2_list --threads=8 --iterations=4 --yield=dl >> lab2_list.csv
	@-./lab2_list --threads=8 --iterations=8 --yield=dl >> lab2_list.csv
	@-./lab2_list --threads=8 --iterations=16 --yield=dl >> lab2_list.csv
	@-./lab2_list --threads=8 --iterations=32 --yield=dl >> lab2_list.csv
	@-./lab2_list --threads=12 --iterations=1 --yield=dl >> lab2_list.csv
	@-./lab2_list --threads=12 --iterations=2 --yield=dl >> lab2_list.csv
	@-./lab2_list --threads=12 --iterations=4 --yield=dl >> lab2_list.csv
	@-./lab2_list --threads=12 --iterations=8 --yield=dl >> lab2_list.csv
	@-./lab2_list --threads=12 --iterations=16 --yield=dl >> lab2_list.csv
	@-./lab2_list --threads=12 --iterations=32 --yield=dl >> lab2_list.csv
#successful runs vs the number of iterations for mutex and spin-lock protection with each of the above four yield combinations(lab2_list-3.png)
	@./lab2_list --threads=12 --iterations=32 --yield=i  --sync=m >> lab2_list.csv
	@./lab2_list --threads=12 --iterations=32 --yield=d  --sync=m >> lab2_list.csv
	@./lab2_list --threads=12 --iterations=32 --yield=il --sync=m >> lab2_list.csv
	@./lab2_list --threads=12 --iterations=32 --yield=dl --sync=m >> lab2_list.csv
	@./lab2_list --threads=12 --iterations=32 --yield=i  --sync=s >> lab2_list.csv
	@./lab2_list --threads=12 --iterations=32 --yield=d  --sync=s >> lab2_list.csv
	@./lab2_list --threads=12 --iterations=32 --yield=il --sync=s >> lab2_list.csv
	@./lab2_list --threads=12 --iterations=32 --yield=dl --sync=s >> lab2_list.csv
#the (corrected for list length) per operation times (for each of the synchronization options: mutex, spin) vs the number of threads(lab2_list-4.png)
	@./lab2_list --threads=1 --iterations=1000 --sync=m >> lab2_list.csv
	@./lab2_list --threads=2 --iterations=1000 --sync=m >> lab2_list.csv
	@./lab2_list --threads=4 --iterations=1000 --sync=m >> lab2_list.csv
	@./lab2_list --threads=8 --iterations=1000 --sync=m >> lab2_list.csv
	@./lab2_list --threads=12 --iterations=1000 --sync=m >> lab2_list.csv
	@./lab2_list --threads=16 --iterations=1000 --sync=m >> lab2_list.csv
	@./lab2_list --threads=24 --iterations=1000 --sync=m >> lab2_list.csv
	@./lab2_list --threads=1 --iterations=1000 --sync=s >> lab2_list.csv
	@./lab2_list --threads=2 --iterations=1000 --sync=s >> lab2_list.csv
	@./lab2_list --threads=4 --iterations=1000 --sync=s >> lab2_list.csv
	@./lab2_list --threads=8 --iterations=1000 --sync=s >> lab2_list.csv
	@./lab2_list --threads=12 --iterations=1000 --sync=s >> lab2_list.csv
	@./lab2_list --threads=16 --iterations=1000 --sync=s >> lab2_list.csv
	@./lab2_list --threads=24 --iterations=1000 --sync=s >> lab2_list.csv

graphs: tests
	@./lab2_add.gp
	@./lab2_list.gp

dist: graphs
	@tar -czf $(TARBALL) README Makefile *.c *.h $(OUTPUT) $(GRAPHS) $(SCRIPTS)

clean:
	@rm -f lab2_add lab2_list lab2a-505111806.tar.gz
