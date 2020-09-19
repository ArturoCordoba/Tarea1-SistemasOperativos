all : cliente servidor
.PHONY : all
servidor : server.c
	gcc server.c -lpng -o server
cliente : client.c
	gcc client.c -o client
clean :
	rm client server