/*******************************************************************************
* This is the sample source code to control FR02A + PIC16F887 using SK40C
* In this example, SW1 and SW2 are used to do Frequency Search
* SW1	- Frequency search up
* SW2	- Frequency search down

* AUTHOR   : Tony Ng Wei Kang
* COMPANY  : Cytron Technologies Sdn Bhd
* REVISION : 1.0
* DATE     : 20/05/2014
* Software : MPLABX
* Complier : XC8 (v1.21)
*******************************************************************************/

//	include
//==========================================================================
#include <xc.h>
#include "i2c.h"
#include "system.h"
#include "lcd.h"

//   Configuration
//==========================================================================
#pragma config BOREN = OFF
#pragma config CPD = OFF
#pragma config WRT = OFF
#pragma config FOSC = HS
#pragma config WDTE = OFF
#pragma config CP = OFF
#pragma config LVP = OFF
#pragma config PWRTE = OFF

//==========================================================================//
//	Define
//==========================================================================//
#define Address_W   0xC0        // Address Write
#define Address_R   0xC1        // Address Read

//==========================================================================//
//	function prototype
//==========================================================================//
void Freq_Display(void);
void SetFrequency(double frequency);
double Freq_Available();
void SearchUp();
void SearchDown();

//==========================================================================//
//	main function
//==========================================================================//
void main() {

    PORTB = 0;
    PORTC = 0;
    PORTD = 0;

    TRISB = 0b00000011;
    TRISD = 0;
    
    ANSELH = 0;

    /* Initialize*/
    i2c_initialize();
    lcd_initialize();
    lcd_clear();

    lcd_home();
    lcd_putstr("Freq:   . ");

    SetFrequency(92.8);         //Default Frequency 92.8
    
    while(1)  {
        if(SW1 == 0) {              //Press SW1 to Search Up
            SearchUp();
        }
        if(SW2 == 0) {              //Press SW2 to Search Down
            SearchDown();
        }
        Freq_Display();
    }
}

//==========================================================================//
// Function Purpose: Display the current FM channel
//==========================================================================//
void Freq_Display(void) {
    unsigned int freq_available = 0;

    lcd_goto(0x05);
    lcd_bcd(3,Freq_Available());

    freq_available = Freq_Available() * 10;     //eg: 92.8
    lcd_goto(0x09);
    lcd_bcd(1,freq_available%10);
}

//==========================================================================//
// Function Purpose: Set FM Radio Frequency
//==========================================================================//
void SetFrequency(double frequency) {

    unsigned char frequencyH=0;
    unsigned char frequencyL=0;
    unsigned int frequencyB;

    frequencyB = (4 * (frequency * 1000000 + 225000)) / 32768; //calculating PLL word
    frequencyH = frequencyB >> 8;
    frequencyL = frequencyB & 0xff;

    I2C_Start();
    I2C_Write_Byte(Address_W);       
    I2C_Send_ACK();
    I2C_Write_Byte(frequencyH);
    I2C_Write_Byte(frequencyL);
    I2C_Write_Byte(0xB0);
    I2C_Write_Byte(0x10);
    I2C_Write_Byte(0x00);
    I2C_Send_NACK();
    I2C_Stop();
}

//==========================================================================//
//Function : Check the current frequency
//==========================================================================//
double Freq_Available() {
  unsigned char buffer[5] , i;
  double freq_available = 0;

    i2c_initialize();
    
    I2C_Start();
    I2C_Write_Byte(Address_R);
    I2C_Send_ACK();
    for (i=0; i<5; i++) {
        buffer[i]= I2C_Read_Byte();       //Read 5 Bytes from TEA5767
        I2C_Send_ACK();
    }
    I2C_Send_NACK();
    I2C_Stop();

    freq_available = (((buffer[0]&0x3F)<<8)+buffer[5])*32768/4-225000;    //Current Freq is in Buffer[0] & [1]
    freq_available = freq_available/1000000;

    return freq_available;
}

//==========================================================================//
//Function : Search up form current FM frequency
//==========================================================================//
void SearchUp() {
    double frequency = 0;
    unsigned char frequencyH=0;
    unsigned char frequencyL=0;
    unsigned int frequencyB;

    frequency = Freq_Available();

    frequencyB = (4 * (frequency * 1000000 + 225000)) / 32768 + 1; //calculating PLL word
    frequencyH = frequencyB >> 8;
    frequencyL = frequencyB & 0xff;

    I2C_Start();
    I2C_Write_Byte(Address_W);
    I2C_Send_ACK();
    I2C_Write_Byte(frequencyH + 0x40); // On Search mode
    I2C_Write_Byte(frequencyL);
    I2C_Write_Byte(0xD0);       // Search UP
    I2C_Write_Byte(0x1F);
    I2C_Write_Byte(0x00);
    I2C_Send_NACK();
    I2C_Stop();
}

//==========================================================================//
//Function : Search down from current frequency
//==========================================================================//
void SearchDown() {
    double frequency = 0;

    frequency = Freq_Available();

    unsigned char frequencyH=0;
    unsigned char frequencyL=0;
    unsigned int frequencyB;

    frequencyB = (4 * (frequency * 1000000 + 225000)) / 32768 + 1; //calculating PLL word
    frequencyH = frequencyB >> 8;
    frequencyL = frequencyB & 0xff;

    I2C_Start();
    I2C_Write_Byte(Address_W);
    I2C_Send_ACK();
    I2C_Write_Byte(frequencyH + 0x40); // On Search mode
    I2C_Write_Byte(frequencyL);
    I2C_Write_Byte(0x50);             // Search Down
    I2C_Write_Byte(0x1F);
    I2C_Write_Byte(0x00);
    I2C_Send_NACK();
    I2C_Stop();
}