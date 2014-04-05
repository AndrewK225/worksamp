/*
  This program allows the user to play battleship with the computer. I made the 
  pseudorandom generator.
*/


#include <stdio.h>
#include <stdlib.h>



short choose_place(char comp_board[], char size, char place);
void place_piece(char comp_board[], char size, short position);
void human_place_piece(char player_board[]);
uint randgen(char seed);
char guess_handle(char buffer[]);
char take_guess(char seed);
void table_gen(char computer_board[], char player_board[]);

static char comp_board[100];
static char player_board[100];




int main()
{
  char a, cont, e, buf[3];
  char s;
  short start_position;
  short placement;

  printf("enter a number(0-10):");
  scanf("%d", &a);

 

  //setup stuffs
  for(s = 5; s >= 2; s--)
  {
    start_position = choose_place(comp_board, s, a);
    //printf("return position = %d\n", start_position);
    if(start_position != -1)
      {
	place_piece(comp_board, s, start_position);
	a = a +2;
      }

    else
      {
        while(start_position < 0)
          {
            //printf("start = %d\n",start_position);
	    start_position = choose_place(comp_board, s, a+3);
            //printf("%d\n", start_position);
	    a++;
          }
        place_piece(comp_board, s, start_position);
      }
    a = start_position;
  }
  
  human_place_piece(player_board);

  //gameplay
   cont = 0;
   while(cont == 0)
    {
      
      buf[0] = 0;
      buf[1] = 0;
      buf[2] = 0;
      fflush(stdin);
      printf("guess: ");
      fgets(buf, 4, stdin);
      //printf("skipped\n");
      cont = guess_handle(buf);
      
      e = take_guess(a);   
    
      }

   //    printf("YOU WIN!!!\n");


    table_gen(comp_board, player_board);

  
}

//***NOTE*** main will have to deal with a reroll
short choose_place(char comp_board[],char size, char place) //will return either a valid place to place ship or if horizontal and vertical about spot dont work, return -1
{

  unsigned char spot,z;
  char multiplier;
  char i;
  i = 0;

  // This variable will increment by ten so each boundary is checked

  spot = randgen(place);
  //printf("spot(should equal start_position) = %d\n", spot);
  multiplier = spot/10;


  if(spot+size <= (9+10*multiplier)) //right hand boundary check
    {
      while (i < size)
        {
          if(comp_board[spot+i] != 1) //check the place to check ship crash
            {
              i++;
            }

          else
            {
              goto vertical;
            }
        }

      return spot;


    }//end of horizontal check

 vertical:
  i = 0;//reinitialize
  while(i < size)
    {
      z = spot+(10*i); //move vertically
      if((z < 99) && comp_board[z] != 1)
        {
          i++;
        }
      else
        {
          //printf("about to return\n");
          return -1;
          //printf("returned");
        }

    } //end vertical check
  return spot+100;

}


uint randgen(char seed)
{
  uint number;
  number = seed + 16;
  number = number * 32;
  number = (number) / 9;
  number = number *2;
  number = number * 18;
  number = number << 2;
  number = number + 5;
  number = number / 60;
  number = number * 11;
  number = number + 6;

  if(number > 99)
    {
      while(number > 99)
        number = number / 23;
      return number;
    }
  else
    return number;
}





/* function for placing pieces*/
void place_piece(char comp_board[], char size, short position)
{
  unsigned char l;// size of ship
  l = 0;

  while (l < size)
    {
      
      if(position < 100 && position >= 0) // the ship will be placed horizontally
        {

	  //printf("in horizontal positioning\n");
	  //printf("%d\n", (position + l));
	  comp_board[position + l] = 1; //change the 0s to 1s to signify a ship
	  //p//rintf("l = %d\n",l);
        }

      else if(position > 99)
	{
          //printf("in vertical positioning\n");
          //printf("%d\n", (position - 100 + (10 * l)));
          comp_board[(position - 100 + (10 * l))] = 1;
        }
      l++;
      //printf("%d\n",l);
    }

  
}
 

void human_place_piece(char player_board[])
{
  int start_of_ship; 
  char dummy;
  char orientation; 
  char size, multiplier,i;
  char c, linecount, line_move;
  size = 5; 
  while(size >= 2)
    {
      printf("placing ship of size %d\n",size);
      printf("where do you want to place it(0-99)? ");
      //printf("size234 = %d\n",size);
      scanf("%d", &start_of_ship);
      //printf("start_of_ship = %d\n",start_of_ship);
      //printf("what orientation would you like? 1 for horizontal, 2 for vertical? ");
      //scanf("%d", &orientation);
      orientation = 1;
      printf("start = %d\n", start_of_ship);
      multiplier = (start_of_ship)/10;
      
      if(orientation == 1)
	{
	  //printf("fell here\n");
	  //printf("size0 = %d\n",size);
	  if(start_of_ship + size <= (9 + 10*multiplier)) //check boundary
	    {
	      //printf("came here\n");
	      //printf("size1 = %d\n",size);
	      for(i = 0; i <= size; i++)
		{
		  //printf("about to channge array\n");
		  player_board[start_of_ship+i] = 1;
		}
	      
	      //printf("changed\n");
	      
	    }
	
	}
      /*      else if(orientation == 2)
	{
	  if((start_of_ship + 10*size - 10) <= 99) // lower boundary check
	    {
	      for(i = 0; i < size; i++)
		{
		  player_board[start_of_ship + 10*i] = 1;
		
		}
	    
	    }
	    } */

      else
	;
      //printf("size = %d",size);
      size--;
      //printf("size = %d",size);
    }


  printf("Player board print out\n");


  //why don't you just make c count up to ten, and every time it gets up to ten you add a newline.  This should get you 10 in a row
  //you can also make some other variable that counts up to 10 so you increment it at the end of every line.

  linecount = 0;
  line_move = 0;
  while(linecount < 10) 
    {
      for(c = 0; c <= 9; c++)
	{
	  if(c < 9)
	    printf("%d ",player_board[(10*line_move)+c]);
	  else
	    {
	      printf("%d \n", player_board[(10*line_move)+c]);
	      linecount++;
	      line_move++;
	    }
    
	}
    }
}



//This function will take in a guess from the user.  check for a hit/miss (return a 0 while game continues)
char guess_handle(char buffer[])
{
  
  char letter;
  char number;
  char guessed_index;
  char scanner;

  letter = buffer[0] - 65;
  number = (buffer[1]*10);
  guessed_index = letter+number;
  //  printf("guessed = %d\n", guessed_index);
  if(buffer[0] == 's' || buffer[0] == 'S')
    {
      printf("That's right, be scared\n You lose\n");
      return 1;
    }

  if(comp_board[guessed_index] == 1) //check for a hit
    {
      printf("HIT! You're soooo good!\n");
      comp_board[guessed_index] = 2; //set the 1 to a 2
      for(scanner = 0; scanner <= 99; scanner++)
	{
	  if(comp_board[scanner] == 1)//if there are ships left
	    return 0;
	  else
	    ;
	}
       
    
    }
  else 
    {
      printf("MISS\n");
      comp_board[guessed_index] = 3;
      return 0;//game continues
    }
  printf("you win\n");
  return 1;//game ends since it will only get here if the scanner did not find a 1;

}

char take_guess(char cue)
{
  char letter;
  char number;
  char comp_guess;
  char y;
  //printf("in take_guess\n");
  comp_guess = randgen(cue);
  //printf("%d\n", comp_guess);
  while(player_board[comp_guess] == 1)
    {
      comp_guess = randgen(cue + y);
      y++;
    }
  letter = (comp_guess % 10) + 65;
  number = (comp_guess/10);

  printf("I choose %c%d\n",letter,number);
  
  
  player_board[comp_guess] = 1;
  return 0;
}


//generate table
void table_gen(char comp_board[], char player_board[])
{

  int c; // value in buf
  char linecount;
  char line_move;

  printf("Computer board printout\n");


  linecount = 0;
  line_move = 0;
  while(linecount < 10)
    {
      for(c = 0; c <=9; c++)
	{
	  if(c<9)
	    printf("%d ", comp_board[(10*line_move) + c]);
	  else 
	    {
	      printf("%d\n", comp_board[(10*line_move) + c]);
	      linecount++;
	      line_move++;
	    }
	}
    }


  printf("Player board print out\n");

  linecount = 0;
  line_move = 0;
  while(linecount < 10)
    {
      for(c = 0; c <= 9; c++)
	{
	  if(c < 9)
	    printf("%d ", player_board[(10*line_move) + c]);
	  else 
	    {
	      printf("%d \n", player_board[(10*line_move) + c]);
	      linecount++;
	      line_move++;
	    }
	}
    }
}
