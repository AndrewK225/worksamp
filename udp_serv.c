/* 
   Andrew Koe
   UDP_server
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
  int serv_sock,rcv,cli_size;
  struct sockaddr_in serv_addr;
  struct sockaddr_in cli_addr;
  struct sysinfo info;
  char buf[32];
  char resp[80];
  time_t date;
  struct tm *timeinfo;
 
  cli_size = sizeof(cli_addr);
  memset((char *)&resp, '\0',sizeof(resp));
  if(argc < 2)
    {
      fprintf(stderr,"usage: prog port\n");
      exit(-1);
    }
  if((serv_sock = socket(AF_INET,SOCK_DGRAM,0)) < 0)
    {
      fprintf(stderr,"socket error: %s\n",strerror(errno));
    }

  memset((char*)&serv_addr,0,sizeof(serv_addr));
  
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  serv_addr.sin_port = htons(atoi(argv[1]));
  
  if(bind(serv_sock,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0)
    {
      fprintf(stderr,"error with bind: %s\n",strerror(errno));
      exit(-1);
    
    }

  if((rcv=recvfrom(serv_sock,buf,sizeof(buf), 0, (struct sockaddr *)&cli_addr, &cli_size)) < 0)
    {
      fprintf(stderr,"error w/recvfrom: %s",strerror(errno));
      exit(-1);
    }

  if(buf[0] == 'U')
    {      
      sysinfo(&info);
      sprintf(resp,"%d",(int)info.uptime);
      sendto(serv_sock, resp, sizeof(resp), 0,(struct sockaddr *)&cli_addr, sizeof(cli_addr));
    }
  else
    {
      time(&date);
      timeinfo = localtime(&date);
      strftime (resp,80,"%D  %I:%M%p.", timeinfo);
      sendto(serv_sock, resp, sizeof(resp), 0,(struct sockaddr *)&cli_addr, sizeof(cli_addr));
    }
  
  close(serv_sock);
  return 0;
}
