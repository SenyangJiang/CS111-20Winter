#NAME: Senyang Jiang
#EMAIL: senyangjiang@yahoo.com
#ID: 505111806

all:
	@gcc -Wall -Wextra lab4c_tcp.c -o lab4c_tcp -lmraa -lm
	@gcc -Wall -Wextra lab4c_tls.c -o lab4c_tls -lmraa -lm -lssl -lcrypto
clean:
	@rm -f lab4c_tcp lab4c_tls lab4c-505111806.tar.gz
dist:
	@tar -czf lab4c-505111806.tar.gz Makefile lab4c_tcp.c lab4c_tls.c README
