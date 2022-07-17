/*
 * File:   newmain.c
 * Author: user
 *
 * Created on October 4, 2021, 7:01 PM
 */


#define F_CPU       16000000UL

#include <xc.h>
#include <stdlib.h>

#define __DELAY_BACKWARD_COMPATIBLE__


using namespace std;
//--
#pragma config   JTAGEN    = OFF    // JTAG Enable OFF (only use for '250)
#pragma config   FNOSC     = FRCPLL // Fast RC w PLL 8mHz internal rc Osc
#pragma config   FPLLIDIV  = DIV_10  // PLL in 8mHz/10 = 800 kHz
#pragma config   FPLLMUL   = MUL_15 // PLL mul 800 khz * 15 = 12 Mhz
#pragma config   FPLLODIV  = DIV_64  // PLL Out: 12MHz / 64 187.5 kHz
#pragma config   FPBDIV    = DIV_1  // Peripheral Bus Divisor
#pragma config   FCKSM     = CSECME // Clock Switch Enable, FSCM Enabled
#pragma config   POSCMOD   = OFF    // Primary osc disabled
#pragma config   IESO      = OFF    // Internal/external switch over
#pragma config   OSCIOFNC  = OFF    // CLKO Output Signal Active on the OSCO Pin
#pragma config   FWDTEN    = OFF 

//--


//#include <cstdlib>


int dly;
int timeBase = 500; //half a second
int pause = 5;  // debug was 1
int space_pause = 7; // ' '
int space_characters = 3;  //between characters of same word "AB" 

void turn_on_LED();
void turn_off_LED();
//void turn_on_buzzer();
//void turn_off_buzzer();



void encode_ch(char ch);
void encode_str(const char *str);
void encode_text(const char *text[]);

const char *text[] = { //const saves it in EEPROM
    "A",
    "BBB",
    "C",
    0
};

 const char lut_ascii_to_morse_code[][6] = {
        "13",   // {'1', '3', 0, 0, 0}, //, //A -> 0 
        "3111", // B
        "3131", // C .    
        "311", // D
        "1", //E
        "1131", //F
        "331",//G
        "1111", // H
        "11",
        "1333",
        "313",
        "1311", //L
        "33",
        "31",
        "333",
        "1331",
        "3313", //Q
        "131",//R
        "111",//S
        "3",//T
        "113",//U
        "1113",//V
        "133", //W
        "3113",//X
        "3133",//Y
        "3311"//Z             
    };

 int global_ctr = 0;                 // a global variable counter.
int global_delay_val = 1; 

// Function prototypes
void _delay_ms(int d); // This is the prototype declaration for the Delay function


int main(int argc, char** argv)
{
    // Set Port A to be output, in two steps.
	LATA = 0;				// Set value of PORT A output to 0. 
	TRISA = 0;				// Set all pins on PORT A to output
    
    
    _delay_ms(2000);

//    encode_ch('Z');
    
//    encode_str("ABC D");
    
    encode_text(text);
    
    
    _delay_ms(1000);
    
    //encode_ch('E');
    
   // _delay_ms(1000);
    
  //  encode_ch('Z');

  
    while(1);
    
    while(1)   
    { 
        
//        continue;
        
        turn_on_LED();
        //turn_on_buzzer();
      

        _delay_ms(1 * 1000);
    
    //turn off
        turn_off_LED();
       // turn_off_buzzer();
        _delay_ms(1 * 1000);
     
     //try a dash?
            
   // _delay_ms(3 * 10000);
    
    }
    return 0;
    
}

void turn_on_LED(){
    LATA = 0x0001;	
}

void turn_off_LED(){
    LATA = 0x0000;
}

//void turn_on_buzzer(){
   // PORTD |= (1UL<<PORT5);
//}

//void turn_off_buzzer(){
    //PORTD &= ~(1UL<<PORT5);
//}
       
void encode_ch(char ch){
    //look up table
    //first convert any small letters to capitals

    ch &= 0xDF;

    //
    int index = ch - 'A';
    char ch2 = lut_ascii_to_morse_code[index][0];
    int i = 0;

    while(ch2){
        
        // turn on buzzer orled 
        turn_on_LED();
        //turn_on_buzzer();
        
        // example
        // *p = '3'
        // '3' - '0' -> 3
        // '8' - '0' -> 8


        dly = ch2 - '0';  // converts from ascii to number //inline
        _delay_ms(timeBase * dly);
        
        // turn off buzzer or led
        
        turn_off_LED();
       // turn_off_buzzer();

        dly = pause;                      // pause
        _delay_ms(timeBase * dly);
    
         i++;
         ch2 = lut_ascii_to_morse_code[index][i];
         
    }
}

// assumes input is 'A'..'Z' or ' '
void encode_str(const char *str) {

    const char *p = str;

    while(*p) {

        if (*p == ' ')
            _delay_ms(timeBase * space_pause);

        else {

            encode_ch(*p);
        
            _delay_ms(timeBase*space_characters);   
            
        }

        p++;
    }
}

     
void encode_text(const char *text[]) {
    int i ;

    for (i = 0; text[i] != 0; i++ ) { //using 0 instead of null


        const char *x = text[i];

        encode_str(x);

    }
}    

void _delay_ms(int d)
{
    int i;  // Declare variable to be used in the loop

    while(d)    // While d > 0
    {
        i = 5;    // set i to 100 for inner loop
        while(i)    // while i > 0
        {
            i--;    // decrement i (e.g. i will equal 99, 98, 97,...)
        }

        d--;    // decrement d
    }
}