#include "vt100.h"
#include "serial.h"

void vt100_move(uint8_t x, uint8_t y)
{
	serial_putchar(VT100_ESC);
	serial_putchar('[');
	serial_putchar('0' + (y / 100));
	serial_putchar('0' + ((y / 10) % 10));
	serial_putchar('0' + (y % 10));
	serial_putchar(';');
	serial_putchar('0' + (x / 100));
	serial_putchar('0' + ((x / 10) % 10));
	serial_putchar('0' + (x % 10));
	serial_putchar('H');
}

void vt100_clear_screen(void)
{
	serial_putchar(VT100_ESC);
	serial_putchar('[');
	serial_putchar('2');
	serial_putchar('J');
	serial_putchar(VT100_ESC);
	serial_putchar('[');
	serial_putchar('?');
	serial_putchar('2');
	serial_putchar('5');
	serial_putchar('l');
}

