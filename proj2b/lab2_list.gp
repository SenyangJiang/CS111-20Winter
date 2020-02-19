#! /usr/bin/gnuplot
#
# purpose:
#	 generate data reduction graphs for the multi-threaded list project
#
# input: lab2_list.csv
#	1. test name
#	2. # threads
#	3. # iterations per thread
#	4. # lists
#	5. # operations performed (threads x iterations x (ins + lookup + delete))
#	6. run time (ns)
#	7. run time per operation (ns)
#	8. average wait-for-lock time (ns)
# output:
#	lab2b_1.png ... total number of operations per second for each synchronization method
#	lab2_list-2.png ... threads and iterations that run (un-protected) w/o failure
#	lab2_list-3.png ... threads and iterations that run (protected) w/o failure
#	lab2_list-4.png ... cost per operation vs number of threads
#
# Note:
#	Managing data is simplified by keeping all of the results in a single
#	file.  But this means that the individual graphing commands have to
#	grep to select only the data they want.
#
#	Early in your implementation, you will not have data for all of the
#	tests, and the later sections may generate errors for missing data.
#

# general plot parameters
set terminal png
set datafile separator ","

# total number of operations per second for each synchronization method
set title "Total number of operations per second vs. Number of threads"
set xlabel "Threads"
set logscale x 2
set xrange[0.75:]
set ylabel "Operations per second"
set logscale y 10
set output 'lab2b_1.png'

plot \
     "< grep -E 'list-none-m,.+,.+,1,' lab2b_list.csv" using ($2):(1000000000)/($7) \
	title 'mutex' with linespoints lc rgb 'red', \
     "< grep -E 'list-none-s,.+,.+,1,' lab2b_list.csv" using ($2):(1000000000)/($7) \
	title 'spin-lock' with linespoints lc rgb 'green'


set title "wait-for-lock time & the average time per operation vs. number of threads"
set xlabel "Threads"
set logscale x 2
set xrange [0.75:]
set ylabel "Time(ns)"
set logscale y 10
set output 'lab2b_2.png'

plot \
     "< grep -E 'list-none-m,.+,.+,1,' lab2b_list.csv" using ($2):($8) \
        title 'wait-for-lock time' with linespoints lc rgb 'red', \
     "< grep -E 'list-none-m,.+,.+,1,' lab2b_list.csv" using ($2):($7) \
        title 'average time per operation' with linespoints lc rgb 'green'

set title "Threads and Iterations that run without failure"
set xlabel "Threads"
set logscale x 2
set xrange [0.75:]
set ylabel "Iterations per thread"
set logscale y 2
set yrange [0.75:]
set output 'lab2b_3.png'

plot \
     "< grep -E 'list-id-none,.+,.+,4,' lab2b_list.csv" using ($2):($3) \
        title 'no synchronization' with points lc rgb 'blue', \
     "< grep -E 'list-id-m,.+,.+,4,' lab2b_list.csv" using ($2):($3) \
        title 'mutex' with points ps 1.3 lc rgb 'red', \
     "< grep -E 'list-id-s,.+,.+,4,' lab2b_list.csv" using ($2):($3) \
        title 'spin-lock' with points lc rgb 'green'

set title "aggregated throughput vs. the number of threads (sync=m)"
set xlabel "Threads"
set logscale x 2
set xrange [0.75:]
set ylabel "total operations per second"
set logscale y 10
set yrange [10000:]
set output 'lab2b_4.png'

plot \
     "< grep -E 'list-none-m,(1|2|4|8|12),1000,1,' lab2b_list.csv" using ($2):(1000000000)/($7) \
        title 'lists=1' with linespoints lc rgb 'red', \
     "< grep -E 'list-none-m,(1|2|4|8|12),1000,4,' lab2b_list.csv" using ($2):(1000000000)/($7) \
        title 'lists=4' with linespoints lc rgb 'green', \
     "< grep -E 'list-none-m,(1|2|4|8|12),1000,8,' lab2b_list.csv" using ($2):(1000000000)/($7) \
        title 'lists=8' with linespoints lc rgb 'violet', \
     "< grep -E 'list-none-m,(1|2|4|8|12),1000,16,' lab2b_list.csv" using ($2):(1000000000)/($7) \
        title 'lists=16' with linespoints lc rgb 'orange'

set title "aggregated throughput vs. the number of threads (sync=s)"
set xlabel "Threads"
set logscale x 2
set xrange [0.75:]
set ylabel "total operations per second"
set logscale y 10
set yrange [10000:]
set output 'lab2b_5.png'

plot \
     "< grep -E 'list-none-s,(1|2|4|8|12),1000,1,' lab2b_list.csv" using ($2):(1000000000)/($7) \
        title 'lists=1' with linespoints lc rgb 'red', \
     "< grep -E 'list-none-s,(1|2|4|8|12),1000,4,' lab2b_list.csv" using ($2):(1000000000)/($7) \
        title 'lists=4' with linespoints lc rgb 'green', \
     "< grep -E 'list-none-s,(1|2|4|8|12),1000,8,' lab2b_list.csv" using ($2):(1000000000)/($7) \
        title 'lists=8' with linespoints lc rgb 'violet', \
     "< grep -E 'list-none-s,(1|2|4|8|12),1000,16,' lab2b_list.csv" using ($2):(1000000000)/($7) \
        title 'lists=16' with linespoints lc rgb 'orange'
