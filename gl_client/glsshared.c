#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>
#include <string.h>
#include "glclient.h"
#include <fcntl.h>
#include <math.h>
#include "EGL/egl.h"

void gls_initshared()
{
    static server_context_t sc;
    static int init = 0;
    if( init )
        return;
    int opt;
    char my_ip[GLS_STRING_SIZE_PLUS];
    char his_ip[GLS_STRING_SIZE_PLUS];
    uint16_t my_port = 12346;
    uint16_t his_port = 12345;
    strncpy(my_ip, "127.0.0.1", GLS_STRING_SIZE);
    strncpy(his_ip, "127.0.0.1", GLS_STRING_SIZE);
    server_init(&sc);
    set_bind_address_port(&sc, my_ip, my_port);
    set_address_port(&sc, his_ip, his_port);

    server_start(&sc);
    gls_init(&sc);
    gls_cmd_get_context();
    init = 1;

    return 0;
}

EGLBoolean eglBindAPI(EGLenum api)
{
    return 1;
}

EGLint eglGetError( void )
{
    return EGL_SUCCESS;
}

EGLDisplay eglGetDisplay( NativeDisplayType display )
{
    return 1;
}

EGLBoolean eglInitialize( EGLDisplay dpy, EGLint *major, EGLint *minor )
{
    gls_initshared();
    return EGL_TRUE;
}

EGLBoolean eglTerminate( EGLDisplay dpy )
{
    return EGL_TRUE;
}
const char *eglQueryString( EGLDisplay dpy, EGLint name )
{
    return "";
}


EGLAPI __eglMustCastToProperFunctionPointerType EGLAPIENTRY eglGetProcAddress( const char *procname )
{
    printf("AAAAAAAAAAAAaaa\n");
    return dlsym(dlopen(NULL, procname));
    //return NULL;
}

EGLBoolean eglGetConfigs( EGLDisplay dpy, EGLConfig *configs, EGLint config_size, EGLint *num_config )
{
    *num_config = 1;
    return EGL_TRUE;
}

EGLBoolean eglChooseConfig( EGLDisplay dpy, const EGLint *attrib_list, EGLConfig *configs, EGLint config_size, EGLint *num_config )
{
    *num_config = 1;
    return EGL_TRUE;
}

EGLBoolean eglGetConfigAttrib( EGLDisplay dpy, EGLConfig config, EGLint attribute, EGLint *value )
{
    return EGL_FALSE;
}

EGLSurface eglCreateWindowSurface( EGLDisplay dpy, EGLConfig config, NativeWindowType window, const EGLint *attrib_list )
{
    return 1;
}

EGLSurface eglCreatePixmapSurface( EGLDisplay dpy, EGLConfig config, NativePixmapType pixmap, const EGLint *attrib_list )
{
    return 1;
}

EGLSurface eglCreatePbufferSurface( EGLDisplay dpy, EGLConfig config, const EGLint *attrib_list )
{
    return 1;
}

EGLBoolean eglDestroySurface( EGLDisplay dpy, EGLSurface surface )
{
    return EGL_TRUE;
}
EGLBoolean eglQuerySurface( EGLDisplay dpy, EGLSurface surface, EGLint attribute, EGLint *value )
{
    if( attribute == EGL_WIDTH )
        *value = 100;
    else if( attribute == EGL_HEIGHT )
        *value = 100;
    else return EGL_FALSE;
    return EGL_TRUE;
}
/* EGL 1.1 render-to-texture APIs */
EGLBoolean eglSurfaceAttrib( EGLDisplay dpy, EGLSurface surface, EGLint attribute, EGLint value )
{

}

EGLBoolean eglBindTexImage( EGLDisplay dpy, EGLSurface surface, EGLint buffer )
{

}
EGLBoolean eglReleaseTexImage( EGLDisplay dpy, EGLSurface surface, EGLint buffer )
{

}

/* EGL 1.1 swap control API */
EGLBoolean eglSwapInterval( EGLDisplay dpy, EGLint interval )
{

}

EGLContext eglCreateContext( EGLDisplay dpy, EGLConfig config, EGLContext share_list, const EGLint *attrib_list )
{
    return 1;
}

EGLBoolean eglDestroyContext( EGLDisplay dpy, EGLContext ctx )
{
    return EGL_TRUE;
}

EGLBoolean eglMakeCurrent( EGLDisplay dpy, EGLSurface draw, EGLSurface read, EGLContext ctx )
{
    return EGL_TRUE;
}

EGLContext eglGetCurrentContext( void )
{
    return 1;
}
EGLSurface eglGetCurrentSurface( EGLint readdraw )
{
    return 1;
}
EGLDisplay eglGetCurrentDisplay( void )
{
    return 1;
}

EGLBoolean eglQueryContext( EGLDisplay dpy, EGLContext ctx, EGLint attribute, EGLint *value )
{
    return EGL_FALSE;
}

EGLBoolean eglWaitGL( void )
{

    return EGL_FALSE;
}

EGLBoolean eglWaitNative( EGLint engine )
{
    return EGL_FALSE;
}

EGLBoolean eglSwapBuffers( EGLDisplay dpy, EGLSurface draw )
{
    static int frame;
    return gls_cmd_flip(frame++);
}

EGLBoolean eglCopyBuffers( EGLDisplay dpy, EGLSurface surface, NativePixmapType target )
{
    return EGL_TRUE;
}
