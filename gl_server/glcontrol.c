/*
Copyright (c) 2013, Shodruky Rhyammer
All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

  Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.

  Redistributions in binary form must reproduce the above copyright notice, this
  list of conditions and the following disclaimer in the documentation and/or
  other materials provided with the distribution.

  Neither the name of the copyright holders nor the names of its
  contributors may be used to endorse or promote products derived from
  this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/


#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <unistd.h>

#include "glcontrol.h"

struct egl_state_t {
  Window x_window_;
  Pixmap x_pixmap_;
  EGLDisplay display_;
  EGLSurface surface_;
  EGLContext context_;
};
struct egl_state_t egl_create_state_window_shared(EGLContext share_context,
												  int x, int y, int w, int h) {
  static Display * g_x_display;
  struct egl_state_t retval;
  retval.x_window_ = 0;
  retval.x_pixmap_ = 0;
  retval.display_ = EGL_NO_DISPLAY;
  retval.surface_ = EGL_NO_SURFACE;
  retval.context_ = EGL_NO_CONTEXT;

  if (g_x_display == NULL) {
	XInitThreads();
	fprintf(stdout, "Using display: %s\n", XDisplayName(NULL));
	g_x_display = XOpenDisplay(NULL);
	if (g_x_display == NULL)
	{
	  fprintf(stderr, "XOpenDisplay() failed.\n");
	  goto error;
	}
  }
  Window x_root_window = DefaultRootWindow(g_x_display);
  XSetWindowAttributes x_swa;
  memset(&x_swa, 0, sizeof(x_swa));
  x_swa.background_pixmap = None;
  retval.x_window_ = XCreateWindow(g_x_display, x_root_window, x, y, w, h, 0,
								   CopyFromParent, InputOutput, CopyFromParent,
								   CWBackPixmap, &x_swa);
  if (retval.x_window_ == 0) {
	fprintf(stderr, "XCreateWindow() failed.\n");
	goto error;
  }
  XMapWindow(g_x_display, retval.x_window_);
  retval.display_ = eglGetDisplay(g_x_display);
  if (retval.display_ == EGL_NO_DISPLAY) {
	fprintf(stderr, "eglGetDisplay() failed.\n");
	goto error;
  }
  if (!eglInitialize(retval.display_, NULL, NULL)) {
	fprintf(stderr, "eglInitialize() failed with error: %x\n", eglGetError());
	goto error;
  }
  static const EGLint config_attribs[] = {
	EGL_BUFFER_SIZE, 24,
	EGL_BLUE_SIZE, 8,
	EGL_GREEN_SIZE, 8,
	EGL_RED_SIZE, 8,
	EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
	EGL_SURFACE_TYPE, EGL_WINDOW_BIT | EGL_PBUFFER_BIT,
	EGL_NONE
  };
  EGLint num_configs;
  if (!eglChooseConfig(retval.display_, config_attribs, NULL, 0,
					   &num_configs)) {
	fprintf(stderr, "eglChooseConfig() failed with error: %x\n", eglGetError());
	goto error;
  }
  EGLConfig egl_config;
  if (!eglChooseConfig(retval.display_, config_attribs, &egl_config, 1,
	  &num_configs)) {
	fprintf(stderr, "eglChooseConfig() failed with error: %x\n", eglGetError());
	goto error;
  }
  retval.surface_ = eglCreateWindowSurface(retval.display_, egl_config,
										   retval.x_window_, NULL);
  if (retval.surface_ == EGL_NO_SURFACE) {
	fprintf(stderr, "eglCreateWindowSurface() failed with error: %x\n",
			eglGetError());
	goto error;
  }
  static const EGLint context_attributes[] = {
	EGL_CONTEXT_CLIENT_VERSION, 2,
	EGL_NONE
  };
  retval.context_ = eglCreateContext(retval.display_, egl_config,
									 share_context, context_attributes);
  if (retval.context_ == EGL_NO_CONTEXT) {
	fprintf(stderr, "eglCreateContext() failed with error: %x\n",
			eglGetError());
	goto error;
  }
  error:
  return retval;
}



void init_egl(graphics_context_t *gc)
{
  EGLBoolean r;
  struct egl_state_t state = egl_create_state_window_shared(0,10,10,100,100);
  gc->display = state.display_;
  gc->context = state.context_;
  gc->screen_height = 100;
  gc->screen_width = 100;
  gc->surface = state.surface_;

  r = eglMakeCurrent(gc->display, gc->surface, gc->surface, gc->context);
  assert(EGL_FALSE != r);
  check_gl_err();
}


void release_egl(graphics_context_t *gc)
{
  eglMakeCurrent(gc->display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
  eglDestroySurface(gc->display, gc->surface);
  eglDestroyContext(gc->display, gc->context);
  eglTerminate(gc->display);
  eglReleaseThread();
}


