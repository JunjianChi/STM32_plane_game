/*
 * led_driver.h
 *
 *  Created on: Mar 17, 2024
 *      Author: Junjian Chi
 */

#ifndef INC_LED_DRIVER_H_
#define INC_LED_DRIVER_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
//Functions

extern void clearscreen(void);
extern void turnoffscreen(void);
extern void turnonscreen(void);
extern void setpixel(uint8_t r, uint8_t c);
extern void addpixel(uint8_t r, uint8_t c);
extern void removepixel(uint8_t r, uint8_t c);
extern void addcar(uint8_t r, uint8_t c);
extern void removecar(uint8_t r, uint8_t c);
extern void addbadcar(uint8_t r, uint8_t c);
extern void removebadcar(uint8_t r, uint8_t c);
extern void turnonall(void);
extern void setpwm(uint8_t c);
extern void add_PWM_pixel(uint8_t pixel,int8_t brightness);
extern void turnon(void);
extern void Cube2(void);
extern void RemoveCube2(void);
extern void Cube1(void);
extern void RemoveCube1(void);


#endif /* INC_LED_DRIVER_H_ */
