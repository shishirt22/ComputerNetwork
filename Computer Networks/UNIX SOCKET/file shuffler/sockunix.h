#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
int recv_fd(int sfd)
 {
  int sent_fd, buff;
  struct msghdr sock_msg;
  struct iovec io_vec[1];
  struct cmsghdr * cntl_msg = NULL;
  char msg_buff[1];
  char anc_buff[CMSG_SPACE(sizeof(int))];

  /* start clean */
  memset(&sock_msg, 0, sizeof(struct msghdr));
  memset(anc_buff, 0, CMSG_SPACE(sizeof(int)));

  /* setup a place to fill in message contents */
  io_vec[0].iov_base = msg_buff;
  io_vec[0].iov_len = 1;
  sock_msg.msg_iov = io_vec;
  sock_msg.msg_iovlen = 1;

  /* provide space for the ancillary data */
  sock_msg.msg_control = anc_buff;
  sock_msg.msg_controllen = CMSG_SPACE(sizeof(int));

  if(recvmsg(sfd, &sock_msg, MSG_CMSG_CLOEXEC) < 0)
   return -1;

  if(msg_buff[0] != 'F')
  {
   /* this did not originate from the above function */
   return -1;
  }

  if((sock_msg.msg_flags & MSG_CTRUNC) == MSG_CTRUNC)
  {
   /* we did not provide enough space for the ancillary element array */
   return -1;
  }

  /* iterate ancillary elements */
   for(cntl_msg = CMSG_FIRSTHDR(&sock_msg);
       cntl_msg != NULL;
       cntl_msg = CMSG_NXTHDR(&sock_msg, cntl_msg))
  {
   if( (cntl_msg->cmsg_level == SOL_SOCKET) &&
       (cntl_msg->cmsg_type == SCM_RIGHTS) )
   {
    sent_fd = *((int *) CMSG_DATA(cntl_msg));
    return sent_fd;
   }
  }

  return -1;
 }
int send_fd(int sfd, int fd)
 {
  struct msghdr sock_msg;
  struct iovec io_vec[1];
  struct cmsghdr * cntl_msg = NULL;
  char msg_buff[1];

  char anc_buf[CMSG_SPACE(sizeof(int))];
  int bufflen;

  /* at least one vector of one byte must be sent */
  msg_buff[0] = 'F';
  io_vec[0].iov_base = msg_buff;
  io_vec[0].iov_len = 1;

  /* initialize socket message */
  memset(&sock_msg, 0, sizeof(struct msghdr));
  sock_msg.msg_iov = io_vec;
  sock_msg.msg_iovlen = 1;

  /* provide space for the ancillary data */
 	bufflen = CMSG_SPACE(sizeof(int));
  memset(anc_buf, 0, bufflen);
  sock_msg.msg_control = anc_buf;  //anc buffer
  sock_msg.msg_controllen = bufflen;   //ancillary buffer length

  /* initialize a single ancillary data element for fd passing */
  cntl_msg= CMSG_FIRSTHDR(&sock_msg);
  cntl_msg->cmsg_level = SOL_SOCKET;   //for passing fd over socket
  cntl_msg->cmsg_type = SCM_RIGHTS;   //for passing Fd
  cntl_msg->cmsg_len = CMSG_LEN(sizeof(int));
  *((int *) CMSG_DATA(cntl_msg)) = fd;

  return sendmsg(sfd, &sock_msg, 0);
 }