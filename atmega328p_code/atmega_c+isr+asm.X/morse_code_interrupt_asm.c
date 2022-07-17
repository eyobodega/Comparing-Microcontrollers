/*
 * File:   morse_code_interrupt.c
 * Author: User macbook
 *
 * Created on dec 1, 2021,
 */

#define F_CPU       16000000UL
//#include <xc.h>
//#include <util/delay.h>
#include <avr/io.h> //avr io lib
#include <avr/interrupt.h>



#define  TIMER_VAL (60000)



//int dly;
int timeBase = 500; //half a second
int pause = 5;  // debug was 1
int space_pause = 7; // ' '
int space_characters = 3;  //between characters of same word "AB" 



//call these global - good practise 
volatile int timer_flag = 0;
int tmr_counter = 3;

void turn_on_LED();
void turn_off_LED();
void turn_on_buzzer();
void turn_off_buzzer();


void encode_ch(char ch);
void encode_str(const char *str);
void encode_text(const char *text[]);


void my_delay(int delay); //prototype

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
 

// timer1 isr
ISR(TIMER1_OVF_vect) {
        
     TCNT1 = TIMER_VAL; //when tcn1 is 0, the interrupt executed 
     
     timer_flag = 1;
    
}

int main(void) 
{
    DDRD = (1 << 4); //| (1 << 5) ; 
    
    PORTD = 0;
       
    TCNT1 = TIMER_VAL;
    
    TCCR1A = 0x00; //timer counter register reset to 0
    
    TCCR1B = (1 << CS10) | (1 << CS12); //
    
    TIMSK1 = (1 << TOIE1);
    
    sei();

        
    encode_text(text);
   
    while(1); //DEBUG

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
    unsigned char index = 1; // debug was int
    char dly;

    ch &= 0xDF;




    asm ("mov  r22, %1     \n"
         "subi r22, 'A'    \n"
         "mov  %0, r22     \n"
         : "=r" (index)
         : "r" (ch)
         : "r22");


    
    
    

    char ch2 = lut_ascii_to_morse_code[index][0];
    int i = 0;
  
    while(ch2){
        
        // turn on buzzer orled 
        turn_on_LED();
       // turn_on_buzzer();
        
        // example
        // *p = '3'
        // '3' - '0' -> 3
        // '8' - '0' -> 8


//        dly = ch2 - '0';  // converts from ascii to number //inline
        
        asm("mov  r22, %1     \n"
           "subi r22, '0'    \n"
           "mov  %0, r22     \n"
           : "=r" (dly)
           : "r" (ch2)
           : "r22");
 
        my_delay(dly);
        
        // turn off buzzer or led
        
        turn_off_LED();
       // turn_off_buzzer();

        dly = pause;                      // pause
        my_delay(dly);
              
    
        i++;
        ch2 = lut_ascii_to_morse_code[index][i];
                  
    }
}

// assumes input is 'A'..'Z' or ' '
void encode_str(const char *str) {

    const char *p = str;

    while(*p) {

        if (*p == ' ')
            my_delay(space_pause);

        else {

            encode_ch(*p);
        
            my_delay(space_characters);   
            
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

void my_delay(int delay) {
    
    while (1) {
    
        if (timer_flag)
        {
            timer_flag = 0; 
            delay--;

            if (delay == 0)
            {
                break;
            }
        }    
    }
}

