#NAME: Senyang Jiang
#EMAIL: senyangjiang@yahoo.com
#ID: 505111806

all: lab0.c
	gcc -Wall -Wextra lab0.c -o lab0
check: all
	echo "test" | ./lab0 1> /dev/null ;\
	if [ $$? -ne 0 ] ; then \
	echo "Standard input/output test failed! Returns $$? instead of 0" ;\
	exit 1 ;\
	fi ;\
	touch input_file ;\
	echo "test" > input_file ;\
	./lab0 --input=input_file --output=output_file ;\
	if [ $$? -ne 0 ] ; then \
	echo "Input/Output file redirection test failed! Returns $$? instead of 0" ;\
	exit 1 ;\
	fi ;\
	./lab0 nonargument 2> /dev/null ;\
	if [ $$? -ne 1 ] ; then \
	echo "Non-argument test failed! Returns $$? instead of 1" ;\
	exit 1 ;\
	fi ;\
	./lab0 --input 2> /dev/null ;\
	if [ $$? -ne 1 ] ; then \
	echo "Missing-argument test failed! Returns $$? instead of 1" ;\
	exit 1 ;\
	fi ;\
	./lab0 --input=nonexistent_file 2> /dev/null ;\
	if [ $$? -ne 2 ] ; then \
	echo "Nonexistent-input test failed! Returns $$? instead of 2" ;\
	exit 1 ;\
	fi ;\
	chmod -w output_file;
	./lab0 --input=input_file --output=output_file 2> /dev/null ;\
	if [ $$? -ne 3 ] ; then \
	echo "Unwritable-output test failed! Returns $$? instead of 3" ;\
	exit 1 ;\
	fi ;\
	./lab0 --segfault --catch 2> /dev/null ;\
	if [ $$? -ne 4 ] ; then \
	echo "Segmentation-and-catch test failed! Returns $$? instead of 4" ;\
	exit 1 ;\
	fi ;\
	echo "Passed all tests!" ;\
	rm -f input_file output_file
clean:
	rm -f lab0 lab0-505111806.tar.gz input_file output_file
dist:
	tar -czf lab0-505111806.tar.gz lab0.c Makefile backtrace.png breakpoint.png README 
