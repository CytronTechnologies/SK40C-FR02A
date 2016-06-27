
#ifndef _I2C_H
#define _I2C_H

// Function Purpose: I2C Initialization
extern void i2c_initialize(void);

// Function Purpose: I2C_Start sends start bit sequence
extern void I2C_Start(void);


// Function Purpose: I2C_ReStart sends start bit sequence
extern void I2C_ReStart(void);


//Function : I2C_Stop sends stop bit sequence
extern void I2C_Stop(void);


//Function : I2C_Send_ACK sends ACK bit sequence
extern void I2C_Send_ACK(void);


//Function : I2C_Send_NACK sends NACK bit sequence
extern void I2C_Send_NACK(void);


// Function Purpose: I2C_Write_Byte transfers one byte
extern bit I2C_Write_Byte(unsigned char Byte);


// Function Purpose: I2C_Read_Byte reads one byte
unsigned char I2C_Read_Byte(void);

#endif
