#NAME: Senyang Jiang
#EMAIL: senyangjiang@yahoo.com
#ID: 505111806

ID=505111806
LAB=lab2b
OUTPUT = lab2b_list.csv profile.out
GRAPHS = lab2b_1.png lab2b_2.png lab2b_3.png lab2b_4.png lab2b_5.png
SCRIPTS = lab2_list.gp
TARBALL=$(LAB)-$(ID).tar.gz

build:
	@gcc -Wall -Wextra lab2_list.c SortedList.c -o lab2_list -pthread -g -lprofiler
tests: build
#the total number of operations per second for each synchronization method (lab2b_1.png)
#wait-for-lock time, and average time per operation against the number of competing threads for mutex (lab2b_2.png)
	@ > lab2b_list.csv
	@./lab2_list --threads=1 --iterations=1000 --sync=m >> lab2b_list.csv
	@./lab2_list --threads=2 --iterations=1000 --sync=m >> lab2b_list.csv
	@./lab2_list --threads=4 --iterations=1000 --sync=m >> lab2b_list.csv
	@./lab2_list --threads=8 --iterations=1000 --sync=m >> lab2b_list.csv
	@./lab2_list --threads=12 --iterations=1000 --sync=m >> lab2b_list.csv
	@./lab2_list --threads=16 --iterations=1000 --sync=m >> lab2b_list.csv
	@./lab2_list --threads=24 --iterations=1000 --sync=m >> lab2b_list.csv
	@./lab2_list --threads=1 --iterations=1000 --sync=s >> lab2b_list.csv
	@./lab2_list --threads=2 --iterations=1000 --sync=s >> lab2b_list.csv
	@./lab2_list --threads=4 --iterations=1000 --sync=s >> lab2b_list.csv
	@./lab2_list --threads=8 --iterations=1000 --sync=s >> lab2b_list.csv
	@./lab2_list --threads=12 --iterations=1000 --sync=s >> lab2b_list.csv
	@./lab2_list --threads=16 --iterations=1000 --sync=s >> lab2b_list.csv
	@./lab2_list --threads=24 --iterations=1000 --sync=s >> lab2b_list.csv

#Confirm the correctness of your new implementation (lab2b_3.png)
	@./lab2_list --threads=1 --iterations=1 --lists=4 --yield=id >> lab2b_list.csv
	@./lab2_list --threads=1 --iterations=2 --lists=4 --yield=id >> lab2b_list.csv
	@./lab2_list --threads=1 --iterations=4 --lists=4 --yield=id >> lab2b_list.csv
	@./lab2_list --threads=1 --iterations=8 --lists=4 --yield=id >> lab2b_list.csv
	@./lab2_list --threads=1 --iterations=16 --lists=4 --yield=id >> lab2b_list.csv
	@-./lab2_list --threads=4 --iterations=1 --lists=4 --yield=id >> lab2b_list.csv
	@-./lab2_list --threads=4 --iterations=2 --lists=4 --yield=id >> lab2b_list.csv
	@-./lab2_list --threads=4 --iterations=4 --lists=4 --yield=id >> lab2b_list.csv
	@-./lab2_list --threads=4 --iterations=8 --lists=4 --yield=id >> lab2b_list.csv
	@-./lab2_list --threads=4 --iterations=16 --lists=4 --yield=id >> lab2b_list.csv
	@-./lab2_list --threads=8 --iterations=1 --lists=4 --yield=id >> lab2b_list.csv
	@-./lab2_list --threads=8 --iterations=2 --lists=4 --yield=id >> lab2b_list.csv
	@-./lab2_list --threads=8 --iterations=4 --lists=4 --yield=id >> lab2b_list.csv
	@-./lab2_list --threads=8 --iterations=8 --lists=4 --yield=id >> lab2b_list.csv
	@-./lab2_list --threads=8 --iterations=16 --lists=4 --yield=id >> lab2b_list.csv
	@-./lab2_list --threads=12 --iterations=1 --lists=4 --yield=id >> lab2b_list.csv
	@-./lab2_list --threads=12 --iterations=2 --lists=4 --yield=id >> lab2b_list.csv
	@-./lab2_list --threads=12 --iterations=4 --lists=4 --yield=id >> lab2b_list.csv
	@-./lab2_list --threads=12 --iterations=8 --lists=4 --yield=id >> lab2b_list.csv
	@-./lab2_list --threads=12 --iterations=16 --lists=4 --yield=id >> lab2b_list.csv
	@-./lab2_list --threads=16 --iterations=1 --lists=4 --yield=id >> lab2b_list.csv
	@-./lab2_list --threads=16 --iterations=2 --lists=4 --yield=id >> lab2b_list.csv
	@-./lab2_list --threads=16 --iterations=4 --lists=4 --yield=id >> lab2b_list.csv
	@-./lab2_list --threads=16 --iterations=8 --lists=4 --yield=id >> lab2b_list.csv
	@-./lab2_list --threads=16 --iterations=16 --lists=4 --yield=id >> lab2b_list.csv

	@./lab2_list --threads=1 --iterations=10 --lists=4 --yield=id --sync=s >> lab2b_list.csv
	@./lab2_list --threads=1 --iterations=20 --lists=4 --yield=id --sync=s >> lab2b_list.csv
	@./lab2_list --threads=1 --iterations=40 --lists=4 --yield=id --sync=s >> lab2b_list.csv
	@./lab2_list --threads=1 --iterations=80 --lists=4 --yield=id --sync=s >> lab2b_list.csv
	@./lab2_list --threads=4 --iterations=10 --lists=4 --yield=id --sync=s >> lab2b_list.csv
	@./lab2_list --threads=4 --iterations=20 --lists=4 --yield=id --sync=s >> lab2b_list.csv
	@./lab2_list --threads=4 --iterations=40 --lists=4 --yield=id --sync=s >> lab2b_list.csv
	@./lab2_list --threads=4 --iterations=80 --lists=4 --yield=id --sync=s >> lab2b_list.csv
	@./lab2_list --threads=8 --iterations=10 --lists=4 --yield=id --sync=s >> lab2b_list.csv
	@./lab2_list --threads=8 --iterations=20 --lists=4 --yield=id --sync=s >> lab2b_list.csv
	@./lab2_list --threads=8 --iterations=40 --lists=4 --yield=id --sync=s >> lab2b_list.csv
	@./lab2_list --threads=8 --iterations=80 --lists=4 --yield=id --sync=s >> lab2b_list.csv
	@./lab2_list --threads=12 --iterations=10 --lists=4 --yield=id --sync=s >> lab2b_list.csv
	@./lab2_list --threads=12 --iterations=20 --lists=4 --yield=id --sync=s >> lab2b_list.csv
	@./lab2_list --threads=12 --iterations=40 --lists=4 --yield=id --sync=s >> lab2b_list.csv
	@./lab2_list --threads=12 --iterations=80 --lists=4 --yield=id --sync=s >> lab2b_list.csv
	@./lab2_list --threads=16 --iterations=10 --lists=4 --yield=id --sync=s >> lab2b_list.csv
	@./lab2_list --threads=16 --iterations=20 --lists=4 --yield=id --sync=s >> lab2b_list.csv
	@./lab2_list --threads=16 --iterations=40 --lists=4 --yield=id --sync=s >> lab2b_list.csv
	@./lab2_list --threads=16 --iterations=80 --lists=4 --yield=id --sync=s >> lab2b_list.csv

	@./lab2_list --threads=1 --iterations=10 --lists=4 --yield=id --sync=m >> lab2b_list.csv
	@./lab2_list --threads=1 --iterations=20 --lists=4 --yield=id --sync=m >> lab2b_list.csv
	@./lab2_list --threads=1 --iterations=40 --lists=4 --yield=id --sync=m >> lab2b_list.csv
	@./lab2_list --threads=1 --iterations=80 --lists=4 --yield=id --sync=m >> lab2b_list.csv
	@./lab2_list --threads=4 --iterations=10 --lists=4 --yield=id --sync=m >> lab2b_list.csv
	@./lab2_list --threads=4 --iterations=20 --lists=4 --yield=id --sync=m >> lab2b_list.csv
	@./lab2_list --threads=4 --iterations=40 --lists=4 --yield=id --sync=m >> lab2b_list.csv
	@./lab2_list --threads=4 --iterations=80 --lists=4 --yield=id --sync=m >> lab2b_list.csv
	@./lab2_list --threads=8 --iterations=10 --lists=4 --yield=id --sync=m >> lab2b_list.csv
	@./lab2_list --threads=8 --iterations=20 --lists=4 --yield=id --sync=m >> lab2b_list.csv
	@./lab2_list --threads=8 --iterations=40 --lists=4 --yield=id --sync=m >> lab2b_list.csv
	@./lab2_list --threads=8 --iterations=80 --lists=4 --yield=id --sync=m >> lab2b_list.csv
	@./lab2_list --threads=12 --iterations=10 --lists=4 --yield=id --sync=m >> lab2b_list.csv
	@./lab2_list --threads=12 --iterations=20 --lists=4 --yield=id --sync=m >> lab2b_list.csv
	@./lab2_list --threads=12 --iterations=40 --lists=4 --yield=id --sync=m >> lab2b_list.csv
	@./lab2_list --threads=12 --iterations=80 --lists=4 --yield=id --sync=m >> lab2b_list.csv
	@./lab2_list --threads=16 --iterations=10 --lists=4 --yield=id --sync=m >> lab2b_list.csv
	@./lab2_list --threads=16 --iterations=20 --lists=4 --yield=id --sync=m >> lab2b_list.csv
	@./lab2_list --threads=16 --iterations=40 --lists=4 --yield=id --sync=m >> lab2b_list.csv
	@./lab2_list --threads=16 --iterations=80 --lists=4 --yield=id --sync=m >> lab2b_list.csv

#the aggregated throughput vs. the number of threads (lab2b_4.png) (lab2b_5.png)
	@./lab2_list --threads=1 --iterations=1000 --lists=4 --sync=m >> lab2b_list.csv
	@./lab2_list --threads=2 --iterations=1000 --lists=4 --sync=m >> lab2b_list.csv
	@./lab2_list --threads=4 --iterations=1000 --lists=4 --sync=m >> lab2b_list.csv
	@./lab2_list --threads=8 --iterations=1000 --lists=4 --sync=m >> lab2b_list.csv
	@./lab2_list --threads=12 --iterations=1000 --lists=4 --sync=m >> lab2b_list.csv
	@./lab2_list --threads=1 --iterations=1000 --lists=8 --sync=m >> lab2b_list.csv
	@./lab2_list --threads=2 --iterations=1000 --lists=8 --sync=m >> lab2b_list.csv
	@./lab2_list --threads=4 --iterations=1000 --lists=8 --sync=m >> lab2b_list.csv
	@./lab2_list --threads=8 --iterations=1000 --lists=8 --sync=m >> lab2b_list.csv
	@./lab2_list --threads=12 --iterations=1000 --lists=8 --sync=m >> lab2b_list.csv
	@./lab2_list --threads=1 --iterations=1000 --lists=16 --sync=m >> lab2b_list.csv
	@./lab2_list --threads=2 --iterations=1000 --lists=16 --sync=m >> lab2b_list.csv
	@./lab2_list --threads=4 --iterations=1000 --lists=16 --sync=m >> lab2b_list.csv
	@./lab2_list --threads=8 --iterations=1000 --lists=16 --sync=m >> lab2b_list.csv
	@./lab2_list --threads=12 --iterations=1000 --lists=16 --sync=m >> lab2b_list.csv

	@./lab2_list --threads=1 --iterations=1000 --lists=4 --sync=s >> lab2b_list.csv
	@./lab2_list --threads=2 --iterations=1000 --lists=4 --sync=s >> lab2b_list.csv
	@./lab2_list --threads=4 --iterations=1000 --lists=4 --sync=s >> lab2b_list.csv
	@./lab2_list --threads=8 --iterations=1000 --lists=4 --sync=s >> lab2b_list.csv
	@./lab2_list --threads=12 --iterations=1000 --lists=4 --sync=s >> lab2b_list.csv
	@./lab2_list --threads=1 --iterations=1000 --lists=8 --sync=s >> lab2b_list.csv
	@./lab2_list --threads=2 --iterations=1000 --lists=8 --sync=s >> lab2b_list.csv
	@./lab2_list --threads=4 --iterations=1000 --lists=8 --sync=s >> lab2b_list.csv
	@./lab2_list --threads=8 --iterations=1000 --lists=8 --sync=s >> lab2b_list.csv
	@./lab2_list --threads=12 --iterations=1000 --lists=8 --sync=s >> lab2b_list.csv
	@./lab2_list --threads=1 --iterations=1000 --lists=16 --sync=s >> lab2b_list.csv
	@./lab2_list --threads=2 --iterations=1000 --lists=16 --sync=s >> lab2b_list.csv
	@./lab2_list --threads=4 --iterations=1000 --lists=16 --sync=s >> lab2b_list.csv
	@./lab2_list --threads=8 --iterations=1000 --lists=16 --sync=s >> lab2b_list.csv
	@./lab2_list --threads=12 --iterations=1000 --lists=16 --sync=s >> lab2b_list.csv

profile: build
	@env CPUPROFILE=list.prof ./lab2_list --threads=12 --iterations=1000 --sync=s
	@pprof ./lab2_list list.prof > profile.out
	@pprof --list=thread_func ./lab2_list list.prof >> profile.out
	@rm -f list.prof

graphs: tests
	@chmod +x ./lab2_list.gp
	@./lab2_list.gp

dist: graphs profile
	@tar -czf $(TARBALL) README Makefile *.c *.h $(OUTPUT) $(GRAPHS) $(SCRIPTS)

clean:
	@rm -f lab2_list lab2b-505111806.tar.gz
