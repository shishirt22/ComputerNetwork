#include <stdio.h>
#include <fcntl.h>
#include <poll.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include "semaphore.h"
struct serverinfo{
	int sid,co_id,serv_id,cl_id;
	char wfifo[50],rfifo[50];
	int availablity;
};
