#ifndef VT100_H_
#define VT100_H_

#include <stdint.h>

#define VT100_SCREEN_HEIGHT (24)
#define VT100_SCREEN_WIDTH  (80)

#define VT100_SCREEN_XMIN (1)
#define VT100_SCREEN_YMIN (1)
#define VT100_SCREEN_XMAX (VT100_SCREEN_WIDTH)
#define VT100_SCREEN_YMAX (VT100_SCREEN_HEIGHT)

#define VT100_ESC       (27)
#define VT100_FORM_FEED (12)

void vt100_move(uint8_t x, uint8_t y);
void vt100_clear_screen(void);

#endif /* VT100_H_ */
