#include "renderer/core.hpp"
#include <OpenGL/OpenGL.h>
#include <iostream>

void glClearError() {
    while (glGetError() != GL_NO_ERROR)
        ;
}

bool glLogCall(const char* function, const char* file, int line) {
    bool noError = true;
    while (GLenum error = glGetError()) {
        std::cout << "[OpenGL Error] (" << error << ")" << function << " "
                  << file << ":" << line << "\n";
        noError = false;
    }
    return noError;
}
