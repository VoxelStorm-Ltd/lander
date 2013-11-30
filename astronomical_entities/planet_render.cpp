#include "planet.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "vmath.h"

void planet::render_diagram(double scale, bool labels) {
  /// Render in the orthographic diagram view
  glPushMatrix();
  // move into position
  glTranslated(position.x, position.y, position.z);
  // undo rotation - billboard effect
  Matrix4d modelview;
  glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
  for(unsigned int i = 0; i != 3; ++i) {
    for(unsigned int j = 0; j != 3; ++j) {
      if(i == j) {
        modelview[i * 4 + j] = 1.0;
      } else {
        modelview[i * 4 + j] = 0.0;
      }
    }
  }
  // set the modelview matrix with no rotations and scaling
  glLoadMatrixd(modelview);

  double thisradius = get_radius();
  if(thisradius * scale < 1.0) {
    thisradius = 1.0 / scale;
  }

  // draw a filled circle at the radius
  glColor4dv(Vector4d(0.25, 0.35, 0.25, 1.0));
  glBegin(GL_TRIANGLE_FAN);
  glVertex3d(0.0, 0.0, 0.0);
  double const circlestep = M_PI / 32.0;
  for(double angle = 0.0; angle < (M_PI * 2.0) + circlestep; angle += circlestep) {
    glVertex3d(sin(angle) * thisradius, cos(angle) * thisradius, 0.0);
  }
  glEnd();
  // circle outline
  glColor4dv(Vector4d(1.0, 1.0, 1.0, 1.0));
  glBegin(GL_LINE_LOOP);
  for(double angle = 0.0; angle <= M_PI * 2.0; angle += circlestep) {
    glVertex3d(sin(angle) * thisradius, cos(angle) * thisradius, 0.0);
  }
  glEnd();

  // atmosphere
  glColor4dv(Vector4d(0.2, 0.5, 0.5, 1.0));
  glBegin(GL_LINES);
  for(double angle = 0.0; angle <= M_PI * 2.0; angle += circlestep) {
    glVertex3d(sin(angle) * (thisradius + 100000), cos(angle) * (thisradius + 100000), 0.0);
  }
  glEnd();

  // restore rotation
  glPopMatrix();
}
void planet::render_visible() {
  /// Render in the visible spectrum
  std::cout << "DEBUG: " << __PRETTY_FUNCTION__ << " not yet implemented" << std::endl;
}
void planet::render_radio() {
  /// Render in the radio spectrum, i.e. radar reflection
  std::cout << "DEBUG: " << __PRETTY_FUNCTION__ << " not yet implemented" << std::endl;
}
void planet::render_infrared() {
  /// Render in the infrared spectrum
  std::cout << "DEBUG: " << __PRETTY_FUNCTION__ << " not yet implemented" << std::endl;
}
void planet::render_ultraviolet() {
  /// Render in the ultraviolet spectrum
  std::cout << "DEBUG: " << __PRETTY_FUNCTION__ << " not yet implemented" << std::endl;
}
