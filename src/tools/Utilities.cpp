#include "Utilities.h"
#include <OpenGl/glu.h>
#include <iostream>

GLint reportGLError() {
  GLint glErr = glGetError();
  
  if (0 != glErr) {
    std::cout << "OPENGL ERROR! code: " << glErr << '\n';
    const GLubyte *errString = gluErrorString(glErr);
    std::cout << errString << '\n';
  }
  return glErr;
}
