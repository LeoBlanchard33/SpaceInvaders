#ifndef SERIAL_H_
#define SERIAL_H_

#include <stdint.h>

void serial_init(const uint32_t baudrate);
void serial_putchar(const volatile char c);
void serial_puts(const volatile char *s);

/* -1 if none */
signed char serial_get_last_char(void);

#endif /* SERIAL_H_ */
