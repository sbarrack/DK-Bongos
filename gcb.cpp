/*	This is ProjectBongo; a program dedicated to the functionality of Donkey Kong Bongos and beyond.
	Copyright (C) 2018  Stephen Barrack

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.

	For any issues, please contact stephen.barrack12@yahoo.com.
	*/

//TODO: Base nops on F_CPU (clock speed in Hz) and send/get functions on ARDUINO (microcontroller type)

//gcn64.c
#include "gcb.h"

void gcSend(const uint8_t* buff, uint8_t len, volatile uint8_t* modePort, volatile uint8_t* outPort, uint8_t bitMask) {
	*outPort |= bitMask;
	*modePort |= bitMask;

	register uint8_t bitCount;
	register uint8_t data;

	__asm__ volatile (
		".L%=_byte_loop:\n"
		"ldr %[data],%a[buff],1\n"	//data -> buffer++
		"mov %[bitCount],8\n"		//bitCount = 8

		".L%=_bit_loop:\n"
		"ldr %[outPort],%a[low]\n"	//outPort -> low
		"lsl %[data],1\n"			//data << 1
		"bcc .L%=_zero_bit\n"		//if C == 0, goto

		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"

		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
		"nop\nnop\nnop\nnop\nnop\n"	//nop 135
		"ldr %[outPort],%a[high]\n"	//outPort -> high
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"

		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"

		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"

		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"	//nop 338
		"b .L%=_finish_bit\n"	//goto

		".L%=_zero_bit:\n"
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"

		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"

		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"

		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"

		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
		"nop\nnop\nnop\nnop\n"		//nop 484
		"ldr %[outPort],%a[high]\n"	//outPort -> high

		".L%=_finish_bit:\n"
		"sub %[bitCount],1\n"		//bitCount--
		"beq .L%=_load_next_byte\n"	//if bitCount==0, goto
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"

		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
		"nop\nnop\nnop\n"	//nop 113
		"b .L%=_bit_loop\n"	//goto

		".L%=_load_next_byte:\n"
		"sub %[len],1\n"		//len--
		"beq .L%=_loop_exit\n"	//if len==0, goto
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
		"nop\nnop\nnop\nnop\nnop\n"	//nop 45
		"b .L%=_byte_loop\n"		//goto

		".L%=_loop_exit:\n"
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
									//nop 90
		"ldr %[outPort],%a[low]\n"	//outPort -> low
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"

		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"	//nop 158
		"ldr %[outPort],%a[high]\n"					//outPort -> high

		://outs
		//read and write
		[buff] "+r" (buff),
		[outPort] "+r" (outPort),
		//out only, write only
		[bitCount] "=&r" (bitCount),
		[data] "=&r" (data)
		://ins
		[len] "r" (len),
		[high] "r" (*outPort | bitMask),
		[low] "r" (*outPort & ~bitMask)
	);
}

uint8_t gcGet(uint8_t* buff, uint8_t len, volatile uint8_t* modePort, volatile uint8_t* outPort, volatile uint8_t * inPort, uint8_t bitMask) {
	*modePort &= ~bitMask;
	*outPort |= bitMask;

	register uint8_t timeoutCount;
	register uint8_t bitCount;
	register uint8_t inputVal;
	register uint8_t data;
	register uint8_t receivedBytes;
	register uint8_t initialTimeoutCount;

	__asm__ volatile(
		"mov %[bitCount],8\n"				//bitCount = 8
		"mov %[receivedBytes],0\n"			//receivedBytes = 0
		"mov %[initialTimeoutCount],0\n"	//initialTimeoutCount = 0

		".L%=_wait_for_low_extra:\n"
		"mov %[timeoutCount],0\n"	//timeoutCount = 0

		".L%=_wait_for_low_loop_extra1:\n"
		"ldr %[inputVal], %a[inPort]\n"			//inputVal -> inPort
		"and %[inputVal], %[bitMask]\n"			//to compare
		"beq .L%=_wait_for_measure\n"			//if inputVal = 0, goto
		"sub %[timeoutCount],1\n"				//timeoutCount--
		"bne .L%=_wait_for_low_loop_extra1\n"	//if timeoutCount != 0, goto

		"mov %[timeoutCount],0\n"	//timeoutCount = 0

		".L%=_wait_for_low_loop_extra2:\n"
		"ldr %[inputVal], %a[inPort]\n"			//inputVal -> inPort
		"and %[inputVal], %[bitMask]\n"			//to compare
		"beq .L%=_wait_for_measure\n"			//if inputVal = 0, goto
		"sub %[timeoutCount],1\n"				//timeoutCount--
		"bne .L%=_wait_for_low_loop_extra2\n"	//if timeoutCount != 0, goto

		".L%=_wait_for_low:\n"
		"mov %[timeoutCount],128\n"	//timeoutCount = 128

		".L%=_wait_for_low_loop:\n"
		"ldr %[inputVal], %a[inPort]\n"	//inputVal -> inPort
		"and %[inputVal], %[bitMask]\n"	//to compare
		"beq .L%=_wait_for_measure\n"	//if inputVal = 0, goto
		"sub %[timeoutCount],1\n"		//timeoutCount--
		"bne .L%=_wait_for_low_loop\n"	//if timeoutCount != 0, goto

		"sub %[initialTimeoutCount],1\n"
		"bne .L%=_wait_for_low_extra\n"	//if initialTimeoutCount != 0, goto
		"b .L%=_exit\n"					//goto

		".L%=_wait_for_measure:\n"
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"

		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"

		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
		//nop 270 (in the average case, 237 worst, 304 best)
		"mov %[initialTimeoutCount],1\n"	//initialTimeoutCount = 1
		"lsl %[data],1\n"					//data << 1
		"ldr %[inputVal],%a[inPort]\n"		//inputVal -> inPort
		"and %[inputVal],%[bitMask]\n"		//to compare
		"beq .L%=_check_bit_count\n"		//if inputVal = 0, goto
		"orr %[data],1\n"					//data | 1

		".L%=_check_bit_count:\n"
		"sub %[bitCount],1\n"		//bitCount--
		"bne .L%=_wait_for_high\n"	//if bitCount != 0, goto

		"ldr %[buff],%a[data]\n"		//buff -> data
		"ldr %[data],%a[buff],1\n"		//&buff++
		"add %[receivedBytes],1\n"		//receivedBytes++
		"mov %[bitCount],8\n"			//bitCount = 8
		"mov %[len],%[receivedBytes]\n"	//to compare
		"beq .L%=_exit\n"				//if len == receivedBytes, goto

		".L%=_wait_for_high:\n"
		"mov %[timeoutCount],128\n"	//timeoutCount = timeout

		".L%=_wait_for_high_loop:\n"
		"ldr %[inputVal],%a[inPort]\n"	//inputVal -> inPort
		"and %[inputVal],%[bitMask]\n"	//to compare
		"bne .L%=_wait_for_low\n"		//if inputVal != 0, goto

		"sub %[timeoutCount],1\n"		//timeoutCount--
		"bne .L%=_wait_for_high_loop\n"	//if timeoutCount != 0, goto
		".L%=_exit:\n"

		://outs
		//read and write
		[buff] "+r" (buff),
		//out only, write only
		[receivedBytes] "=&r" (receivedBytes),
		[bitCount] "=&r" (bitCount),
		[timeoutCount] "=&r" (timeoutCount),
		[initialTimeoutCount] "=&r" (initialTimeoutCount),
		[inputVal] "=&r" (inputVal),
		[data] "=&r" (data)
		://ins
		[len] "r" (len),
		[inPort] "r" (inPort),
		[bitMask] "r" (bitMask)
	);
	return receivedBytes;
}

uint8_t gcSendGet(const uint8_t pin, uint8_t* command, const uint8_t commandLen, uint8_t* report, const uint8_t reportLen) {
	uint8_t bitMask = digitalPinToBitMask(pin);
	uint8_t port = digitalPinToPort(pin);

	volatile uint8_t* modePort = portModeRegister(port);
	volatile uint8_t* outPort = portOutputRegister(port);
	volatile uint8_t* inPort = portInputRegister(port);

	uint8_t oldSREG = SREG;
	cli();

	gcSend(command, commandLen, modePort, outPort, bitMask);
	uint8_t receivedBytes = gcGet(report, reportLen, modePort, outPort, inPort, bitMask);

	SREG = oldSREG;

	return receivedBytes;
}
