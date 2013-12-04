#include "altimeter.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

void altimeter::render() {
  /// Draw the altimiter arrows and clock
  glPushMatrix();

  glTranslated(position.x,
               position.y,
               position.z);

  glColor4dv(Vector4d(1.0, 1.0, 1.0, 1.0));
  glBegin(GL_QUADS);
  glVertex3d(0.0,    0.0,    size.z);
  glVertex3d(size.x, 0.0,    size.z);
  glVertex3d(size.x, size.y, size.z);
  glVertex3d(0.0,    size.y, size.z);
  glEnd();

  // render arrow
  // TODO

  glPopMatrix();
}
