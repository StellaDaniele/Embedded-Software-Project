#ifndef __CAPTURE_H__
#define HPP_CAPTURE

#include <stdint.h>

/*
Captures the images of size wgXhg and
transfers it over USART0
*/
void captureImg(uint16_t wg, uint16_t hg);

#endif
