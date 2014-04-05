/* 
   Andrew Koe
   UDP_Client
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
#include <sys/sysinfo.h>
#include <time.h>

int main(int argc, char* argv[])
{
  int client_sock, rcv,serv_size;
  struct sockaddr_in client_addr;
  struct sockaddr_in server_addr;
  struct hostent *hp;
  char request[32];
  char response[80];
  int i;
  int server_size;

  if(argc < 4)
    {
      fprintf(stderr,"usage: prog hostn port request");
      exit(-1);
    }
  
  server_size = sizeof(server_addr);

  memset((char *)&request, '\0',sizeof(request));
  sprintf(request,"%s",argv[3]);
  
  if((client_sock = socket(AF_INET,SOCK_DGRAM,0)) < 0)
    {
      fprintf(stderr,"socket error: %s\n",strerror(errno));
      exit(-1);
    }
  
  memset((char *)&client_addr,0,sizeof(server_addr));
  client_addr.sin_family = AF_INET;
  client_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  client_addr.sin_port = htons(0);

  memset((char *)&server_addr,0,sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(atoi(argv[2]));
  
  if((hp = gethostbyname(argv[1])) <= 0)
    {
      fprintf(stderr,"hostnam: %s\n",strerror(errno));
      exit(-1);
    }
  
  if(bind(client_sock,(struct sockaddr *)&client_addr,sizeof(client_addr)) < 0)
    {
      fprintf(stderr,"Bind error: %s\n",strerror(errno));
      exit(-1);
    }
  
  if(sendto(client_sock,request,sizeof(request),0,(struct sockaddr *)&server_addr,sizeof(server_addr))< 0)
    {
      fprintf(stderr,"error sendto: %s\n",strerror(errno));
    }


  if(recvfrom(client_sock,response,sizeof(response),0, (struct sockaddr *)&server_addr, &server_size) <0)
    {
      fprintf(stderr,"error with recv: %s\n",strerror(errno));
    }

  for(i = 0; i < 81; i++)
    {
      fprintf(stderr,"%c",response[i]);
    }
  fprintf(stderr,"\n");
  close(client_sock);
}
