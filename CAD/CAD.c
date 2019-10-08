/*
 * CAD.c
 *
 * Created: 10/04/2018 02:16:17 p. m.
 * Author: Cesar
 */

#include <90usb1286.h>
#include <io.h>
#include <stdio.h>
#include <delay.h> 

int i=0,h=0,j=35;
int comacounter = 0;

 
void main(void)
{
char dato;
char resp;
char GPS [254];
char car;
char quotes = '"';
int pressed =0;



    
    // USART1 initialization
    // Communication Parameters: 8 Data, 1 Stop, No Parity
    // USART1 Receiver: On
    // USART1 Transmitter: On
    // USART1 Mode: Asynchronous
    // USART1 Baud Rate: 9600
    UCSR1A=(0<<RXC1) | (0<<TXC1) | (0<<UDRE1) | (0<<FE1) | (0<<DOR1) | (0<<UPE1) | (0<<U2X1) | (0<<MPCM1);
    UCSR1B=(0<<RXCIE1) | (0<<TXCIE1) | (0<<UDRIE1) | (1<<RXEN1) | (1<<TXEN1) | (0<<UCSZ12) | (0<<RXB81) | (0<<TXB81);
    UCSR1C=(0<<UMSEL11) | (0<<UMSEL10) | (0<<UPM11) | (0<<UPM10) | (0<<USBS1) | (1<<UCSZ11) | (1<<UCSZ10) | (0<<UCPOL1);
    UBRR1H=0x00;
    UBRR1L=0x0C; 
       


DDRC.7=0;
PORTC.7=1;
DDRC.6=0;
PORTC.6=1;
DDRC.5=1;
DDRC.4=1;

    
 
   
   printf("AT\n\r");
    
    //delay_ms(100);
       dato =getchar();
     
      
    while(dato != 'O')
    {

        delay_ms(100);
        i++; 
        if(i>=5)
        {
            break;
        } 
    }
      
    
    printf("AT+CGPSPWR=1\n\r");
    
   resp = getchar();
    
      
    while(resp != 'O')
    {
        delay_ms(100);
        h++; 
        if(h>=5)
        {
            break;
        } 
    }
          
    printf("AT+CGPSSTATUS?\n\r");   
                   
    delay_ms(100);
        
    printf("AT+COPS?\n\r");
    delay_ms(100);    
    printf("AT+CMGF=1\n\r");    

   
  PORTC.5=1;

while (1)
    {     
    

     
    
    if(0==PINC.7 && 0==pressed)
    { 
    delay_ms(200); 
    pressed = 1;
        
    printf("AT+CGPSINF=0\n\r");
     
    while(comacounter < 4)
    {
        GPS[h] = getchar();
        if(GPS[h] == ',')
        {
         comacounter++;
        
        } 
        h++;
        
    }    
         
     delay_ms(200);   
     
     printf("AT+CMGS=%c4427081897%c\n\r",quotes,quotes);     
     delay_ms(100);  
     comacounter=0;
     while(GPS[j] != 0x00)
     {
        car =  GPS[j];
        printf("%c",car); 
 
            j++; 
     }    
     delay_ms(100);
     putchar(0x1A);   
     delay_ms(500);
     PORTC.4=1;
 
        } 
                 
    
        
      
    
}
}
