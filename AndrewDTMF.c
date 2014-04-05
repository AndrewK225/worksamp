/* 
Andrew Koe
DTMF Project 03/11/2013
expanded on sample code that was provided
*/


#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/soundcard.h>
#include <string.h> //used for strlen typecast
#include <math.h>


int dsp_fd;// File descr for sound card; used by several functions


void dsp_open(char *dsp_name, int mode, int samp_fmt, int n_chan, int samp_rate)
{
  int arg;

  // Open the sound card
  if( (dsp_fd = open(dsp_name, mode)) < 0 )
    {
      printf("Cannot open sound card '%s'\n", dsp_name);
      exit(1);
    }


  // According to article cited above, settings should
  // take place in the following order...
  
  // Set sample format (num. of bits, little/big endian, encoding, etc.)
  arg = samp_fmt;
  if( ioctl(dsp_fd, SNDCTL_DSP_SETFMT, &arg) == -1 )
    {
      printf("Ioctl error setting sound card sample format\n");
      exit(1);
    }
  if( arg != samp_fmt )    {
      printf("Sound card cannot be set to %d\n", samp_fmt);
      exit(1);
    }


  // Set number of channels (e.g. 1 (mono) or 2 (stereo))
  arg = n_chan;
  n_chan = 1;
  if( ioctl(dsp_fd, SNDCTL_DSP_CHANNELS, &arg) == -1 )
    {
      printf("Ioctl error setting number of channels\n");
      exit(1);
    }
  if( arg != n_chan )
    {
      printf("Could not set number of channels to %d\n", n_chan);
      exit(1);
    }


  // Set sample rate (num. of samps. per sec.)
  arg = samp_rate;
  if( ioctl(dsp_fd, SNDCTL_DSP_SPEED, &arg) == -1 )
    {
      printf("Ioctl error setting sample rate\n");
      exit(1);
    }
  if( arg != samp_rate )
    {
      printf("Could not set sample rate to %d\n", samp_rate);
      exit(1);
    }
}


void dsp_write(void *data, size_t samp_size, size_t n_samps)
{
  int size_ttmp;

  size_ttmp = samp_size * n_samps;

  if( write(dsp_fd, data, size_ttmp) != size_ttmp )
    {
      printf("Write error\n");
      exit(1);
    }
}


void dsp_sync(void)
{
  int tmp;

  tmp = 0;
  if( ioctl(dsp_fd, SNDCTL_DSP_SYNC, &tmp) == -1 )
    {
      printf("Ioctl error syncing dsp\n");
      exit(1);
    }
}


void dsp_close(void)
{
  if( close(dsp_fd) < 0 )
    {
      printf("Error closing sound card\n");
      exit(1);
    }
}




// Start of the program

#define TEST_LEN 10000
#define PAUSE 40000
#define PAUSE2 80000
#define SAMP_RATE 48000

#define PI (3.14159265)

 
int main()
{
  short s[TEST_LEN];
  short a[TEST_LEN];
  int F1; //first frequency
  int F2; // second
  int i; // iteration counter for test length
  int n; // counter for the for loop(switch case) addresses elements of char key
  int length_of_array;
  
  
  char key[256];
  fgets(key, 256, stdin); //Stores user input into a character array.  Buffer is set to 256

  length_of_array = (int) strlen(key) - 1; //-1 accounts for the null character
  
 
  /*The following for loop and switch case loops access each element of the char array(where user input is stored), check its value and play
    the respective frequencies.  The rationale for including the dsp functions inside the case loops is due to the fact that I implemented the
    pauses p and P by writing silence (0 frequency) to the sound card multiple times*/
  
  for(n = 0; n < length_of_array; n++)
    {
      switch (key[n]) {
  
      case 48: //0
	F1 = 1336;
	F2 =  941;
	for(i=0; i < TEST_LEN; i++)
	  s[i] = (32767*sin(2.0*PI*F1*i/SAMP_RATE))/2 + (32767*sin(2.0*PI*F2*i/SAMP_RATE)/2);
	
	//SOUND CARD (NOT part of the for loop immediately above)
	dsp_open("/dev/dsp", O_WRONLY, AFMT_S16_NE, 1, SAMP_RATE); //access sound card
	dsp_write(s, sizeof(short), TEST_LEN); // write the test signal
	break;
  
      case 49: // 1
	F1 = 1209;
	F2 =  697;
	for(i=0; i < TEST_LEN; i++)
	  s[i] = (32767*sin(2.0*PI*F1*i/SAMP_RATE))/2 + (32767*sin(2.0*PI*F2*i/SAMP_RATE)/2);
	
	dsp_open("/dev/dsp", O_WRONLY, AFMT_S16_NE, 1, SAMP_RATE);
 	dsp_write(s, sizeof(short), TEST_LEN);
	break;
  
      case 50: //2
  
	F1 = 1336;
	F2 = 697;
	for(i=0; i < TEST_LEN; i++)
	  s[i] = (32767*sin(2.0*PI*F1*i/SAMP_RATE))/2 + (32767*sin(2.0*PI*F2*i/SAMP_RATE)/2);
       
	dsp_open("/dev/dsp", O_WRONLY, AFMT_S16_NE, 1, SAMP_RATE); 
    	dsp_write(s, sizeof(short), TEST_LEN);
	break;
  
      case 51: //3
	F1 = 1477;
	F2 =  697;
	for(i=0; i < TEST_LEN; i++)
	  s[i] = (32767*sin(2.0*PI*F1*i/SAMP_RATE))/2 + (32767*sin(2.0*PI*F2*i/SAMP_RATE)/2);
    
	dsp_open("/dev/dsp", O_WRONLY, AFMT_S16_NE, 1, SAMP_RATE); 
	dsp_write(s, sizeof(short), TEST_LEN);
	break;
  
      case 52: //4
	F1 = 1209;
	F2 =  770;
	for(i=0; i < TEST_LEN; i++)
	  s[i] = (32767*sin(2.0*PI*F1*i/SAMP_RATE))/2 + (32767*sin(2.0*PI*F2*i/SAMP_RATE)/2);
	
	dsp_open("/dev/dsp", O_WRONLY, AFMT_S16_NE, 1, SAMP_RATE);
       	dsp_write(s, sizeof(short), TEST_LEN);
	break;
    
      case 53://5
	F1 = 1336;
	F2 =  770;
	for(i=0; i < TEST_LEN; i++)
	  s[i] = (32767*sin(2.0*PI*F1*i/SAMP_RATE))/2 + (32767*sin(2.0*PI*F2*i/SAMP_RATE)/2);
	
	dsp_open("/dev/dsp", O_WRONLY, AFMT_S16_NE, 1, SAMP_RATE);
	dsp_write(s, sizeof(short), TEST_LEN);
	break;
  
  
      case 54://6
	F1 = 1477;
	F2 =  770;
	for(i=0; i < TEST_LEN; i++)
	  s[i] = (32767*sin(2.0*PI*F1*i/SAMP_RATE))/2 + (32767*sin(2.0*PI*F2*i/SAMP_RATE)/2);
	
	dsp_open("/dev/dsp", O_WRONLY, AFMT_S16_NE, 1, SAMP_RATE);
	dsp_write(s, sizeof(short), TEST_LEN);
	break;
  
  
      case 55: //7
	F1 = 1209;
	F2 =  852;
	for(i=0; i < TEST_LEN; i++)
	  s[i] = (32767*sin(2.0*PI*F1*i/SAMP_RATE))/2 + (32767*sin(2.0*PI*F2*i/SAMP_RATE)/2);
	
	dsp_open("/dev/dsp", O_WRONLY, AFMT_S16_NE, 1, SAMP_RATE);
	dsp_write(s, sizeof(short), TEST_LEN);
	break;
  
      case 56: //8
	F1 = 1336;
	F2 =  852;
	for(i=0; i < TEST_LEN; i++)
	  s[i] = (32767*sin(2.0*PI*F1*i/SAMP_RATE))/2 + (32767*sin(2.0*PI*F2*i/SAMP_RATE)/2);

	dsp_open("/dev/dsp", O_WRONLY, AFMT_S16_NE, 1, SAMP_RATE);   
        dsp_write(s, sizeof(short), TEST_LEN);
	break;
  
      case 57: //9
	F1 = 1477;
	F2 =  852;
	for(i=0; i < TEST_LEN; i++)
	  s[i] = (32767*sin(2.0*PI*F1*i/SAMP_RATE))/2 + (32767*sin(2.0*PI*F2*i/SAMP_RATE)/2);
	
	dsp_open("/dev/dsp", O_WRONLY, AFMT_S16_NE, 1, SAMP_RATE);
	dsp_write(s, sizeof(short), TEST_LEN);
	break;
  
      case 97: //a
	F1 = 1633;
	F2 =  697;
	for(i=0; i < TEST_LEN; i++)
	  s[i] = (32767*sin(2.0*PI*F1*i/SAMP_RATE))/2 + (32767*sin(2.0*PI*F2*i/SAMP_RATE)/2);
	
	dsp_open("/dev/dsp", O_WRONLY, AFMT_S16_NE, 1, SAMP_RATE);
       	dsp_write(s, sizeof(short), TEST_LEN);
	break;
  
      case 98://b
	F1 = 1633;
	F2 =  770;
	for(i=0; i < TEST_LEN; i++)
	  s[i] = (32767*sin(2.0*PI*F1*i/SAMP_RATE))/2 + (32767*sin(2.0*PI*F2*i/SAMP_RATE)/2);
	
	dsp_open("/dev/dsp", O_WRONLY, AFMT_S16_NE, 1, SAMP_RATE);   
      	dsp_write(s, sizeof(short), TEST_LEN);
	break;
  
      case 99://c
	F1 = 1633;
	F2 =  852;
	for(i=0; i < TEST_LEN; i++)
	  s[i] = (32767*sin(2.0*PI*F1*i/SAMP_RATE))/2 + (32767*sin(2.0*PI*F2*i/SAMP_RATE)/2);
	
	dsp_open("/dev/dsp", O_WRONLY, AFMT_S16_NE, 1, SAMP_RATE);
        dsp_write(s, sizeof(short), TEST_LEN);
	break;
  
      case 100: //d
	F1 = 1633;
	F2 =  941;
	for(i=0; i < TEST_LEN; i++)
	  s[i] = (32767*sin(2.0*PI*F1*i/SAMP_RATE))/2 + (32767*sin(2.0*PI*F2*i/SAMP_RATE)/2);
	
	dsp_open("/dev/dsp", O_WRONLY, AFMT_S16_NE, 1, SAMP_RATE);
     	dsp_write(s, sizeof(short), TEST_LEN);
	break;
  
      case 65: //A
	F1 = 1633;
	F2 =  697;
	for(i=0; i < TEST_LEN; i++)
	  s[i] = (32767*sin(2.0*PI*F1*i/SAMP_RATE))/2 + (32767*sin(2.0*PI*F2*i/SAMP_RATE)/2);
	
	dsp_open("/dev/dsp", O_WRONLY, AFMT_S16_NE, 1, SAMP_RATE);      
	dsp_write(s, sizeof(short), TEST_LEN);
	break;
  
      case 66: //B
	F1 = 1633;
	F2 =  770;
	for(i=0; i < TEST_LEN; i++)
	  s[i] = (32767*sin(2.0*PI*F1*i/SAMP_RATE))/2 + (32767*sin(2.0*PI*F2*i/SAMP_RATE)/2);
	
	dsp_open("/dev/dsp", O_WRONLY, AFMT_S16_NE, 1, SAMP_RATE);    
        dsp_write(s, sizeof(short), TEST_LEN);
	break;
  
      case 67: //C
	F1 = 1633;
	F2 =  852;
	for(i=0; i < TEST_LEN; i++)
	  s[i] = (32767*sin(2.0*PI*F1*i/SAMP_RATE))/2 + (32767*sin(2.0*PI*F2*i/SAMP_RATE)/2);
       
	dsp_open("/dev/dsp", O_WRONLY, AFMT_S16_NE, 1, SAMP_RATE);
	dsp_write(s, sizeof(short), TEST_LEN);
	break;
  
      case 68: //D
	F1 = 1633;
	F2 =  941;
	for(i=0; i < TEST_LEN; i++)
	  s[i] = (32767*sin(2.0*PI*F1*i/SAMP_RATE))/2 + (32767*sin(2.0*PI*F2*i/SAMP_RATE)/2);
	
	dsp_open("/dev/dsp", O_WRONLY, AFMT_S16_NE, 1, SAMP_RATE);
       	dsp_write(s, sizeof(short), TEST_LEN);
	break;
  
      case 42: //*
	F1 = 1209;
	F2 =  941;
	for(i=0; i < TEST_LEN; i++)
	  s[i] = (32767*sin(2.0*PI*F1*i/SAMP_RATE))/2 + (32767*sin(2.0*PI*F2*i/SAMP_RATE)/2);
	
	dsp_open("/dev/dsp", O_WRONLY, AFMT_S16_NE, 1, SAMP_RATE);
     	dsp_write(s, sizeof(short), TEST_LEN);
	break;
  
      case 35: //#
	F1 = 1477;
	F2 =  941;
	for(i=0; i < TEST_LEN; i++)
	  s[i] = (32767*sin(2.0*PI*F1*i/SAMP_RATE))/2 + (32767*sin(2.0*PI*F2*i/SAMP_RATE)/2);
	
	dsp_open("/dev/dsp", O_WRONLY, AFMT_S16_NE, 1, SAMP_RATE);
        dsp_write(s, sizeof(short), TEST_LEN);
	break;
      
      case 112: //p (1 sec pause)
	F1 = 0;
	F2 = 0;
	for(i=0; i < TEST_LEN; i++)
	  s[i] = (32767*sin(2.0*PI*F1*i/SAMP_RATE))/2 + (32767*sin(2.0*PI*F2*i/SAMP_RATE)/2);
       
	dsp_open("/dev/dsp", O_WRONLY, AFMT_S16_NE, 1, SAMP_RATE);
 
	/*These 4 dsp writes continually write silence(0 frequency) to the sound card.  each write is 
	  approx 0.25 sec therefore 4 writes make 1 sec.  Multiple writes in this case and the 
	  following case are the reason for not a general dsp_open/dsp_write that all cases go to.  
	  These two cases require more writes. */
	dsp_write(s, sizeof(short), TEST_LEN);
	dsp_write(s, sizeof(short), TEST_LEN);
	dsp_write(s, sizeof(short), TEST_LEN);
	dsp_write(s, sizeof(short), TEST_LEN);
      break;

      case 80: //P( 2 sec pause)
	F1 = 0;
	F2 =  0;
	for(i=0; i < TEST_LEN; i++)
	  s[i] = (32767*sin(2.0*PI*F1*i/SAMP_RATE))/2 + (32767*sin(2.0*PI*F2*i/SAMP_RATE)/2);
	
	dsp_open("/dev/dsp", O_WRONLY, AFMT_S16_NE, 1, SAMP_RATE);
 
    
	/* There are 8 writes to write 2 seconds of silence*/
	dsp_write(s, sizeof(short), TEST_LEN);
	dsp_write(s, sizeof(short), TEST_LEN);
	dsp_write(s, sizeof(short), TEST_LEN);
	dsp_write(s, sizeof(short), TEST_LEN);
	dsp_write(s, sizeof(short), TEST_LEN);
	dsp_write(s, sizeof(short), TEST_LEN);
	dsp_write(s, sizeof(short), TEST_LEN);
	dsp_write(s, sizeof(short), TEST_LEN);
	break;
	
      default:
	printf("That's not a valid character; silly goose :P.  Try again, you'll get it right next time :)\n");  
	break;
      }
    
    
      //The following two functions all called regardless of which case is used since they are outside of switch case,
      //but part of the for loop.
         
      // May be necessary to flush the buffers
      dsp_sync();

      // Close connection to the sound card
      dsp_close();
    }

  // Successful exit
  exit(0);
}
