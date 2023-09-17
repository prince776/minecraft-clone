#pragma once

#if defined(__APPLE_CC__)
#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl3.h>
#else
#include <GL/gl3.h> /* assert OpenGL 3.2 core profile available. */
#endif

#if defined(__i386__) || defined(__x86_64__)
#define BREAKPOINT __asm__("int $3\n" : :) /* NOLINT */
#elif defined(__aarch64__)
#define BREAKPOINT __asm__(".inst 0xd4200000")
#endif

#include <cstddef>

#define ASSERT(x)                                                              \
    if (!(x))                                                                  \
    BREAKPOINT

#define GLCALL(x)                                                              \
    glClearError();                                                            \
    x;                                                                         \
    ASSERT(glLogCall(#x, __FILE__, __LINE__))

void glClearError();

bool glLogCall(const char* function, const char* file, int line);
