#include <avr/io.h>
#include <stdlib.h>
#include <avr/interrupt.h>

#define REF_256 (1<<REFS0)|(1<<REFS1) // NAPIECIE REFERENCYJNE
#define REF_VCC (1<<REFS0)|(0<<REFS1)

void ADC_Init(uint8_t preskaler, uint8_t ref){

	DDRC|=(1<<PC0); //WLACZENIE STANU NA DIODZIE
	PORTC|=(1<<PC0);

	if(preskaler==0)ADCSRA|=(0<<ADPS2)|(0<<ADPS1)|(1<<ADPS0);//2
	if(preskaler==1)ADCSRA|=(0<<ADPS2)|(1<<ADPS1)|(0<<ADPS0);//4
	if(preskaler==2)ADCSRA|=(0<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);//8
	if(preskaler==3)ADCSRA|=(1<<ADPS2)|(0<<ADPS1)|(0<<ADPS0);//16
	if(preskaler==4)ADCSRA|=(1<<ADPS2)|(0<<ADPS1)|(1<<ADPS0);//32
	if(preskaler==5)ADCSRA|=(1<<ADPS2)|(1<<ADPS1)|(0<<ADPS0);//64
	if(preskaler==6)ADCSRA|=(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);//128
	//ADMUX|=(1<<ADLAR); //USTAWIENEI DOMYSLNE ADLAR=0, ODPOWIADA ZA KOLEJNOSC BITOW
	if(ref==0)ADMUX|=REF_VCC;
	if(ref==1)ADMUX|=REF_256;// NAPIECIE REFERENCYJNE

}

void ADC_Start(void){
	ADCSRA|=(1<<ADATE);
	ADCSRA|=(1<<ADIE); //WY£¥CZENIE PRZERWAN ADC
	sei(); // URUCHOMIENIE GLOBALNYCH PRZERWAN
	ADCSRA|=(1<<ADSC); // PIERWSZA KONWERSJA
	ADCSRA|=(1<<ADEN);// WY£ACZENIE ADC
}

void ADC_Stop(void){
	cli();
	ADCSRA|=(0<<ADIE); //WY£¥CZENIE PRZERWAN ADC
	ADCSRA|=(0<<ADEN); // WY£ACZENIE ADC
}
