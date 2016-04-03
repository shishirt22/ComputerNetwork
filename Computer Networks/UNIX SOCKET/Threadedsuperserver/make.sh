gcc super.c -o super -w
gcc server.c -o s1 -w -lpthread
gcc server.c -o s2 -w -lpthread
gcc cl.c -o cl -w