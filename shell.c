/*
  Andrew Koe
  shell program
  
*/

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>


char parse_line(char line[]);
void spawn(char *cmd,char *args[],char *filename, char redir[]);
void redirect_control(char redir[],char *fname);

int main(int argc, char* argv[])
{
  char buf[256];
  char quit;
  FILE* fp;
  quit = 0;
  if(argc == 1)   //no command args if just shell
    {
      while(quit != 1)
	{
	  if(fgets(buf,256,stdin) == NULL)
	    {
	      fprintf(stderr,"Cannot read from stdin\n");
	      exit(-1);
	    }

	  if(buf[0] == '#') //ignore comments
	    {
	      continue;
	    }
	  else
	    {
	      quit = parse_line(buf);
	    }
	}
    }
  else if(argc == 2)  //if filename provided, be interpreter
    {
      fp = fopen(argv[1],"r");
      while(fgets(buf,256,fp) != NULL)
	{
	  if(buf[0] == '#' || buf[0] == '!')
	    {
	      continue;	
	    }
	  else
	    {
	      quit = parse_line(buf);
	    }
	}
    }
  else
    {
      fprintf(stderr,"usage: shell [script file]\n");      
      exit(-1);
    }
  return 0;
}

char parse_line(char line[])
{
  char *tok;
  char *cmd_arg[4];
  char *fname;          //filename
  char checker[24];
  int check_cnt;
  unsigned char sig, cmd_index, redir_index;
  char redir[3];   //for parsing through redirection ops.  CAUSES ERROR
  cmd_index = 0;
  sig = 0;
  redir_index = 0;

  tok = strtok(line,"\n"); //get rid of the newline and check for exit
  if(strcmp(tok,"exit") == 0)
    {
      return 1;
    }

  tok = strtok(tok," \t"); //WORKS ON ASSUMPTION THAT COMMAND IS ALWAYS present first
  cmd_arg[cmd_index] = tok;

  /*break up the rest of the line into cmd options and redirection*/
  while(tok != NULL)
    {
      tok = strtok(NULL," \t");
      if(sig == 1)
	{
	  fname = tok;
	  break;
	}

      check_cnt = sprintf(checker,"%s",tok);      
      if(checker[0] != '<' && checker[0] != '>')
	{
	  if(checker[0] == '2')
	    {
	      if(checker[1] == '>')
		{
		  sig = 1;
		  redir[redir_index] = checker[0];
		  redir[++redir_index] = checker[1];
		  if(checker[2] == '>')
		    {
		      redir[++redir_index] = checker[2];
		    }
		}
	      else
		{
		  cmd_arg[++cmd_index] = tok;
		}
	    }
  
	  cmd_arg[++cmd_index] = tok;
	}
      else
	{
	  sig = 1;
	  redir[redir_index] = checker[0];
	  redir[++redir_index] = checker[1];
	}
    }
  spawn(cmd_arg[0],cmd_arg,fname,redir);
  return 0;
  
}

void spawn(char *cmd, char *args[], char *filename, char redir[])
{
  int pid;
  int fd; //file descriptor
  printf("%s%s\n",cmd,args[1]);
  pid = fork();
  
  switch(pid) {
  case -1:
    fprintf(stderr,"ERROR WITH FORK\n");
    exit(-1);
    break;

  case 0:
    redirect_control(redir,filename);
    if(execvp(cmd,args) < 0)
      {
	fprintf(stderr,"execvp error: %s\n",strerror(errno));
	exit(-1);
      }
    break;    

  default:
    wait(NULL);   
    break;    
  }
}

void redirect_control(char redir[],char *fname)
{
  char fd;
  char n;
  n = 0;

  if(redir[n] == '<')
    {
      printf("red 1\n");
      if((fd = open(fname, O_RDWR)) < 0);
      {
	fprintf(stderr,"open error: %s\n", strerror(errno));
      }
      dup2(fd,0);
    }
 
  if(redir[0] == '2')
    {
      n++;
      if(redir[n] == '>' && redir[n+1] == '>')
	{
	  printf("red 5\n");
	  if((fd = open(fname,O_CREAT | O_RDWR | O_APPEND,0777)) < 0)
	    {
	      fprintf(stderr,"open error: %s\n", strerror(errno));
	    }
	}
      else if(redir[n] == '>')
	{
	  printf("red 3\n");
	  if((fd = open(fname,O_CREAT | O_RDWR | O_TRUNC,0777)) < 0)
	    {
	      fprintf(stderr,"open error: %s\n", strerror(errno));
	    }
	}
      dup2(fd,2);
    }
  else
    {
      if(redir[n] == '>' && redir[n+1] == '>')
        {
	  printf("red 4\n");
          if((fd = open(fname,O_CREAT | O_RDWR | O_APPEND,0777)) < 0)
	    {
	      fprintf(stderr,"open error: %s\n", strerror(errno));
	    }
	  dup2(fd,1);
        }
      else if(redir[n] == '>')
        {
	  printf("red 2\n");
          if((fd = open(fname,O_CREAT | O_RDWR | O_TRUNC,0777)) < 0)
	    {
	      fprintf(stderr,"open error: %s\n", strerror(errno));
	    }
	  dup2(fd,1);
	}
    }
}
