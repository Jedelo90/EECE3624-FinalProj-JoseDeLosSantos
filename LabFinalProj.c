/*****************************************************************************
 * 	      File: LabFinalProj.c
 *    Lab Name: Zumo Sumo AVR Controller
 *      Author: Prof. John Hutton
 *     Created: 12/30/17
 * Modified by: Jose De Los Santos
 *    Modified: 3/28/19, 1/17/21, 11/13/2023
 *   Processor: ATmega128A (on the ReadyAVR board)
 *
 * This is a placeholder for your final project program.  This will
 * leverage off Lab09, Lab10 as well as other work we have done in this
 * class.
 *
 * This program depends on the UARTLibrary used in previous labs, one for 
 * Character debug to the PUTTY terminal and one to the BT Master.
 *
 * NB: The ReadyAVR-64 board uses UART1 as the default for programming,
 * freeing uart1 for external RS232 traffic (e.g. like adding the Bluetooth
 * transceivers in Lab09).
 *   UART1 for communication to a Putty Terminal
 *   UART0 for communication across the paired BT channel.
 *
 * <Student Notes on program here>
 *****************************************************************************/

// define CPU speed (7.3728 MHz for the ATmega128A)
#define F_CPU   7372800UL

#define END_STRING  '\0'
#define CR          0x0D
#define LF          0x0A
#define ESC         0x1B

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>  // for sprintf functionality
#include "UARTLibrary.h"
#include "UART0Library.h"

#define JOY_RIGHT	PB0
#define JOY_DOWN	PB1
#define JOY_LEFT	PB2
#define JOY_UP		PB3
#define JOY_CENTER	PB4

#define BAUDRATE   9600 // standard values from 1200 to 115200
#define DATABITS      8 // 5-8
#define STOPBITS      1 // 1,2
#define PARITY        0 // 0=none, 1=odd, 2=even

char text[40];
unsigned char armUp = 0;

void send_string(const char *cp) {
	while(*cp != END_STRING) {
		uart_tx(*cp++);
	}
}



char readJoystick(void) {
	
	unsigned char pins = PINB;
	if (!(pins & (1 << JOY_UP))) {
		return 'F';
	}
	if (!(pins & (1 << JOY_DOWN))) {
		return 'B';
	}
	if (!(pins & (1 << JOY_LEFT))) {
		return 'L';
	}
	if (!(pins & (1 << JOY_RIGHT))) {
		return 'R';
	}
	return 'S';
}

int main(void) {
	
	uart_init(BAUDRATE, DATABITS, STOPBITS, PARITY);	// UART1: PuTTY
	uart0_init(BAUDRATE, DATABITS, STOPBITS, PARITY);	// UART0: Bluetooth
	
	// Joystick pins PB4:PB0 assigned as inputs with pull-ups
	DDRB	&= ~0x1F;
	PORTB	|=  0X1F;
	
	// Arm buttons PA0 (LIFT) AND PA1(LOWER) as inputs with pull-ups
	DDRA	&= ~((1 <<PA0) | (1 <<PA1));
	PORTA	|=	(1 << PA0) | (1 <<PA1);
	
	send_string("\r\nTiny Z controller ready.\r\n");
	
	while(1) {
		
		if (!(PINA & (1 << PA0))) armUp = 1;	// LIFT pressed
		if (!(PINA & (1 << PA1))) armUp = 0;	// LOWER pressed
		
		char cmd = readJoystick();
		if (armUp) cmd = cmd + ('a'-'A');		// converts capital A to lowercase a
		uart0_tx(cmd);	// send cmd to Tiny Z via BT
		uart_tx(cmd);	// echo cmd in PuTTY
		_delay_ms(100);
		
	}
	
	return 0;

}