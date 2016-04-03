gcc client.c -o cl -w
gcc ISS.c -o sc -w
gcc service.c -o s1  -lpthread -w
gcc service.c -o s2  -lpthread -w
gcc service.c -o s3  -lpthread -w
gcc service.c -o s4  -lpthread -w
gcc service.c -o s5  -lpthread -w
gcc service.c -o s6  -lpthread -w