/*
 * File:   morse_code_interrupt.c
 * Author: user macbook
 *
 * Created on nov 20, 2021
 */

#define F_CPU       16000000UL
#include <xc.h>
//#include <util/delay.h>
#include <sys/attribs.h>



#define TIMER_VAL (60000)



int dly;
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
void __ISR(_TIMER_1_VECTOR, IPL5SOFT) isr(void) 
{
        
     TMR2 = TIMER_VAL; //when tcn1 is 0, the interrupt executed 
     
     timer_flag = 1;
    
}

int main(void) 
{
   // DDRD = (1 << 4) | (1 << 5) ;
    
    LATA = 0;
       
    TMR2 = TIMER_VAL;
    
    TMR1 = 0x00; //timer counter register reset to 0
    
    TMR1 = (1 << SS1R) | (1 << SS1R); //
    
    IFS1 = (1 << T1CON);
    
    asm volatile("ei");
        
    encode_text(text);
   
    while(1); //DEBUG

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
   // PORTD &= ~(1UL<<PORT5);
//}


void encode_ch(char ch){
    //look up table
    //first convert any small letters to capitals

    ch &= 0xDF;
    char dly;
    //int index = ch - 'A';
    
        asm ("lw $v0, ch \n");
        asm ("sub ch, A  \n" ); 
        
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


       // dly = ch2 - '0';
        

         asm("lw $t1, ch2 \n" 
        "sub $t1, $t1, 0 \n"
        "sw $t1,dly \n");
        // converts from ascii to number //inline
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

