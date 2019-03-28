all: http_srv http_clt

http_srv: http_server.c
	gcc -o http_srv http_server.c

http_clt: http_client.c
	gcc -o http_clt http_client.c

clean:
	rm http_srv http_clt

