#ifndef _WINDOW_H_
#define _WINDOW_H_

#define UNDEFINED -1

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <X11/Xlib.h>

#include "rsiv_type.h"

static void init_rsiv(rsiv_t *main);
static void x_connect(rsiv_t *main);
static void create_window(rsiv_t *main);
static void set_image_as_background(rsiv_t *main, const char *image_path);
static void event_loop(rsiv_t *main, const char *image_path);

void init_background(rsiv_t *main);

void rsiv_open();
void rsiv_open_image(const char *image_path);
void rsiv_help();

#endif /* _WINDOW_H_ */
