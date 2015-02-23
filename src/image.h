#ifndef _IMAGE_H_
#define _IMAGE_H_

#include "rsiv_type.h"
#include "image_type.h"

static void fit_image(rsiv_t *main, Screen *screen);
static void render(rsiv_t *main);
void load_image(rsiv_t *main, const char *image_path);
void resize_image(rsiv_t *main);

#endif /* _IMAGE_H_ */
