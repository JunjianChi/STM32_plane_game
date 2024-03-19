/*
 * led_driver.c
 *
 *  Created on: Mar 17, 2024
 *      Author: Junjian Chi
 */


#include "led_driver.h"

#include "main.h"
uint8_t screenstatus[11];
static const uint8_t LEDMAT_ADD = 0x75 << 1;
static const uint8_t PAGE_1 = 0x00;
//static const uint8_t PAGE_2 = 0x01;
//static const uint8_t PAGE_3 = 0x02;
//static const uint8_t PAGE_4 = 0x03;
//static const uint8_t PAGE_5 = 0x04;
//static const uint8_t PAGE_6 = 0x05;
//static const uint8_t PAGE_7 = 0x06;
//static const uint8_t PAGE_8 = 0x07;
static const uint8_t FUN_REG = 0x0B;
static const uint8_t COM_REG = 0xFD;
static const uint8_t MAT_ROW[11] = {0x00, 0x02, 0x04, 0x06, 0x08, 0x0A, 0x01, 0x03, 0x05, 0x07, 0x09};
static const uint8_t MAT_COL[7] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40};

//static const uint8_t PWM_address[11][7] = {
//	{0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A},
//    {0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A},
//	{0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A},
//	{0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A},
//	{0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A},
//	{0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7A},
//	{0x2C, 0x2D, 0x2E, 0x2F, 0x30, 0x31, 0x32},
//	{0x3C, 0x3D, 0x3E, 0x3F, 0x40, 0x41, 0x42},
//	{0x4C, 0x4D, 0x4E, 0x4F, 0x50, 0x51, 0x52},
//	{0x5C, 0x5D, 0x5E, 0x5F, 0x60, 0x61, 0x62},
//	{0x6C, 0x6D, 0x6E, 0x6F, 0x70, 0x71, 0x72},
//};

// Assuming data array is declared globally
uint8_t data[2]; // Modify the size if necessary



void setpwm(uint8_t c){
	for(uint8_t r = 0x24; r < 0xB3; r++){
		data[0] = COM_REG;
		data[1] = PAGE_1;
		HAL_I2C_Master_Transmit(&hi2c1, LEDMAT_ADD, data, 2, HAL_MAX_DELAY);
		//HAL_Delay(10);
		data[0] = r;
		data[1] = c;
		HAL_I2C_Master_Transmit(&hi2c1, LEDMAT_ADD, data, 2, HAL_MAX_DELAY);
	}
}


void clearscreen() {
    for(uint8_t r = 0; r < 11; r++) {
        data[0] = COM_REG;
        data[1] = PAGE_1;
        HAL_I2C_Master_Transmit(&hi2c1, LEDMAT_ADD, data, 2, HAL_MAX_DELAY);
        //HAL_Delay(10);
        data[0] = MAT_ROW[r];
        data[1] = 0x00;
        HAL_I2C_Master_Transmit(&hi2c1, LEDMAT_ADD, data, 2, HAL_MAX_DELAY);
        //HAL_Delay(10);
    }
}

void turnoffscreen() {
    data[0] = COM_REG;
    data[1] = FUN_REG;
    HAL_I2C_Master_Transmit(&hi2c1, LEDMAT_ADD, data, 2, HAL_MAX_DELAY);
    //HAL_Delay(10);
    data[0] = 0x0A; //shutdown on/off
    data[1] = 0x00; //off
    HAL_I2C_Master_Transmit(&hi2c1, LEDMAT_ADD, data, 2, HAL_MAX_DELAY);
}

void turnonscreen() {
    data[0] = COM_REG;
    data[1] = FUN_REG;
    HAL_I2C_Master_Transmit(&hi2c1, LEDMAT_ADD, data, 2, HAL_MAX_DELAY);
    //HAL_Delay(10);
    data[0] = 0x0A; //shutdown on/off
    data[1] = 0x01; //on
    HAL_I2C_Master_Transmit(&hi2c1, LEDMAT_ADD, data, 2, HAL_MAX_DELAY);
}

void turnon(){
	data[0] = COM_REG;
	    data[1] = FUN_REG;
	    HAL_I2C_Master_Transmit(&hi2c1, LEDMAT_ADD, data, 2, HAL_MAX_DELAY);
	    //HAL_Delay(10);
	    data[0] = 0x09; //shutdown on/off
	    data[1] = 0x01; //on
	    HAL_I2C_Master_Transmit(&hi2c1, LEDMAT_ADD, data, 2, HAL_MAX_DELAY);
}

void turnonall() {
    // Iterate through each row
    for (uint8_t row = 0; row < 11; row++) {
        data[0] = COM_REG;
        data[1] = PAGE_1;
        HAL_I2C_Master_Transmit(&hi2c1, LEDMAT_ADD, data, 2, HAL_MAX_DELAY);

        // Set all columns to turn on LEDs
        data[0] = MAT_ROW[row];
        data[1] = 0xFF; // Turn on all LEDs in the row
        HAL_I2C_Master_Transmit(&hi2c1, LEDMAT_ADD, data, 2, HAL_MAX_DELAY);
    }
}


void setpixel(uint8_t r, uint8_t c) {
    data[0] = COM_REG;
    data[1] = PAGE_1;
    HAL_I2C_Master_Transmit(&hi2c1, LEDMAT_ADD, data, 2, HAL_MAX_DELAY);
    //HAL_Delay(10);
    data[0] = MAT_ROW[r];
    data[1] = MAT_COL[c];
    HAL_I2C_Master_Transmit(&hi2c1, LEDMAT_ADD, data, 2, HAL_MAX_DELAY);
}

void addpixel(uint8_t r, uint8_t c){
	screenstatus[r] |= MAT_COL[c];
	data[0] = COM_REG;
	data[1] = PAGE_1;
	HAL_I2C_Master_Transmit(&hi2c1, LEDMAT_ADD, data, 2, HAL_MAX_DELAY);
	data[0] = MAT_ROW[r];
	data[1] = screenstatus[r];
	HAL_I2C_Master_Transmit(&hi2c1, LEDMAT_ADD, data, 2, HAL_MAX_DELAY);
//	data[0] = 0x25;
//	data[1] = 0b00101011;
	HAL_I2C_Master_Transmit(&hi2c1, LEDMAT_ADD, data, 2, HAL_MAX_DELAY);

}
void removepixel(uint8_t r, uint8_t c){
	screenstatus[r] &= ~MAT_COL[c];
	data[0] = COM_REG;
	data[1] = PAGE_1;
	HAL_I2C_Master_Transmit(&hi2c1, LEDMAT_ADD, data, 2, HAL_MAX_DELAY);
	data[0] = MAT_ROW[r];
	data[1] = screenstatus[r];
	HAL_I2C_Master_Transmit(&hi2c1, LEDMAT_ADD, data, 2, HAL_MAX_DELAY);
}
void addcar(uint8_t r, uint8_t c){
	if(r>7) r=7;
	if(c>5) c=5;
	if(c<1) c=1;
	addpixel(r,c-1); addpixel(r,c); addpixel(r,c+1);
	addpixel(r+1, c);
	addpixel(r+2,c-1); addpixel(r+2,c); addpixel(r+2,c+1);
	addpixel(r+3, c);
}


void removecar(uint8_t r, uint8_t c){
	if(r>7) r=7;
	if(c>5) c=5;
	if(c<1) c=1;
	removepixel(r,c-1); removepixel(r,c); removepixel(r,c+1);
	removepixel(r+1, c);
	removepixel(r+2,c-1); removepixel(r+2,c); removepixel(r+2,c+1);
	removepixel(r+3, c);
}
void addbadcar(uint8_t r, uint8_t c){
	if(c>5) c=5;
	if(c<1) c=1;
	addpixel(r,c-1); addpixel(r,c); addpixel(r,c+1);
	if(r>0) addpixel(r-1, c);
	if(r>1) {addpixel(r-2,c-1); addpixel(r-2,c); addpixel(r-2,c+1);}
	if(r>2) addpixel(r-3, c);
}
void removebadcar(uint8_t r, uint8_t c){
	if(c>5) c=5;
	if(c<1) c=1;
	removepixel(r,c-1); removepixel(r,c); removepixel(r,c+1);
	if(r>0) removepixel(r-1, c);
	if(r>1) {removepixel(r-2,c-1); removepixel(r-2,c); removepixel(r-2,c+1);}
	if(r>2) removepixel(r-3, c);
}

void Cube2(void){
	data[0] = COM_REG;
	data[1] = PAGE_1;
	HAL_I2C_Master_Transmit(&hi2c1, LEDMAT_ADD, data, 2, HAL_MAX_DELAY);
	//HAL_Delay(10);
	data[0] = MAT_ROW[10];
	data[1] = 0x18;
	HAL_I2C_Master_Transmit(&hi2c1, LEDMAT_ADD, data, 2, HAL_MAX_DELAY);
	data[0] = COM_REG;
	data[1] = PAGE_1;
	HAL_I2C_Master_Transmit(&hi2c1, LEDMAT_ADD, data, 2, HAL_MAX_DELAY);
	//HAL_Delay(10);
	data[0] = MAT_ROW[9];
	data[1] = 0x18;
	HAL_I2C_Master_Transmit(&hi2c1, LEDMAT_ADD, data, 2, HAL_MAX_DELAY);
}
void RemoveCube2(void){
	data[0] = COM_REG;
	data[1] = PAGE_1;
	HAL_I2C_Master_Transmit(&hi2c1, LEDMAT_ADD, data, 2, HAL_MAX_DELAY);
	//HAL_Delay(10);
	data[0] = MAT_ROW[10];
	data[1] = 0x00;
	HAL_I2C_Master_Transmit(&hi2c1, LEDMAT_ADD, data, 2, HAL_MAX_DELAY);
	data[0] = COM_REG;
	data[1] = PAGE_1;
	HAL_I2C_Master_Transmit(&hi2c1, LEDMAT_ADD, data, 2, HAL_MAX_DELAY);
	//HAL_Delay(10);
	data[0] = MAT_ROW[9];
	data[1] = 0x00;
	HAL_I2C_Master_Transmit(&hi2c1, LEDMAT_ADD, data, 2, HAL_MAX_DELAY);
}
