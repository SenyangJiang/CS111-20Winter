#NAME: Senyang Jiang
#EMAIL: senyangjiang@yahoo.com
#ID: 505111806

all:
	@gcc -Wall -Wextra lab4b.c -o lab4b -lmraa -lm
check: all
	@{ echo "SCALE=C"; sleep 1; echo "PERIOD=2"; sleep 1; \
	echo "STOP"; sleep 1; echo "START"; sleep 1; echo "LOG helloworld"; \
	sleep 1; echo "OFF"; } | ./lab4b --log=test 1>/dev/null; \
	if [ $$? -ne 0 ]; then \
	echo "program fails!";\
	exit 1; \
	fi; \
	grep -E '^[0-9]{2}:[0-9]{2}:[0-9]{2}\s[0-9]+\.[0-9]{1}$$' test 1>/dev/null;\
	if [ $$? -ne 0 ]; then \
        echo "no temperature readings in logfile!";\
        exit 1;\
        fi; \
	for s in SCALE=C PERIOD=2 STOP START LOG OFF SHUTDOWN; do \
	grep "$$s" test 1>/dev/null; \
	if [ $$? -ne 0 ]; then \
        echo "no $$s command in logfile!";\
        exit 1;\
        fi;\
	done;\
	echo "Passed all tests!";\
	rm -f test
clean:
	@rm -f lab4b lab4b-505111806.tar.gz
dist:
	@tar -czf lab4b-505111806.tar.gz lab4b.c Makefile README
