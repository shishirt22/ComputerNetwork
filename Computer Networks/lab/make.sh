gcc mediator.c -o media -w -lpthread
gcc client.c -o cl -w -lpthread
gcc server.c -o s1 -lpthread -w
gcc server.c -o s2 -lpthread -w