/* 
© Group 7 | UoM 2018 
*/
#ifndef F_CPU
#define F_CPU 16000000UL // 16 MHz clock speed
#endif

#define D0 eS_PORTD0
#define D1 eS_PORTD1
#define D2 eS_PORTD2
#define D3 eS_PORTD3
#define D4 eS_PORTD4
#define D5 eS_PORTD5
#define D6 eS_PORTD6
#define D7 eS_PORTD7
#define RS eS_PORTC6
#define EN eS_PORTC7

#include <avr/io.h>
#include <util/delay.h>
#include "stdlib.h"
#include "string.h"
#include "lcd.h"

char disp[16]="0000000000000001";
char result[8] = "00000001"; 

void lcd_disp(char arr[],int r,int c,char w[]){  
	if(w=="clear")Lcd8_Clear();
	Lcd8_Set_Cursor(r,c);
	Lcd8_Write_String(arr);
}

void ADC_Init(){
	DDRA=0x0;			/* Make ADC port as input */
	ADCSRA = 0x87;			/* Enable ADC, fr/128  */
	ADMUX = 0x40;			/* Vref: Avcc, ADC channel: 0 */
}

int ADC_Read(char channel){
	int Ain,AinLow;
	
	ADMUX=ADMUX|(channel & 0x0f);	/* Set input channel to read */

	ADCSRA |= (1<<ADSC);		/* Start conversion */
	while((ADCSRA&(1<<ADIF))==0);	/* Monitor end of conversion interrupt */
	_delay_us(10);
	AinLow = (int)ADCL;		/* Read lower byte*/
	Ain = (int)ADCH*256;		/* Read higher 2 bits and 
					Multiply with weight */
	Ain = Ain + AinLow;				
	return(Ain);			/* Return digital value*/
}

 

int main(void){


    DDRD = 0xFF;  // #
	DDRC = 0xFF;  //for lcd
	DDRA = 0x00;  //Analog input
	
	
	ADC_Init();
	//ADMUX = 0b01100000; // Configure ADC to be left justified, use AVCC as reference, and select ADC0 as ADC input
	//ADCSRA = 0b10000111; // Enable the ADC and set the prescaler to max value (128)


	Lcd8_Init(); //Initializing the LCD screen
	lcd_disp("Starting Pulse ~)",1,0,"");
	lcd_disp("Meter",2,0,"");
	
	_delay_ms(3000);

	int i;

	for(i=0;i>=0;i++){	
		
		char temp[11]="Analog - ";
		char val[4]; // 0 - 255 value

		
		//ADCSRA = ADCSRA | (1 << ADSC);// #		
		//while(ADCSRA & (1 << ADSC)); // Wait until the ADSC bit has been cleared

		itoa(ADC_Read(0),val,10);

		strcat(temp,val);

		lcd_disp(temp,1,0,"clear");
		_delay_ms(400);
	
	}

}