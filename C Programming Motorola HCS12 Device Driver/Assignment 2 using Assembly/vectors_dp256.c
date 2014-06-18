/* To use, either add this file to the project file list (preferred), or 
 * #include it in a single source file if you are just using 
 * File->CompileToOutput (this works but using the Project feature is better
 */

/* Vector Table for Dx256 S12 CPU
 * As is, all interrupts except reset jumps to 0xffff, which is most
 * likely not going to useful. To replace an entry, declare your function,
 * and then change the corresponding entry in the table. For example,
 * if you have a SCI handler (which must be defined with 
 * #pragma interrupt_handler ...) then in this file:
 * add
 *	extern void SCIHandler();
 * before th table.
 * In the SCI entry, change:
 *	DUMMY_ENTRY,
 * to
 *  SCIHandler, 
 */
#pragma nonpaged_function _start
extern void _start(void);	/* entry point in crt??.s */

#define DUMMY_ENTRY	(void (*)(void))0xFFFF

//#pragma abs_address:0xFF80 		//Used for Standalone Apps.
#pragma abs_address:0x3F8C 		//Used with the monitor

/* change the above address if your vector starts elsewhere
 */
void (*interrupt_vectors[])(void) = 
	{
	/* to cast a constant, say 0xb600, use
	   (void (*)())0xb600
	 */
	//DUMMY_ENTRY, /*Reserved $FF80*/
	//DUMMY_ENTRY, /*Reserved $FF82*/
	//DUMMY_ENTRY, /*Reserved $FF84*/
	//DUMMY_ENTRY, /*Reserved $FF86*/
	//DUMMY_ENTRY, /*Reserved $FF88*/
	//DUMMY_ENTRY, /*Reserved $FF8A*/
	DUMMY_ENTRY, /*PWM Emergency Shutdown*/
	DUMMY_ENTRY, /*Port P Interrupt*/
	DUMMY_ENTRY, /*MSCAN 4 Transmit*/
	DUMMY_ENTRY, /*MSCAN 4 Receive*/
	DUMMY_ENTRY, /*MSCAN 4 Error*/
	DUMMY_ENTRY, /*MSCAN 4 Wake-up*/
	DUMMY_ENTRY, /*MSCAN 3 Transmit*/
	DUMMY_ENTRY, /*MSCAN 3 Receive*/
	DUMMY_ENTRY, /*MSCAN 3 Error*/
	DUMMY_ENTRY, /*MSCAN 3 Wake-up*/
	DUMMY_ENTRY, /*MSCAN 2 Transmit*/
	DUMMY_ENTRY, /*MSCAN 2 Receive*/
	DUMMY_ENTRY, /*MSCAN 2 Error*/
	DUMMY_ENTRY, /*MSCAN 2 Wake-up*/
	DUMMY_ENTRY, /*MSCAN 1 Transmit*/
	DUMMY_ENTRY, /*MSCAN 1 Receive*/
	DUMMY_ENTRY, /*MSCAN 1 Error*/
	DUMMY_ENTRY, /*MSCAN 1 Wake-up*/
	DUMMY_ENTRY, /*MSCAN 0 Transmit*/
	DUMMY_ENTRY, /*MSCAN 0 Receive*/
	DUMMY_ENTRY, /*MSCAN 0 Error*/
	DUMMY_ENTRY, /*MSCAN 0 Wake-up*/
	DUMMY_ENTRY, /*Flash*/
	DUMMY_ENTRY, /*EEPROM*/
	DUMMY_ENTRY, /*SPI2*/
	DUMMY_ENTRY, /*SPI1*/
	DUMMY_ENTRY, /*IIC Bus*/
	DUMMY_ENTRY, /*DLC*/
	DUMMY_ENTRY, /*SCME*/
	DUMMY_ENTRY, /*CRG Lock*/
	DUMMY_ENTRY, /*Pulse Accumulator B Overflow*/
	DUMMY_ENTRY, /*Modulus Down Counter Underflow*/
	DUMMY_ENTRY, /*Port H Interrupt*/
	DUMMY_ENTRY, /*Port J Interrupt*/
	DUMMY_ENTRY, /*ATD1*/
	DUMMY_ENTRY, /*ATD0*/
	DUMMY_ENTRY, /*SCI1*/
	DUMMY_ENTRY, /*SCI0*/
	DUMMY_ENTRY, /*SPI0*/
	DUMMY_ENTRY, /*Pulse Accumulator A Input Edge*/
	DUMMY_ENTRY, /*Pulse Accumulator A Overflow*/
	DUMMY_ENTRY, /*Timer Overflow*/
	DUMMY_ENTRY, /*Timer Channel 7*/
	DUMMY_ENTRY, /*Timer Channel 6*/
	DUMMY_ENTRY, /*Timer Channel 5*/
	DUMMY_ENTRY, /*Timer Channel 4*/
	DUMMY_ENTRY, /*Timer Channel 3*/
	DUMMY_ENTRY, /*Timer Channel 2*/
	DUMMY_ENTRY, /*Timer Channel 1*/
	DUMMY_ENTRY, /*Timer Channel 0*/
	DUMMY_ENTRY, /*Real Time Interrupt*/
	DUMMY_ENTRY, /*IRQ*/
	DUMMY_ENTRY, /*XIRQ*/
	DUMMY_ENTRY, /*SWI*/
	DUMMY_ENTRY, /*Unimplement Intruction Trap*/
	DUMMY_ENTRY, /*COP failure reset*/
	DUMMY_ENTRY, /*Clock monitor fail reset*/
	_start, /*Reset*/
	};
#pragma end_abs_address
