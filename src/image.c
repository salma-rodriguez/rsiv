/*
Copyright (c) 2015, Domagoj Stolfa <domagoj.stolfa@gmail.com>
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer. 
2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "image.h"

static void fit_image(rsiv_t *main, Screen *screen) {
  /* Take the minimum of the scales to produce the smallest image */
  float scale_width = (float) screen->width / (float) main->rsiv_image.width;
  float scale_height = (float) screen->height / (float) main->rsiv_image.height;

  float scale = (scale_width < scale_height) ? scale_width : scale_height;

  main->rsiv_image.width *= scale;
  main->rsiv_image.height *= scale;
}

static void render(rsiv_t *main) {
  /* Create the Pixmap from the image contained in the rsiv struct */
  Pixmap rsiv_bg;
  rsiv_bg = XCreatePixmap(main->rsiv_display,
			  main->rsiv_window,
			  main->rsiv_image.width,
			  main->rsiv_image.height,
			  main->rsiv_image.depth);

  /* Set it to drawable */
  imlib_context_set_drawable(rsiv_bg);

  /* Render the actual image */
  imlib_render_image_on_drawable_at_size(0, /* x */
					 0, /* y */
					 main->rsiv_image.width,
					 main->rsiv_image.height);

  /* Set the background to the actual image */
  XSetWindowBackgroundPixmap(main->rsiv_display, main->rsiv_window, rsiv_bg);
  /* Update the actual window */
  XClearWindow(main->rsiv_display, main->rsiv_window);
  /* Leave the Pixmap for the X server to handle */
  XFreePixmap(main->rsiv_display, rsiv_bg);
}

void load_image(rsiv_t *main, const char *image_path) {
  /* Turn on dithering to produce better results and gradients for rendering at
     under 16 bits */
  imlib_context_set_dither(1);

  /* Initialize display and visual */
  imlib_context_set_display(main->rsiv_display);
  imlib_context_set_visual(main->rsiv_visual);
  
  /* Load the image from the path */
  main->rsiv_image.image = imlib_load_image(image_path);

  imlib_context_set_image(main->rsiv_image.image);

  /* Read the image parameters */
  main->rsiv_image.depth = DefaultDepth(main->rsiv_display, main->rsiv_screen);
  main->rsiv_image.width = imlib_image_get_width();
  main->rsiv_image.height = imlib_image_get_height();

  /* If the image is larger than the screen, crop it */
  Screen *screen = DefaultScreenOfDisplay(main->rsiv_display);
  if (main->rsiv_image.width > screen->width ||
      main->rsiv_image.height > screen->height) {
    fit_image(main, screen);
  }
  
  /* Set the window to fit the image size by default, resizing will be handled
     additionally in the function resize_image */
  XResizeWindow(main->rsiv_display,
		main->rsiv_window,
		main->rsiv_image.width,
		main->rsiv_image.height);

  /* Generate the Pixmap from the loaded image and render it on the background */
  render(main);
}

void resize_image(rsiv_t *main) {
  /* Use the existing image data when resizing */
  main->rsiv_image.image = imlib_create_image_using_data(main->rsiv_width,
						   main->rsiv_height,
						   imlib_image_get_data());
  /* Update the image parameters */
  main->rsiv_image.width = main->rsiv_width;
  main->rsiv_image.height = main->rsiv_height;
  render(main);
}
