// Teensy I/O pins definition

#ifndef _ioPins_h_
#define _ioPins_h_

// INPUT, active low - slam switch
#define SLAM_PIN 2

// INPUT PULLUP, active high - "Next" control button
#define NEXTB_PIN 3

// INPUT PULLUP, active high - "Enter" control button
#define ENTRB_PIN 4

// INPUT PULLUP, active high - "Back" control button
#define BACKB_PIN 5

// OUTPUT, active low - SPI SD select
#define SD_SS_PIN 10

// OUTPUT, active low - baseAPI.h - SPI GPIO select
#define GPIO_SS_PIN 14

// OUTPUT, active low - ledGrid.h - SPI Led Grid select
#define LG_SS_PIN 15

// OUTPUT, active high - pinball display 1 select
#define D_LD1_PIN 16

// OUTPUT, active high - pinball display 2 select
#define D_LD2_PIN 17

// OUTPUT, active high - pinball displays reset
#define D_RES_PIN 18

// OUTPUT - LCD display data bits..
#define LCD_D4_PIN 23

#define LCD_D5_PIN 22
#define LCD_D6_PIN 21
#define LCD_D7_PIN 20

// OUTPUT - LCD display R/W
#define LCD_RW_PIN 6

// OUTPUT - LCD display reset
#define LCD_RS_PIN 8

// OUTPUT - LCD display enable
#define LCD_EN_PIN 9

// SPI clock [Hz]
#define SPI_CLK 1000000

#define DEBOUNCE_T 200 

#endif