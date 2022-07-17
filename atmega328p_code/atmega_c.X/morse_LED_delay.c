/*
 * File:   newmain.c
 * Author: user
 *
 * Created on October 4, 2021, 7:01 PM
 */


#define F_CPU       16000000UL

#include <xc.h>

#define __DELAY_BACKWARD_COMPATIBLE__
#include <util/delay.h>



//#include <cstdlib>


int dly;
int timeBase = 500; //half a second
int pause = 5;  // debug was 1
int space_pause = 7; // ' '
int space_characters = 3;  //between characters of same word "AB" 

void turn_on_LED();
void turn_off_LED();
void turn_on_buzzer();
void turn_off_buzzer();

//Function prototypes
//void delay(int d); // This is the prototype declaration for the Delay function

   // set PB5 to be out 
    
  
//    DDRD = (1<<5); // bit 5 is an out. others set to 0.
//    DDRD = (1<<4);
    
 //   DDRD = 0b00110000;
    
  //  int i;
   // for (i=1; i<20 ; i++){
  
    
  //  PORTD = 0; // all are off
        
   // PORTD = (1UL<<PORT5); 
    
  //  PORTD = (1UL<< PORT4);
    
   // PORTD = (1UL<<PORT5); 
  
    
//    PORTD = 0;
//    
//    PORTD = 0b00100000; 
//    
//    PORTD = 0;
//    
//    PORTD |= 0b00100000; 
//    
//    PORTD = 0; 
//    
//    PORTD |= (1UL<<PORT5); 
//    
//    PORTD = 0;
//    

    
   // }

void encode_ch(char ch);
void encode_str(const char *str);
void encode_text(const char *text[]);

const char *text[] = { //const saves it in EEPROM
    "S",
    "O",
    "S",
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


int main(void) 
{
    DDRD = (1<<4) ; //buzzer used to be here too
    //DDRD = (1<<5);
    
    
    _delay_ms(2000);
    
    encode_text(text);
     
   _delay_ms(1000);

   // while(1);
    
    while(1)   
    { 
        
//        continue;
        
        turn_on_LED();
     
      

        _delay_ms(1 * 1000);
    
    //turn off
        turn_off_LED();
       
        _delay_ms(1 * 1000);
     
 
    
    }
    return 0;
    
}

void turn_on_LED(){
    PORTD |= (1UL<<PORT4);
}

void turn_off_LED(){
    PORTD &= ~(1UL<<PORT4);
}

//void turn_on_buzzer(){
   // PORTD |= (1UL<<PORT5);
//}

//void turn_off_buzzer(){
   // PORTD &= ~(1UL<<PORT5);
//}
       
void encode_ch(char ch)
{
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
        //turn_off_buzzer();

        dly = pause;                      // pause
        _delay_ms(timeBase * dly);
    
         i++;
         ch2 = lut_ascii_to_morse_code[index][i];
         
    }
}

// assumes input is 'A'..'Z' or ' '
void encode_str(const char *str)
{

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

     
void encode_text(const char *text[]) 
{
    int i ;

    for (i = 0; text[i] != 0; i++ ) { //using 0 instead of null


        const char *x = text[i];

        encode_str(x);

    }
}    
