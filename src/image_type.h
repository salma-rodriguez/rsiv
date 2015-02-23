#ifndef _IMAGE_TYPE_H_
#define _IMAGE_TYPE_H_

#include <stdint.h>
#include <Imlib2.h>

/* The structure that holds the current displayed image parameters */

struct image {
  Imlib_Image image;

  uint16_t width;
  uint16_t height;
  uint16_t depth;
};

typedef struct image image_t;

#endif /* _IMAGE_TYPE_H_ */
