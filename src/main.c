


#include "MKL05Z4.h"                    	/* Device header */
#include "CubeL.h"												/* Cube 5x5x5 */
#include "buttons.h"											/* Buttons of external keyboard */
#include <stdlib.h>                       /* Pseudorandom number generator */



int data[5][5][5];
int snake_size = 3;				//here we write current size of the snake
int old_snake_size = 3;				// here we write size of the snake before eating fruit
int direction = 31;
int defeat_or_win = 0;
int win = 0;
int lose = 0;


volatile int flag1 = 0;  			//variables which are used to control direction of the snake
volatile int flag2 = 0;
volatile int flag3 = 0;
volatile int flag4 = 0;

SRGB rgb_leds [ _LED_CNT ];
/// pin 1 - 0.....24  - first wall
/// pin 2 - 25.....49 - second wall
/// pin 3 - 50.....74 - third wall
/// pin 4 - 75.....99  - fourth wall
/// pin 5 - 100.....124 - fifth wall

uint8_t * pasek = (uint8_t*) &rgb_leds;


typedef struct Coordinates{
	int x;  //PIN
	int y;  //height
	int z;  //length
} XYZ;

XYZ snake[125];
XYZ food;
XYZ tmp;



void fill_Data(void);				//here we create reference to data from SRGB struct for our 3D matrix data[5][5][5]
void snake_start(void);				//here we set our starting conditions
void where_food(void);				//here we draw where food should appear
void check_food();				//here we check if snake ate food and if yes we make him bigger
void snake_control(void);			//we move here dependent on which direction we used earlier in loop
void snake_move(void);				//here is described how snake is moving in given direction
void check_when_lose_or_win(void);		//here we check if game ended
void lose_end_game(void);			//here we show word "lose" when somebody loses the game
void win_end_game(void);			//here we show word "win" when somebody wins the game
void display(void);				//here we show snake and food on our cube
void PORTB_IRQHandler(void);			// this function sets flag when we receive interrupt from the keyboard

//main loop
int main(void) {
	
	//here we prepare things necessary for our project 
	InitializeCube();
	buttonsInitialize();
	fill_Data();

	// we use this to turn off the leds which were lit from old program
	clear_all();		
	wait(5000);

	//this function light up leds where snake starts and where food is placed
	snake_start();

	// here we have loop where our game takes place
	while(!defeat_or_win)
	{	
		snake_move(); 
		snake_control();
		check_food();
		display();
		check_when_lose_or_win();
		delay_ms(2000);
	}

	//here we check how game ends
	lose_end_game();
	win_end_game();

}

//here we create reference to data from SRGB struct for our 3D matrix data[5][5][5]
void fill_Data(void)
{
	for(int i =0; i < 5; i++)
	{
		for(int j =0; j < 5; j++)
			{
				if((j == 1) || (j == 3)){
					for(int k =0; k < 5; k++)
					{
						data[i][j][k] = 5-k-1 + j*5 + i*25;
					}		
				}
				else{
				for(int k =0; k < 5; k++)
					{
						data[i][j][k] = k + j*5 + i*25;
					}
				}
			}
	}
}


//here we set our starting conditions
void snake_start()
{
	snake[0].x = 1;
	snake[0].y = 1;
	snake[0].z = 2;
	
	snake[1].x = 1;
	snake[1].y = 1;
	snake[1].z = 1;
	
	snake[2].x = 1;
	snake[2].y = 1;
	snake[2].z = 3;
	
	food.x = 1;
	food.y = 3;
	food.z = 2;
}


//here we draw where food should appear
void where_food()
{
	food.x =rand() % 5;
	food.y = rand() % 5;
	food.z = rand() % 5;
	
	for(int i = 0; i < snake_size; i++){
		if(snake[i].x == food.x){
			food.x =rand() % 5;
			//where_food();
		}
		if(snake[i].y == food.y){
			food.y = rand() % 5;
			//where_food();
		}
		if(snake[i].z == food.z){
			food.z = rand() % 5;
			//where_food();
		}
	}
}


//here we check if snake ate food and if yes we make him bigger
void check_food()
{
	if((snake[0].x == food.x) && (snake[0].y == food.y) && (snake[0].z == food.z)){
		old_snake_size = snake_size;
		snake_size++;
		snake[old_snake_size].x = tmp.x;
		snake[old_snake_size].y = tmp.y;
		snake[old_snake_size].z = tmp.z;
		where_food();
	}
}

//S1 button - turn right in XZ plane
//S5 button - turn left in XZ plane
//S9 - go up (direction = 21)
//S13 - go down ( direction = 22)

//we move here dependent on which direction we used earlier in loop
void snake_control()
{
	// turn right on XZ plane
	if(flag1)
	{
		
		if(direction == 11)
		{
				direction = 32;
		}	
		else if(direction == 12)
		{
				direction = 31;
		}
		else if(direction == 21)
		{
				direction = 32;
		}	
		else if(direction == 22)
		{
				direction = 31;
		}
		else if(direction == 31) 
		{
				direction = 11;
		}
		else if(direction == 32)
		{
				direction = 12;
		}
		
		flag1=0;
	}
	// turn left on XZ plane
	else if(flag2)
	{
		if(direction == 11)
		{
				direction = 31;
		}
		else if(direction == 12)
		{
				direction = 32;
		}	
		else if(direction == 21)
		{
				direction = 32;
		}	
		else if(direction == 22)
		{
				direction = 31;
		}
		else if(direction == 31) //start dir
		{
				direction = 12;
		}
		else if(direction == 32)
		{
				direction = 11;
		}
		flag2=0;
	}
	// go up
	else if(flag3)
	{
		if(direction == 11)
		{
				direction = 21;
		}	
		else if(direction == 12)
		{
				direction = 21;
		}
		else if(direction == 21)   
		{
				direction = 21;						
		}
		else if(direction == 22)
		{
				direction = 21;          
		}
		else if(direction == 31)
		{
				direction = 21;
		}	
		else if(direction == 32)
		{
				direction = 21;
		}		
		flag3=0;
	}
	// go down
	else if(flag4)
	{
		if(direction == 11)
		{
				direction = 22;
		}	
		else if(direction == 12)
		{
				direction = 22;
		}
		else if(direction == 21)
		{
				direction = 22;
		}	
		else if(direction == 22)
		{
				direction = 21;
		}
		else if(direction == 31)
		{
				direction = 22;
		}	
		else if(direction == 32)
		{
				direction = 22;
		}		
		flag4=0;
	}
}


//here is described how snake is moving in given direction
void snake_move()
{
	
	if(direction == 11)
	{
		for(int i=snake_size-1; i>0; i--)
		{
			tmp = snake[i];
			snake[i] = snake[i-1];
		}
		snake[0].x++;
		
		if(snake[0].x == Edgesize ) snake[0].x = 0;	
	}
	
	if(direction == 12)
	{
		for(int i=snake_size-1; i>0; i--)
		{
			tmp = snake[i];
			snake[i] = snake[i-1];
		}
		snake[0].x--;
		
		if(snake[0].x == -1 ) snake[0].x = 4;
	}
	
	if(direction == 21)
	{
		for(int i=snake_size-1; i>0; i--)
		{
			tmp = snake[i];
			snake[i] = snake[i-1];
		}
		snake[0].y++;
		
		if(snake[0].y == Edgesize ) snake[0].y = 0;
	}
	
	if(direction == 22)
	{
		for(int i=snake_size-1; i>0; i--)
		{
			tmp = snake[i];
			snake[i] = snake[i-1];
		}
		snake[0].y--;
		
		if(snake[0].y == -1 ) snake[0].y = 4;
	}
	
	if(direction == 31)
	{
		for(int i=snake_size-1; i>0; i--)
		{
			tmp = snake[i];
			snake[i] = snake[i-1];
		}
		snake[0].z++;
		
		if(snake[0].z == Edgesize ) snake[0].z = 0;
	}
	
	if(direction == 32)
	{
		for(int i=snake_size-1; i>0; i--)
		{
			tmp = snake[i];
			snake[i] = snake[i-1];
		}
		snake[0].z--;
		
		if(snake[0].z == -1 ) snake[0].z = 4;
	}
}


//here we check if game ended
void check_when_lose_or_win()
{
	for(int i=1; i < snake_size; i++)
	{
		if((snake[0].x == snake[i].x) && (snake[0].y == snake[i].y) && (snake[0].z == snake[i].z) ) 
		{
			defeat_or_win++;
			lose++;
		}
	}
	if (snake_size == 80) win++;		
}


//here we show word "lose" when somebody loses the game
void lose_end_game()
{
	if(lose == 1)
	{
		//1 we show letter here
		setPixel(pasek,data[4][4][1], 0xFF0000);
		setPixel(pasek,data[4][3][1], 0xFF0000);
		setPixel(pasek,data[4][2][1], 0xFF0000);
		setPixel(pasek,data[4][1][1], 0xFF0000);
		setPixel(pasek,data[4][0][1], 0xFF0000);
		setPixel(pasek,data[4][0][2], 0xFF0000);
		setPixel(pasek,data[4][0][3], 0xFF0000);
		setPixel(pasek,data[4][0][4], 0xFF0000);
		send_leds(pasek, 0);
		delay_ms(4000);
		//~1 and we clear letter here
		setPixel(pasek,data[4][4][1], 0x0000);
		setPixel(pasek,data[4][3][1], 0x0000);
		setPixel(pasek,data[4][2][1], 0x0000);
		setPixel(pasek,data[4][1][1], 0x0000);
		setPixel(pasek,data[4][0][1], 0x0000);
		setPixel(pasek,data[4][0][2], 0x0000);
		setPixel(pasek,data[4][0][3], 0x0000);
		setPixel(pasek,data[4][0][4], 0x0000);
		send_leds(pasek, 0);
		delay_ms(500);
		//2
		setPixel(pasek,data[4][4][1], 0xFF0000);
		setPixel(pasek,data[4][3][1], 0xFF0000);
		setPixel(pasek,data[4][2][1], 0xFF0000);
		setPixel(pasek,data[4][1][1], 0xFF0000);
		setPixel(pasek,data[4][0][1], 0xFF0000);
		setPixel(pasek,data[4][4][3], 0xFF0000);
		setPixel(pasek,data[4][3][3], 0xFF0000);
		setPixel(pasek,data[4][2][3], 0xFF0000);
		setPixel(pasek,data[4][1][3], 0xFF0000);
		setPixel(pasek,data[4][0][3], 0xFF0000);
		setPixel(pasek,data[4][0][2], 0xFF0000);
		setPixel(pasek,data[4][4][2], 0xFF0000);
		send_leds(pasek, 0);
		delay_ms(4000);
		//~2
		setPixel(pasek,data[4][4][1], 0x0000);
		setPixel(pasek,data[4][3][1], 0x0000);
		setPixel(pasek,data[4][2][1], 0x0000);
		setPixel(pasek,data[4][1][1], 0x0000);
		setPixel(pasek,data[4][0][1], 0x0000);
		setPixel(pasek,data[4][4][3], 0x0000);
		setPixel(pasek,data[4][3][3], 0x0000);
		setPixel(pasek,data[4][2][3], 0x0000);
		setPixel(pasek,data[4][1][3], 0x0000);
		setPixel(pasek,data[4][0][3], 0x0000);
		setPixel(pasek,data[4][0][2], 0x0000);
		setPixel(pasek,data[4][4][2], 0x0000);
		send_leds(pasek, 0);
		delay_ms(500);
		//3
		setPixel(pasek,data[4][4][1], 0xFF0000);
		setPixel(pasek,data[4][4][2], 0xFF0000);
		setPixel(pasek,data[4][4][3], 0xFF0000);
		setPixel(pasek,data[4][4][3], 0xFF0000);
		setPixel(pasek,data[4][3][1], 0xFF0000);
		setPixel(pasek,data[4][2][1], 0xFF0000);
		setPixel(pasek,data[4][2][2], 0xFF0000);
		setPixel(pasek,data[4][2][3], 0xFF0000);
		setPixel(pasek,data[4][1][3], 0xFF0000);
		setPixel(pasek,data[4][0][3], 0xFF0000);
		setPixel(pasek,data[4][0][2], 0xFF0000);
		setPixel(pasek,data[4][0][1], 0xFF0000);
		send_leds(pasek, 0);
		delay_ms(4000);
		//~3
		setPixel(pasek,data[4][4][1], 0x0000);
		setPixel(pasek,data[4][4][2], 0x0000);
		setPixel(pasek,data[4][4][3], 0x0000);
		setPixel(pasek,data[4][4][3], 0x0000);
		setPixel(pasek,data[4][3][1], 0x0000);
		setPixel(pasek,data[4][2][1], 0x0000);
		setPixel(pasek,data[4][2][2], 0x0000);
		setPixel(pasek,data[4][2][3], 0x0000);
		setPixel(pasek,data[4][1][3], 0x0000);
		setPixel(pasek,data[4][0][3], 0x0000);
		setPixel(pasek,data[4][0][2], 0x0000);
		setPixel(pasek,data[4][0][1], 0x0000);
		send_leds(pasek, 0);
		delay_ms(500);
		//4
		setPixel(pasek,data[4][4][1], 0xFF0000);
		setPixel(pasek,data[4][4][2], 0xFF0000);
		setPixel(pasek,data[4][4][3], 0xFF0000);
		setPixel(pasek,data[4][4][3], 0xFF0000);
		setPixel(pasek,data[4][3][1], 0xFF0000);
		setPixel(pasek,data[4][2][1], 0xFF0000);
		setPixel(pasek,data[4][2][2], 0xFF0000);
		setPixel(pasek,data[4][2][3], 0xFF0000);
		setPixel(pasek,data[4][1][1], 0xFF0000);
		setPixel(pasek,data[4][0][3], 0xFF0000);
		setPixel(pasek,data[4][0][2], 0xFF0000);
		setPixel(pasek,data[4][0][1], 0xFF0000);
		send_leds(pasek, 0);
		delay_ms(2000);
	}
}

//here we show word "win" when somebody wins the game
void win_end_game()
{
	if(win == 1)
	{
		//1
		setPixel(pasek,data[0][4][0], 0xFF0000);
		setPixel(pasek,data[0][3][0], 0xFF0000);
		setPixel(pasek,data[0][2][0], 0xFF0000);
		setPixel(pasek,data[0][1][0], 0xFF0000);
		setPixel(pasek,data[0][0][0], 0xFF0000);
		setPixel(pasek,data[0][1][1], 0xFF0000);
		setPixel(pasek,data[0][2][2], 0xFF0000);
		setPixel(pasek,data[0][1][3], 0xFF0000);
		setPixel(pasek,data[0][0][4], 0xFF0000);
		setPixel(pasek,data[0][1][4], 0xFF0000);
		setPixel(pasek,data[0][2][4], 0xFF0000);
		setPixel(pasek,data[0][3][4], 0xFF0000);
		setPixel(pasek,data[0][4][4], 0xFF0000);
		send_leds(pasek, 0);
		delay_ms(4000);
		//~1
		setPixel(pasek,data[0][4][0], 0x0000);
		setPixel(pasek,data[0][3][0], 0x0000);
		setPixel(pasek,data[0][2][0], 0x0000);
		setPixel(pasek,data[0][1][0], 0x0000);
		setPixel(pasek,data[0][0][0], 0x0000);
		setPixel(pasek,data[0][1][1], 0x0000);
		setPixel(pasek,data[0][2][2], 0x0000);
		setPixel(pasek,data[0][1][3], 0x0000);
		setPixel(pasek,data[0][0][4], 0x0000);
		setPixel(pasek,data[0][1][4], 0x0000);
		setPixel(pasek,data[0][2][4], 0x0000);
		setPixel(pasek,data[0][3][4], 0x0000);
		setPixel(pasek,data[0][4][4], 0x0000);
		send_leds(pasek, 0);
		delay_ms(500);
		//2
		setPixel(pasek,data[0][4][2], 0xFF0000);
		setPixel(pasek,data[0][3][2], 0xFF0000);
		setPixel(pasek,data[0][2][2], 0xFF0000);
		setPixel(pasek,data[0][1][2], 0xFF0000);
		setPixel(pasek,data[0][0][2], 0xFF0000);
		send_leds(pasek, 0);
		delay_ms(4000);
		//~2
		setPixel(pasek,data[0][4][2], 0x0000);
		setPixel(pasek,data[0][3][2], 0x0000);
		setPixel(pasek,data[0][2][2], 0x0000);
		setPixel(pasek,data[0][1][2], 0x0000);
		setPixel(pasek,data[0][0][2], 0x0000);
		send_leds(pasek, 0);
		delay_ms(500);
		//3
		setPixel(pasek,data[0][4][0], 0xFF0000);
		setPixel(pasek,data[0][3][0], 0xFF0000);
		setPixel(pasek,data[0][2][0], 0xFF0000);
		setPixel(pasek,data[0][1][0], 0xFF0000);
		setPixel(pasek,data[0][0][0], 0xFF0000);
		setPixel(pasek,data[0][3][1], 0xFF0000);
		setPixel(pasek,data[0][2][2], 0xFF0000);
		setPixel(pasek,data[0][1][3], 0xFF0000);
		setPixel(pasek,data[0][0][4], 0xFF0000);
		setPixel(pasek,data[0][1][4], 0xFF0000);
		setPixel(pasek,data[0][2][4], 0xFF0000);
		setPixel(pasek,data[0][3][4], 0xFF0000);
		setPixel(pasek,data[0][4][4], 0xFF0000);
		send_leds(pasek, 0);
		delay_ms(2000);
		
	}
}


//here we show snake and food on our cube
void display()
{	
	//clear_all();
	clearAllPixels( pasek );
	for(int i=1; i <snake_size; i++)
		{
			setPixel(pasek,data[snake[i].x][snake[i].y][snake[i].z], 0x001f00);
		}
		setPixel(pasek,data[snake[0].x][snake[0].y][snake[0].z], 0x001f1f);
		setPixel(pasek,data[food.x][food.y][food.z], 0x1f1f00);
		for(int j = 0; j < 5; j++)
		{
				send_leds(pasek, j);
		}
}


// this function sets flag when we receive interrupt from the keyboard
void PORTB_IRQHandler(void)
{
	//clear pending interrupts
	NVIC_ClearPendingIRQ(PORTB_IRQn);
	
  if((PORTB->ISFR & (1<<BUTTON_1_POS))){
		flag1 = 1;	}
	
		PORTB->PCR[BUTTON_1_POS] |= PORT_PCR_IRQC_MASK;
		
	 if((PORTB->ISFR & (1<<BUTTON_2_POS))){
		flag2 = 1;	}
	 PORTB->PCR[BUTTON_2_POS] |= PORT_PCR_IRQC_MASK;
		
	 if((PORTB->ISFR & (1<<BUTTON_3_POS))){
		flag3 = 1;	}
	 PORTB->PCR[BUTTON_3_POS] |= PORT_PCR_IRQC_MASK;
		
	 if((PORTB->ISFR & (1<<BUTTON_4_POS))){
		flag4 = 1;	}
		PORTB->PCR[BUTTON_4_POS] |= PORT_PCR_IRQC_MASK;
		
	
	//we use this to clear status flag
	PORTB->ISFR = 0xffffffff;
	
}

