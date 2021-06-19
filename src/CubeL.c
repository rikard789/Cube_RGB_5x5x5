////////////////////////////////////////////////////////////
//////***non-optymised library version***///////////////////
////////////////////////////////////////////////////////////

#include "MKL05Z4.h"                    //Device header
#include "CubeL.h"												



void InitializeCube()
{
	//Initialize pins to send data on leds
	SIM->SCGC5 |=  SIM_SCGC5_PORTB_MASK;      /* Enable Clock to Port B */ 
	for(volatile int i = 0; i < Edgesize; i++ )
	{
			PORTB->PCR[pins[i]] = PORT_PCR_MUX(1);        /*Set pin 1,2,3,4,5 as GPIO */	
			PTB->PDDR |= (1<<pins[i]);	
	}

}

//////////////////////////////////////////////////////////////////////////////////////////
//***********here i have functions which send bits 0 and 1 to LEDS*************//
//***********amount of nopes we use here depends on which type of diode we use (here we use WS2811)*******//
static void set1(int x)
{
	PTB->PDOR |= (1 << pins[x]);
	__asm("nop");__asm("nop");__asm("nop");__asm("nop");__asm("nop");__asm("nop");
	__asm("nop");__asm("nop");__asm("nop");__asm("nop");__asm("nop");__asm("nop");
	__asm("nop");__asm("nop");__asm("nop");__asm("nop");__asm("nop");__asm("nop");              							
	
	
	
	PTB->PDOR&= ~(1 << pins[x]);
}

static void set0(int x)
{
	PTB->PDOR |= (1 << pins[x]);
	__asm("nop");__asm("nop");__asm("nop");__asm("nop");__asm("nop");__asm("nop");
	__asm("nop");__asm("nop");
	PTB->PDOR&= ~(1 << pins[x]);
}
//////////////////////////////////////////////////////////////////////////////////////////


void sendrgb(uint8_t r, uint8_t g, uint8_t b, int x )
{
	
	if(r & 128) set1(x); else set0(x);
	if(r & 64) set1(x); else set0(x);
	if(r & 32) set1(x); else set0(x);
	if(r & 16) set1(x); else set0(x);
	if(r & 8) set1(x); else set0(x);
	if(r & 4) set1(x); else set0(x);
	if(r & 2) set1(x); else set0(x);
	if(r & 1) set1(x); else set0(x);
	
	if(g & 128) set1(x); else set0(x);
	if(g & 64) set1(x); else set0(x);
	if(g & 32) set1(x); else set0(x);
	if(g & 16) set1(x); else set0(x);
	if(g & 8) set1(x); else set0(x);
	if(g & 4) set1(x); else set0(x);
	if(g & 2) set1(x); else set0(x);
	if(g & 1) set1(x); else set0(x);
	
	if(b & 128) set1(x); else set0(x);
	if(b & 64) set1(x); else set0(x);
	if(b & 32) set1(x); else set0(x);
	if(b & 16) set1(x); else set0(x);
	if(b & 8) set1(x); else set0(x);
	if(b & 4) set1(x); else set0(x);
	if(b & 2) set1(x); else set0(x);
	if(b & 1) set1(x); else set0(x);
}

//here we send color in hex 
//for example: sendcolor(0xFF0000, 2); to light up red color on pin 2 leds
void sendcolor(uint32_t c, int x )
{
	uint8_t r = c >> 16, g = c >> 8, b = c;
	
	if(r & 128) set1(x); else set0(x);
	if(r & 64) set1(x); else set0(x);
	if(r & 32) set1(x); else set0(x);
	if(r & 16) set1(x); else set0(x);
	if(r & 8) set1(x); else set0(x);
	if(r & 4) set1(x); else set0(x);
	if(r & 2) set1(x); else set0(x);
	if(r & 1) set1(x); else set0(x);
	
	if(g & 128) set1(x); else set0(x);
	if(g & 64) set1(x); else set0(x);
	if(g & 32) set1(x); else set0(x);
	if(g & 16) set1(x); else set0(x);
	if(g & 8) set1(x); else set0(x);
	if(g & 4) set1(x); else set0(x);
	if(g & 2) set1(x); else set0(x);
	if(g & 1) set1(x); else set0(x);
	
	if(b & 128) set1(x); else set0(x);
	if(b & 64) set1(x); else set0(x);
	if(b & 32) set1(x); else set0(x);
	if(b & 16) set1(x); else set0(x);
	if(b & 8) set1(x); else set0(x);
	if(b & 4) set1(x); else set0(x);
	if(b & 2) set1(x); else set0(x);
	if(b & 1) set1(x); else set0(x);
}

//// send bit 0 to all leds on the wall
void clearwall(int x)
{
	
	for(int i = 0; i <24*25 ; i++) set0(x);
	
}


//// send bit 0 to all leds on all walls
void clear_all(void)
{
for(int j=0; j<5; j++){
		for(int i = 0; i <24*25 ; i++) set0(j);
	}

}

//// send bit 1 to all leds on the wall
void lightupwall(int x)
{
	
	for(int i = 0; i < 24*25; i++)
	{
			set1(x);
	}
	
}

// here we set color and brightness of one led in our memory 
void setPixel( void* line, int ledofchoice, uint32_t color )
{
	if(ledofchoice<0) return;
	if(ledofchoice >= _LED_CNT) return;
	ledofchoice*=3;
	*((uint8_t*)line + ledofchoice) = color >> 16;
	*((uint8_t*)line + ledofchoice+1) = color >> 8;
	*((uint8_t*)line + ledofchoice+2) = color ;
}

void clearAllPixels(void * line )
{
	for(int i = 0; i < 125; i++)
	{
		setPixel(line, i, 0x00);
	}
}


void create_line(void * line, int len_start, uint16_t len, uint32_t color)
{
	for( int i = len_start; i < (int)(len_start + len); i++)
	{
		setPixel(line, i,  color );
	}
}



//here we send to our cube what is written in our memory
void send_leds(SRGB * leds, int x)
{
	uint8_t i;
	uint8_t *wsk= (uint8_t*)leds; 
	if(x == 0)
	{
		for(i = 0; i < 25; i++ )
		{
			sendrgb( *wsk, *(wsk +1), *(wsk +2), 0);
			wsk+=3;
		}
	}
	if(x == 1)
	{
		for(i = 25; i < 50; i++ )
		{
			sendrgb( *(wsk+75 ), *(wsk+75 +1), *(wsk+75+2), 1);
			wsk+=3;
		}
	}
	if(x == 2)
	{
		for(i = 50; i < 75; i++ )
		{
			sendrgb( *(wsk+150), *(wsk+150 +1), *(wsk+150 +2), 2);
			wsk+=3;
		}
	}
	if(x == 3)
	{
		for(i = 75; i < 100; i++ )
		{
			sendrgb( *(wsk+225), *(wsk+225 +1), *(wsk+225 +2), 3);
			wsk+=3;
		}
	}
	if(x == 4)
	{
		for(i = 100; i < _LED_CNT; i++ )
		{
			sendrgb( *(wsk+300), *(wsk+300 +1), *(wsk+300 +2), 4);
			wsk+=3;
		}
	}
	
}


/////////////////////////////////////////////////////////////////////////
//here we have diffrent functions which show eye-catching effects


// in this function i change between red, green and blue color in the same pattern periodically
void cycle_color_change(	SRGB rgb_leds[] )
{
	uint16_t i, k=0;	
	//uint8_t *wsk = (uint8_t*)&rgb_leds;

	uint16_t howmanyleds = 125 ;                    //here i sets how many led i want to use
	memset(&rgb_leds,0, 125*3 );			//we must clear all before we begin because when we don't use all leds, leds which are not used can be lit up in diffrent colours
	while(1)
	{
		for(int j=0; j<5; j++)
		{
			uint8_t *wsk = (uint8_t*)&rgb_leds;
			memset(&rgb_leds,0, howmanyleds*3 );                               //here we must reset what we written to RAM because we want our colors to keep changing
			for(i=0; i < howmanyleds*3; i++)  
			{
				if((i % 3) == k )*wsk = 255;                              //here we change our colours  
				*wsk++;
			}	

			send_leds(&rgb_leds, j);
			k++;
			if(k>2) k=0;
		}
		wait(2400000);
	}

}




void flying_strips1(void * pasek)
{
while(1){
	for(int i = 0; i < 125; i++)
	{
	
	create_line( pasek, i, 4, 0x0f0f00);
	create_line( pasek, 121 -i, 4, 0x000f0f);
		for(int j = 0; j < 5; j++)
		{
			send_leds(pasek, j);
		}
	delay_ms(100);
	}
 }
}


void flying_strips2(void * pasek)
{
while(1){
	for(int i = 0; i < 125; i++)
	{
	
	create_line( pasek, i, 5, 0x0f0f00);
	setPixel(pasek, i, 0x000000);
	create_line( pasek, 121 -i, 5, 0x000f0f);
	setPixel(pasek, 125-i, 0x000000);
		for(int j = 0; j < 5; j++)
		{
			send_leds(pasek, j);
		}
	delay_ms(50);
	}
 }
}


void flying_strips3(void * pasek, int data[5][5][5])
{
	
	while(1){
		
		//1
			for( int y=0; y < 5; y++)
			{
				uint8_t y1 = 1;
				uint8_t y2 = 2;
				
				//1 strip 
					setPixel(pasek, data[0][0][4-y], 0x000000);
					if(y==4) setPixel(pasek, data[0][y-4][0], 0x000000);
					setPixel(pasek, data[0][y][0], 0x0f0f00);
					//5 strip 
					setPixel(pasek, data[4][0][4-y], 0x000000);
					if(y==4) setPixel(pasek, data[4][y-4][0], 0x000000);
					setPixel(pasek, data[4][y][0], 0x0f0f00);
				
				
				//4 strip 
				setPixel(pasek, data[3][0][4-y], 0x000000);
				if(y==4) setPixel(pasek, data[3][y-4][0], 0x000000);
				setPixel(pasek, data[3][y][0], 0x00000f);
				//2 strip 
				setPixel(pasek, data[1][0][4-y], 0x000000);
				if(y==4) setPixel(pasek, data[1][y-4][0], 0x000000);
				setPixel(pasek, data[1][y][0], 0x00000f);
				
				//3 strip 
				setPixel(pasek, data[2][0][4-y], 0x000000);
				if(y==4) setPixel(pasek, data[2][y-4][0], 0x000000);
				setPixel(pasek, data[2][y][0], 0x000f0f);
				
				
				send_leds(pasek, 0);
				send_leds(pasek, 1);
				send_leds(pasek, 2);
				send_leds(pasek, 3);
				send_leds(pasek, 4);
				delay_ms(500);
			}
			//2
			for( int z=1; z < 5; z++)
			{
				//1 strip 
				setPixel(pasek, data[0][z-1][0], 0x000000);
				if(z==4) setPixel(pasek, data[0][4][z-4], 0x000000);
				setPixel(pasek, data[0][4][z], 0x0f0f00);
				//5 strip 
				setPixel(pasek, data[4][z-1][0], 0x000000);
				if(z==4) setPixel(pasek, data[4][4][z-4], 0x000000);
				setPixel(pasek, data[4][4][z], 0x0f0f00);
				//4 strip 
				setPixel(pasek, data[3][z-1][0], 0x000000);
				if(z==4) setPixel(pasek, data[3][4][z-4], 0x000000);
				setPixel(pasek, data[3][4][z], 0x00000f);
				//2 strip
				setPixel(pasek, data[1][z-1][0], 0x000000);
				if(z==4) setPixel(pasek, data[1][4][z-4], 0x000000);
				setPixel(pasek, data[1][4][z], 0x00000f);
				//3 strip 
				setPixel(pasek, data[2][z-1][0], 0x000000);
				if(z==4) setPixel(pasek, data[2][4][z-4], 0x000000);
				setPixel(pasek, data[2][4][z], 0x000f0f);
				
				send_leds(pasek, 0);
				send_leds(pasek, 1);
				send_leds(pasek, 2);
				send_leds(pasek, 3);
				send_leds(pasek, 4);
				delay_ms(500);
			}
			//3
			for( int y=3; y >= 0; y--)
			{
				//1 strip
				setPixel(pasek, data[0][4][4-y], 0x000000);
				if(y==1) setPixel(pasek, data[0][y+4][0], 0x000000);
				setPixel(pasek, data[0][y][4], 0x0f0f00);
				//5 strip
				setPixel(pasek, data[4][4][4-y], 0x000000);
				if(y==1) setPixel(pasek, data[4][y+4][0], 0x000000);
				setPixel(pasek, data[4][y][4], 0x0f0f00);
				//4 strip
				setPixel(pasek, data[3][4][4-y], 0x000000);
				if(y==1) setPixel(pasek, data[3][y+4][0], 0x000000);
				setPixel(pasek, data[3][y][4], 0x00000f);
				//2 strip
				setPixel(pasek, data[1][4][4-y], 0x000000);
				if(y==1) setPixel(pasek, data[1][y+4][0], 0x000000);
				setPixel(pasek, data[1][y][4], 0x00000f);
				//3 strip
				setPixel(pasek, data[2][4][4-y], 0x000000);
				if(y==1) setPixel(pasek, data[2][y+4][0], 0x000000);
				setPixel(pasek, data[2][y][4], 0x000f0f);
				
				send_leds(pasek, 0);
				send_leds(pasek, 1);
				send_leds(pasek, 2);
				send_leds(pasek, 3);
				send_leds(pasek, 4);
				delay_ms(500);
			}
			//4
			for( int z=3; z >= 0; z--)
			{
				//1 strip
				setPixel(pasek, data[0][z][4], 0x000000);
				if(z==1) setPixel(pasek, data[0][z+4][0], 0x000000);
				setPixel(pasek, data[0][0][z], 0x0f0f00);
				//5 strip
				setPixel(pasek, data[4][z][4], 0x000000);
				if(z==1) setPixel(pasek, data[4][z+4][0], 0x000000);
				setPixel(pasek, data[4][0][z], 0x0f0f00);
				//4 strip
				setPixel(pasek, data[3][z][4], 0x000000);
				if(z==1) setPixel(pasek, data[3][z+4][0], 0x000000);
				setPixel(pasek, data[3][0][z], 0x00000f);
				//2 strip
				setPixel(pasek, data[1][z][4], 0x000000);
				if(z==1) setPixel(pasek, data[1][z+4][0], 0x000000);
				setPixel(pasek, data[1][0][z], 0x00000f);
				//3 strip
				setPixel(pasek, data[2][z][4], 0x000000);
				if(z==1) setPixel(pasek, data[2][z+4][0], 0x000000);
				setPixel(pasek, data[2][0][z], 0x000f0f);
				
				
				send_leds(pasek, 0);
				send_leds(pasek, 1);
				send_leds(pasek, 2);
				send_leds(pasek, 3);
				send_leds(pasek, 4);
				delay_ms(500);
			}
	}
}

void disappering_cubes(void * pasek, int data[5][5][5])
{
	while(1){
	// frame 1
	clearAllPixels( pasek );
	for( int i = 0 ; i < 5 ; i++){
		send_leds(pasek, i);}
	
	for( int i = 0 ; i < 5 ; i++){
		setPixel ( pasek, data[4][0][i], 0x30D5C8);
		setPixel ( pasek, data[4][i][0], 0x30D5C8);
		setPixel ( pasek, data[4][i][4], 0x30D5C8);
		setPixel ( pasek, data[4][4][i], 0x30D5C8);
		
		setPixel ( pasek, data[i][0][0], 0x30D5C8);
		setPixel ( pasek, data[i][4][0], 0x30D5C8);
		setPixel ( pasek, data[i][0][4], 0x30D5C8);
		setPixel ( pasek, data[i][4][4], 0x30D5C8);
		
		setPixel ( pasek, data[0][0][i], 0x30D5C8);
		setPixel ( pasek, data[0][i][0], 0x30D5C8);
		setPixel ( pasek, data[0][i][4], 0x30D5C8);
		setPixel ( pasek, data[0][4][i], 0x30D5C8);
		
		}
		
	for( int i = 0 ; i < 5 ; i++){
		send_leds(pasek, i);}
	delay_ms(1000);

	
	// frame 2
	
	clearAllPixels( pasek );
		for( int i = 0 ; i < 5 ; i++){
		send_leds(pasek, i);}
	
		for( int i = 1 ; i < 4 ; i++){
		setPixel ( pasek, data[1][1][i], 0x006633);
		setPixel ( pasek, data[1][i][1], 0x006633);
		setPixel ( pasek, data[1][i][3], 0x006633);
		setPixel ( pasek, data[1][3][i], 0x006633);
		
		setPixel ( pasek, data[i][1][1], 0x006633);
		setPixel ( pasek, data[i][3][1], 0x006633);
		setPixel ( pasek, data[i][1][3], 0x006633);
		setPixel ( pasek, data[i][3][3], 0x006633);
		
		setPixel ( pasek, data[3][1][i], 0x006633);
		setPixel ( pasek, data[3][i][1], 0x006633);
		setPixel ( pasek, data[3][3][i], 0x006633);
		setPixel ( pasek, data[3][i][3], 0x006633);
		}
	for( int i = 0 ; i < 4 ; i++){
		send_leds(pasek, i);}
	delay_ms(1000);
	
	//frame 3
		
	 clearAllPixels( pasek );
		for( int i = 0 ; i < 5 ; i++){
		send_leds(pasek, i);}
	
		setPixel ( pasek, data[2][2][2], 0xCF2942);
		send_leds(pasek, 2);
		delay_ms(1000);
	
		
	}
}


//wait function where argument is clock tick
void wait(volatile uint32_t cnt) {
    while(cnt--)
        __asm("nop");
}

