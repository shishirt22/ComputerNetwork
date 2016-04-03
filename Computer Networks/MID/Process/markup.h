#include <fcntl.h>
fg(int fd,int fd1){
fcntl(fd, F_SETFL, fcntl(0, F_GETFL) | O_NONBLOCK);
fcntl(fd1, F_SETFL, fcntl(0, F_GETFL) | O_NONBLOCK);
}