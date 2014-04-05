/* 
   Andrew Koe
   TCP_SEND
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
#include <time.h>

int main(int argc, char* argv[]) 
{
  int sock,count,total;
  struct sockaddr_in from_addr;
  struct sockaddr_in to_addr;
  struct hostent *hp;
  char *host = argv[1];
  char buffer[1024];
  
  total = 0;
  if(argc < 3)
  {
      fprintf(stderr,"usage: prog hostname port <instream\n");
      exit(-1);
  }
  

  // make socket
  if((sock = socket(AF_INET,SOCK_STREAM,0)) < 0)
    {
      fprintf(stderr,"Socket creation error: %s\n",strerror(errno));
      exit(-1);
    }
  fprintf(stderr,"made the socket\n");

  //bind socket
  memset((char *)&from_addr, 0, sizeof(from_addr));
  from_addr.sin_family = AF_INET;
  from_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  from_addr.sin_port = htons(0);

  if(bind(sock, (struct sockaddr *)&from_addr, sizeof(from_addr)) < 0)
    {
      fprintf(stderr, "Bind error: %s\n",strerror(errno));
      exit(-1);
    }

  fprintf(stderr,"Bound the socket\n");
  
  //get info about the server side
  memset((char *)&to_addr,0,sizeof(to_addr));
  to_addr.sin_family = AF_INET;
  to_addr.sin_port = htons(atoi(argv[2]));
  

  if((hp = gethostbyname(host)) <= 0)
    {
      fprintf(stderr,"gethost %s\n",strerror(errno));
      exit(-1);
    }
  
  memcpy((void *)&to_addr.sin_addr, hp->h_addr_list[0],hp->h_length);

  //connect to server
  if(connect(sock, (struct sockaddr *)&to_addr, sizeof(to_addr)) < 0)
    {
      fprintf(stderr,"connect error: %s\n",strerror(errno));
      exit(-1);
    }
  else
    fprintf(stderr,"connected\n");
  
  //take from stdin and send
  
  while((count = read(0,buffer,1024)) > 0)
    {
      if(write(sock,buffer,count) < 0)
	{
	  fprintf(stderr,"error with write: %s\n",strerror(errno));
	  exit(-1);
	}
      total += count;
    }

  if(count < 0)
    {
      fprintf(stderr,"error with read: %s\n",strerror(errno));
      exit(-1);
    }
 
  close(sock);
  
  fprintf(stderr,"Total Bytes sent: %d\n",total);  
    
  return 0;
}
