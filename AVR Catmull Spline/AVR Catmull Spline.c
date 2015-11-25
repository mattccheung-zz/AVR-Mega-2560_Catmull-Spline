/*
 * AVR_Catmull_Spline.c
 *
 * Created: 4/7/2015 12:14:23 PM
 *  Author: Chiuyin
 */ 


#include <avr/io.h>
#include <stdlib.h>
#include "spline.h"

#define BUF_SIZE 10

float theta[13] = {-30, -5, 10, 45, 90, 135, 190, 270, 299, 330, 355, 370, 405}; //9-point interpolation with 2 repeated points before and after, for use in catmull interpolation
float height[13] = {990, 1050, 1025, 990, 860, 780, 700, 812, 890, 990, 1050, 1025, 990};


void USARTInit(uint16_t ubrr_value)
{

   //Set Baud rate

   UBRR0L = ubrr_value;
   UBRR0H = (ubrr_value>>8);

   /*Set Frame Format


   >> Asynchronous mode
   >> No Parity
   >> 1 StopBit

   >> char size 8

   */

   UCSR0C=(1<<USBS0)|(3<<UCSZ00);

   UCSR0B=(1<<RXEN0)|(1<<TXEN0);  //Enable The receiver and transmitter


}

char USARTReadChar()
{
	//Wait untill a data is available

	while(!(UCSR0A & (1<<RXC0)))
	{
		//Do nothing
	}

	//Now USART has got data from host
	//and is available is buffer

	return UDR0;
}


//This fuction writes the given "data" to
//the USART which then transmit it via TX line
void USARTWriteChar(char data)
{
	//Wait untill the transmitter is ready

	while(!(UCSR0A & (1<<UDRE0)))
	{
		//Do nothing
	}

	//Now write the data to USART buffer

	UDR0=data;
}


void USARTWriteStr(char *txt)
{
	while(*txt)
	{
		USARTWriteChar(*txt);
		txt++;
	}
}

int main(void)
{
   char data;

   /*First Initialize the USART with baud rate = 19200bps
   
   for Baud rate = 19200bps

   UBRR value = 51

   */
	SplineInit(theta, height, 13, Catmull);
    USARTInit(51);    //UBRR = 51
   
     for( int i = 0; i <= 360; i+= 1 ) {
	     float temp = SplineValue(i);
		 char* display_i[5]; //to hold string to display theta
		 char* display_temp[BUF_SIZE]; //to hold string to display height
		 itoa(i,display_i,10); //convert int to string for USART transmission
		 //snprintf(display_temp, BUF_SIZE, "%f", temp);
		 dtostrf(temp,7,2,display_temp); //convert float to string for USART transmission
	     
		 USARTWriteStr(display_i);
		
	     USARTWriteStr(" ");
	     USARTWriteStr(display_temp);
		 USARTWriteStr("\r");
     }
	while(1)
    {
        //TODO:: Please write your application code 
    }
}