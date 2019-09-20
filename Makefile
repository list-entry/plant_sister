client.cgi : client.o ipc.o
	arm-linux-gcc -o client.cgi  ipc.o client.o
client.o : client.c config.h ipc.h 
	arm-linux-gcc -c client.c 
ipc.o : ipc.h ipc.c config.h 
	arm-linux-gcc -c ipc.c 
	
server : server.o ipc.o
	arm-linux-gcc -o server server.o ipc.o
server.o : server.c config.h ipc.h
	arm-linux-gcc -c server.c 
	
clean:
	rm -rf *.o
