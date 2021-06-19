#ifndef CubeL_H
#define CubeL_H

#define Edgesize 5	// length of one side of the cube
#define _LED_CNT 125	//here we define how many leds our cube have (int this project we use 5x5x5 cube so its 125)
#define PTB1 6        	//here we define which pins we use for our cube you can use for example pins 1, 2, 3, 4, 5 in port B 
#define PTB2 7
#define PTB3 8
#define PTB4 9
#define PTB5 10


typedef struct{
	uint8_t r;
	uint8_t g;
	uint8_t b;
	
} SRGB;

//extern SRGB rgb_leds[_LED_CNT];

static int pins[Edgesize] = {PTB1,PTB2,PTB3,PTB4,PTB5};


void InitializeCube();                     					//this function prepares pins of our FRDM-KL05Z to  sending data to our cube             
void sendrgb(uint8_t r, uint8_t g, uint8_t b, int x );   			//on subsequent calls it lights up leds on selected pin in color of choice starting from beggining of the line
void sendcolor(uint32_t c, int x );						//on subsequent calls it lights up leds on selected pin in color written in hex starting from beggining of the line
void clearwall(int x);								//this function sends bits 0 on selected pin which causes leds to go out
void lightupwall(int x);							//this function sends bits 1 on selected pin which causes leds to light up  
void clear_all(void);								//this function sends to all of our pins bits 0 which causes leds to go out

void clearAllPixels(void * line );						// this function is setting color of all leds to 0 in the array of structures
void setPixel( void * line, int ledofchoice, uint32_t color );                  // with this we set selected color on led of our choice in the array of structures
void create_line(void * line, int len_start, uint16_t len, uint32_t color);     //with this we create stripe with specified length and position on the cube and color
void send_leds(SRGB * leds, int x);        					//with this we are sending whole array of structures to the cube

void wait(volatile uint32_t cnt);


//effects
void cycle_color_change( SRGB rgb_leds[] );        				// walls of the cube change colors in sequence red,green and blue
void flying_strips1(void * pasek);						// 2 stripes are flying in the cube leaving trace of lit up leds
void flying_strips2(void * pasek);						// 2 stripes which fly in the cube
void flying_strips3(void * pasek, int data[5][5][5]);				// stripes in diffrent colors which are circling on the sides of the cube
void disappering_cubes(void * pasek, int data[5][5][5]);


#endif
