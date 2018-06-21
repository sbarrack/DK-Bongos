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

//gcn64.c
#include "gcb.h"

void gcSend(const uint8_t* buff, uint8_t len, volatile uint8_t* modePort, volatile uint8_t* outPort, uint8_t bitMask) {
	*outPort |= bitMask;
	*modePort |= bitMask;

	register uint8_t bitCount;
	register uint8_t data;

	__asm__ volatile (
		".L%=_byte_loop:\n"
		"ldr %[data],%a[buff],1\n"	//(2) data -> buffer++
		"mov %[bitCount],8\n"		//(1) bitCount = 8
		"nop\nnop\n"				//(2)

		".L%=_bit_loop:\n"
		"nop\nnop\n"				//(2)
		"ldr %[outPort],%a[low]\n"	//(2) outPort -> low
		//1us - 4
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
		"nop\nnop\nnop\nnop\nnop\nnop\n"	//(116)
#ifdef __HARDWARE_MK66FX1M0__	//Teensy 3.6
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"	//(60)
#endif
		"lsl %[data],1\n"			//(1) data << 1
		"bcc .L%=_zero_bit\n"		//(1|1+p) if C == 0, goto
		"ldr %[outPort],%a[high]\n"	//(2) outPort -> high

		".L%=_zero_bit:\n"
		//2us - 5
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
		"nop\nnop\nnop\nnop\nnop\n"	//(235)
#ifdef __HARDWARE_MK66FX1M0__	//Teensy 3.6
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
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"	//(120)
#endif
		"ldr %[outPort],%a[high]\n"	//(2) outPort -> high
		//1us - 8
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
		"nop\nnop\n"	//(112)
#ifdef __HARDWARE_MK66FX1M0__	//Teensy 3.6
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"	//(60)
#endif
		"sub %[bitCount],1\n"		//(1) bitCount--
		"beq .L%=_load_next_byte\n"	//(1|1+p) if bitCount == 0, goto
		"b .L%=_bit_loop\n"			//(1+p) goto

		".L%=_load_next_byte:\n"
		"sub %[len],1\n"		//(1) len--
		"beq .L%=_loop_exit\n"	//(1|1+p) if len == 0, goto
		"b .L%=_byte_loop\n"	//(1+p) goto

		".L%=_loop_exit:\n"
		/*	I've seen several different opinions about what the proper stop bit is:
			1) Both console and controller send a 1 bit (Nico and James)
			2) Controller sends a 2us low - 2us high, console sends a 1us low - 2us high (n64squid)
			3) According to other N64 documentation both send a 1us low - 2us high except if
				the console sends the 0x02/0x03 command (accessing N64 controller pak)
			I'll just stick with a 1 bit here.
		*/
		"ldr %[outPort],%a[low]\n"	//(2) outPort -> low
		//1us - 2
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
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"	//(118)
#ifdef __HARDWARE_MK66FX1M0__	//Teensy 3.6
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"	//(60)
#endif
		"ldr %[outPort],%a[high]\n"	//(2) outPort -> high

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
		"mov %[bitCount],8\n"				//(1) bitCount = 8
		"mov %[receivedBytes],0\n"			//(1) receivedBytes = 0
		"mov %[initialTimeoutCount],0\n"	//(1) initialTimeoutCount = 0

		".L%=_wait_for_low_extra:\n"
		"mov %[timeoutCount],0\n"	//(1) timeoutCount = 0

		".L%=_wait_for_low_loop_extra1:\n"
		"ldr %[inputVal], %a[inPort]\n"			//(2) inputVal -> inPort
		"and %[inputVal], %[bitMask]\n"			//(1) to compare
		"beq .L%=_wait_for_measure\n"			//(1|1+p) if inputVal = 0, goto
		"sub %[timeoutCount],1\n"				//(1) timeoutCount--
		"bne .L%=_wait_for_low_loop_extra1\n"	//(1|1+p) if timeoutCount != 0, goto
		"mov %[timeoutCount],0\n"				//(1) timeoutCount = 0

		".L%=_wait_for_low_loop_extra2:\n"
		"ldr %[inputVal], %a[inPort]\n"			//(2) inputVal -> inPort
		"and %[inputVal], %[bitMask]\n"			//(1) to compare
		"beq .L%=_wait_for_measure\n"			//(1|1+p) if inputVal = 0, goto
		"sub %[timeoutCount],1\n"				//(1) timeoutCount--
		"bne .L%=_wait_for_low_loop_extra2\n"	//(1|1+p) if timeoutCount != 0, goto

		".L%=_wait_for_low:\n"
		"mov %[timeoutCount],128\n"	//(1) timeoutCount = 128

		".L%=_wait_for_low_loop:\n"
		"ldr %[inputVal], %a[inPort]\n"	//(2) inputVal -> inPort
		"and %[inputVal], %[bitMask]\n"	//(1) to compare
		"beq .L%=_wait_for_measure\n"	//(1|1+p) if inputVal = 0, goto
		"sub %[timeoutCount],1\n"		//(1) timeoutCount--
		"bne .L%=_wait_for_low_loop\n"	//(1|1+p) if timeoutCount != 0, goto
		"sub %[initialTimeoutCount],1\n"//(1)
		"bne .L%=_wait_for_low_extra\n"	//(1|1+p) if initialTimeoutCount != 0, goto
		"b .L%=_exit\n"					//(1+p) goto

		".L%=_wait_for_measure:\n"
		/*	This brings me back to the proper stop bit talk. Normally, I'd be
			inclined to wait only 1us to measure the bit, HOWEVER, the controller
			could send a 2us low long stop bit instead of a 1 bit and cause
			trouble. The duty cycle for reading a bit is 2us long so I still have the
			opportunity to read the bit 1us later than normal and I'd have to wait
			anyways for the next bit.
		*/
		//2us - 9
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
		"nop\n"	//(231)
#ifdef __HARDWARE_MK66FX1M0__	//Teensy 3.6
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
		"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"	//(120)
#endif
		"mov %[initialTimeoutCount],1\n"	//(1) initialTimeoutCount = 1
		"lsl %[data],1\n"					//(1) data << 1
		"ldr %[inputVal],%a[inPort]\n"		//(2) inputVal -> inPort
		"and %[inputVal],%[bitMask]\n"		//(1) to compare
		"beq .L%=_check_bit_count\n"		//(1|1+p) if inputVal = 0, goto
		"orr %[data],1\n"					//(1) data | 1

		".L%=_check_bit_count:\n"
		"sub %[bitCount],1\n"			//(1) bitCount--
		"bne .L%=_wait_for_high\n"		//(1|1+p) if bitCount != 0, goto
		"ldr %[buff],%a[data]\n"		//(2) buff -> data
		"ldr %[data],%a[buff],1\n"		//(2) &buff++
		"add %[receivedBytes],1\n"		//(1) receivedBytes++
		"mov %[bitCount],8\n"			//(1) bitCount = 8
		"mov %[len],%[receivedBytes]\n"	//(1) to compare
		"beq .L%=_exit\n"				//(1|1+p) if len == receivedBytes, goto

		".L%=_wait_for_high:\n"
		"mov %[timeoutCount],128\n"	//(1) timeoutCount = timeout

		".L%=_wait_for_high_loop:\n"
		"ldr %[inputVal],%a[inPort]\n"	//(2) inputVal -> inPort
		"and %[inputVal],%[bitMask]\n"	//(1) to compare
		"bne .L%=_wait_for_low\n"		//(1|1+p) if inputVal != 0, goto
		"sub %[timeoutCount],1\n"		//(1) timeoutCount--
		"bne .L%=_wait_for_high_loop\n"	//(1|1+p) if timeoutCount != 0, goto
		
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
