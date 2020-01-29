#NAME: Senyang Jiang
#EMAIL: senyangjiang@yahoo.com
#ID: 505111806

all: lab1b-client.c lab1b-server.c
	@gcc -Wall -Wextra lab1b-client.c -o lab1b-client -lz
	@gcc -Wall -Wextra lab1b-server.c -o lab1b-server -lz

client: lab1b-client.c
	@gcc -Wall -Wextra lab1b-client.c -o lab1b-client -lz

server: lab1b-server.c
	@gcc -Wall -Wextra lab1b-server.c -o lab1b-server -lz~

clean:
	@rm -f lab1b-client lab1b-server lab1b-505111806.tar.gz

dist:
	@tar -czf lab1b-505111806.tar.gz Makefile lab1b-client.c lab1b-server.c README
