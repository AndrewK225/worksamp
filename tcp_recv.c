/*
  Andrew Koe
  TCP_RECV
*/

#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char* argv[])
{
  int sock,port,count,a_sock,total;
  struct sockaddr_in to_addr;
  struct sockaddr_in from_addr;
  socklen_t len;
  char buf[1024];
  if(argc < 2)
    {
      fprintf(stderr,"usage: prog port\n");
      exit(-1);
    }
  
  port = atoi(argv[1]);

  if((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
      fprintf(stderr,"socket creation error: %s\n",strerror(errno));
      exit(-1);
    }
  memset((char*)&to_addr, 0, sizeof(to_addr));
  to_addr.sin_family = AF_INET;
  to_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  to_addr.sin_port = htons(port);  //user specifies port

  if(bind(sock, (struct sockaddr *)&to_addr, sizeof(to_addr)) < 0 )
    {
      fprintf(stderr,"error with bind: %s\n",strerror(errno));
      exit(-1); 
    }

  if(listen(sock,2) < 0)
    {
      fprintf(stderr,"Error with listen: %s\n",strerror(errno));
      exit(-1);
    }

  fprintf(stderr,"listening\n");
  for(;;)   //look to accept 
    {
      while((a_sock=accept(sock,(struct sockaddr *)&from_addr,&len)) < 0)
	{
	  if((errno != ECHILD) && (errno = ERESTART) && (errno != EINTR))
	    {
	      fprintf(stderr,"Error with accept: %s\n",strerror(errno));
	      exit(-1);
	    }
	  
	}
      total = 0;
      while((count = read(a_sock,buf,1024)) > 0)
	{
	  if(write(1,buf,count) < 0)
	    {
	      fprintf(stderr,"write: %s\n",strerror(errno));
	      exit(-1);
	    }
	  total += count;
	}
    
      close(sock);
      close(a_sock);
      fprintf(stderr,"bytes received: %d\n",total);
      
      return 0;
    }

}
	  
