#ifndef _RSIV_H_
#define _RSIV_H_

#include "image_type.h"
#include <stdint.h>
#include <X11/Xlib.h>
#include <Imlib2.h>

/* The structure that holds the image viewers parameters regarding the X server */

struct rsiv {
  uint64_t black_pixel;
  uint64_t white_pixel;
  
  image_t rsiv_image;
  Display *rsiv_display;
  Visual *rsiv_visual;
  Window rsiv_window;

  uint16_t rsiv_width;
  uint16_t rsiv_height;
  uint8_t rsiv_screen;
};

typedef struct rsiv rsiv_t;

#endif /* _RSIV_H_ */
