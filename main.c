#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include "uart_lib.h"
#include "lcd_lib.h"
#include "adc_lib.h"
#include <avr/interrupt.h>
#define P_KEY !(PIND& (1<<0))
#define R_KEY !(PIND& (1<<1))
#define START_KEY !(PIND& (1<<2))
#define REF_256 (1<<REFS0)|(1<<REFS1) // NAPIECIE REFERENCYJNE
#define REF_VCC (1<<REFS0)|(0<<REFS1)

#define UART_BAUD 9600
#define __UBRR ((F_CPU+UART_BAUD*8UL) / (16UL*UART_BAUD)-1)

uint16_t adcValue;
uint8_t lowerBits;
volatile uint8_t preskaler=6;
volatile uint8_t ref=0;
uint8_t changed;
uint8_t running;
uint8_t licznik;
volatile uint16_t buffor[100];

void disp_menu(uint8_t preskaler, uint8_t ref){
	lcd_clr();
	lcd_text("Freq: ");
	if(preskaler==0)lcd_text("9.216 MHz");
	if(preskaler==1)lcd_text("4.608 MHz");
	if(preskaler==2)lcd_text("2.304 MHz");
	if(preskaler==3)lcd_text("1.152 MHz");
	if(preskaler==4)lcd_text("576 kHz");
	if(preskaler==5)lcd_text("288 kHz");
	if(preskaler==6)lcd_text("144 kHz");
	goto_xy(0,1);
	lcd_text("VRef: ");
	if(ref==0)lcd_text("5 V");
	if(ref==1) lcd_text("2.56 V");
}




int main(void)
{
	DDRD&=~(1<<PD0);
	PORTD|=(1<<PD0);
	DDRD&=~(1<<PD1);
	PORTD|=(1<<PD1);
	DDRD&=~(1<<PD2);
	PORTD|=(1<<PD2);

	lcdinit();
	USART_Init(__UBRR);
	DDRC|=(1<<PC0); //WLACZENIE STANU NA DIODZIE
	PORTC|=(1<<PC0);

	lcd_int(F_CPU);

	while(1){
		if(!running){
					if(P_KEY && preskaler<7){ preskaler+=1; changed=1;}
					if(preskaler==7) preskaler=0;
					if(R_KEY && ref<2){ ref+=1; changed=1;}
					if(ref==2) ref=0;
					if(changed){disp_menu(preskaler,ref); changed=0;}
					if(START_KEY){
						//TIMSK|=(0<<OCIE0);
						ADC_Init(3,0);
						running=1;
						ADC_Start();

					}
					}
					if(licznik==99){
						ADC_Stop();
						running=0;
						licznik=0;
						PORTC^=(1<<PC0);
						for(uint16_t i=0;i<99;i++){
							USART_Put_Int(buffor[i],10);
							USART_Puts("\n");

						}
					}


	}

}

ISR(ADC_vect){
	buffor[licznik]=ADCL+((uint16_t)ADCH<<8); // £ACZENIE BITOW
	licznik++;
	//35 kHz
}




