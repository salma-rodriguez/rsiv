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

#include "window.h"
#include "image.h"
#include <assert.h>

static void init_rsiv(rsiv_t *main) {
  /* Initialize everything to undefined and pointers to NULL */
  main->black_pixel = UNDEFINED;
  main->white_pixel = UNDEFINED;
  main->rsiv_display = NULL;
  main->rsiv_visual = NULL;
  main->rsiv_width = UNDEFINED;
  main->rsiv_height = UNDEFINED;
  main->rsiv_screen = UNDEFINED;
}

static void x_connect(rsiv_t *main) {
  /* Attempt to connect the display to the X server */
  main->rsiv_display = XOpenDisplay(NULL);
  if (main->rsiv_display == NULL) {
    fprintf(stderr, "Could not connect to the X server.\nExiting...\n");
    exit(1);
  }

  /* Set our default screen and grab the pixel values */
  main->rsiv_screen = DefaultScreen(main->rsiv_display);
  main->black_pixel = BlackPixel(main->rsiv_display, main->rsiv_screen);
  main->white_pixel = WhitePixel(main->rsiv_display, main->rsiv_screen);
}

static void create_window(rsiv_t *main) {
  /* Set our initial window at 500x500 */
  main->rsiv_width = 500;
  main->rsiv_height = 500;
  main->rsiv_window = XCreateSimpleWindow(main->rsiv_display,
					  DefaultRootWindow(main->rsiv_display),
					  0, /* x coordinate */
					  0, /* y coordinate */
					  main->rsiv_width,
					  main->rsiv_height,
					  0, /* border width */
					  main->black_pixel,
					  main->white_pixel);
  /* Listeners */
  XSelectInput(main->rsiv_display, main->rsiv_window,
	       ExposureMask |

	       /* Adding the StructureNotifyMask for resizability */
	       StructureNotifyMask);
  
  XStoreName(main->rsiv_display, main->rsiv_window, "rsiv");
  XMapWindow(main->rsiv_display, main->rsiv_window);
}

static void set_image_as_background(rsiv_t *main, const char *image_path) {
  main->rsiv_visual = DefaultVisual(main->rsiv_display, main->rsiv_screen);
  if (main->rsiv_visual == NULL) {
    fprintf(stderr, "Unable to recognise visual\nExiting...\n");
    exit(1);
  }

  XSync(main->rsiv_display, False);

  load_image(main, image_path);
  
  XClearWindow(main->rsiv_display, main->rsiv_window);
}

static void event_loop(rsiv_t *main, const char *image_path) {
  /* Handle the termination of the program correctly */
  Atom WM_DELETE_WINDOW = XInternAtom(main->rsiv_display, "WM_DELETE_WINDOW", False);
  XSetWMProtocols(main->rsiv_display, main->rsiv_window, &WM_DELETE_WINDOW, 1);

  /* Set the inital image */
  set_image_as_background(main, image_path);

  /* The actual event loop */
  while (1) {
    XEvent e;
    XNextEvent(main->rsiv_display, &e);
    if (e.type == ConfigureNotify) {
      /* Responding to the ConfigureNotify, the type of event sent by the X server
	 if the window is resized. There can be multiple occasions when this is
	 sent, so we have to handle the situation when the resize happens
	 additionally in the following if statement. */
      
      XConfigureEvent rsiv_xce = e.xconfigure;

      if (rsiv_xce.width != main->rsiv_width ||
	  rsiv_xce.height != main->rsiv_height) {
	main->rsiv_width = rsiv_xce.width;
	main->rsiv_height = rsiv_xce.height;
	resize_image(main);
      }
    } else if (e.type == ClientMessage) {
      /* Handle the press of an X button */
      break;
    }
  }

  /* Cleanly exit the loop */
  imlib_free_image();
  XCloseDisplay(main->rsiv_display);
}

void init_background(rsiv_t *main) {
  GC rsiv_gc = XCreateGC(main->rsiv_display, main->rsiv_window, 0, NULL);
  XSetBackground(main->rsiv_display, rsiv_gc, main->white_pixel);
}

void rsiv_open() {
  /*TODO: Draw white screen and enable vim commands */
  assert(0);
}

void rsiv_open_image(const char *image_path) {
  rsiv_t main;
  init_rsiv(&main);
  x_connect(&main);
  create_window(&main);
  event_loop(&main, image_path);
}

void rsiv_help() {
  fprintf(stdout, "RSIV -- Usage guide:\n");
  fprintf(stdout, "    ./rsiv -- opens white display screen\n");
  fprintf(stdout, "    ./rsiv <image> -- opens the image");
}
