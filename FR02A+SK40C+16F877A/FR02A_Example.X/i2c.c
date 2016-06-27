
#include <xc.h>
#include "i2c.h"
#include "system.h"

/*******************************************************************************
* PRIVATE GLOBAL VARIABLES                                                     *
*******************************************************************************/
#define _XTAL_FREQ 20000000

// Function Purpose: I2C Initialization
void i2c_initialize(void)
{

    TRISCbits.TRISC3 = 1;		// Set the SCL as output.
    TRISCbits.TRISC4 = 1;		// Set the SDA pin as Input / Output.

    // Set the I2C clock frequency.
    // 400kHz, SSPADD = (FOSC / (4 * F_I2C) ) - 1
    SSPADD = 12;

    // Select I2C master mode, clock = FOSC / (4 * (SSPADD+1) ).
    SSPCONbits.SSPM3 = 1;
    SSPCONbits.SSPM2 = 0;
    SSPCONbits.SSPM1 = 0;
    SSPCONbits.SSPM0 = 0;

    // Clear the Write Collision Detect bit.
    SSPCONbits.WCOL = 0;

    // Clear the Receive Overflow Indicator bit.
    SSPCONbits.SSPOV = 0;

    // Enable the MSSP module.
    SSPCONbits.SSPEN = 1;
}


// Function Purpose: I2C_Start sends start bit sequence
void I2C_Start(void)
{
    SSPCON2bits.SEN = 1;
    // Send start bit
    while((SSPCON2bits.SEN || SSPSTATbits.R_W) == 1);
}


// Function Purpose: I2C_ReStart sends start bit sequence
void I2C_ReStart(void)
{
    SSPCON2bits.RSEN = 1;			// Send Restart bit
    while((SSPCON2bits.RSEN || SSPSTATbits.R_W) == 1);
}


//Function : I2C_Stop sends stop bit sequence
void I2C_Stop(void)
{
    SSPCON2bits.PEN = 1;			// Send stop bit
    while((SSPCON2bits.PEN || SSPSTATbits.R_W) == 1);
}


//Function : I2C_Send_ACK sends ACK bit sequence
void I2C_Send_ACK(void)
{
    while(SSPSTATbits.R_W == 1);
    if(SSPCON2bits.ACKSTAT == 1)    {
        SSPCON2bits.PEN = 1;
        while((SSPCON2bits.PEN || SSPSTATbits.R_W) == 1);
    }
}


//Function : I2C_Send_NACK sends NACK bit sequence
void I2C_Send_NACK(void)
{
    while(SSPSTATbits.R_W == 1);
    SSPCON2bits.ACKDT = 1;			// 1 means NACK
    SSPCON2bits.ACKEN = 1;			// Send ACKDT value
    while((SSPCON2bits.ACKEN || SSPSTATbits.R_W)== 1);
}


// Function Purpose: I2C_Write_Byte transfers one byte
bit I2C_Write_Byte(unsigned char Byte)
{
    SSPBUF = Byte;		// Send Byte value
    while(SSPSTATbits.BF == 1);

    return SSPCON2bits.ACKSTAT;		// Return ACK/NACK from slave
}


// Function Purpose: I2C_Read_Byte reads one byte
unsigned char I2C_Read_Byte(void)
{
    SSPCON2bits.RCEN = 1;			// Enable reception of 8 bits
    while(SSPSTATbits.BF == 0);
    SSPCON2bits.RCEN = 0;

    return SSPBUF;		// Return received byte
}