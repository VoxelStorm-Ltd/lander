#include "body.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "vmath.h"

void body::render_diagram(double scale, bool labels) {
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
  if(thisradius * scale < 1) {
    thisradius = 1 / scale;
  }

  // circle outline
  glColor4dv(Vector4d(1.0, 1.0, 1.0, 1.0));
  double const circlestep = M_PI / 4.0;
  glBegin(GL_LINE_LOOP);
  for(double angle = 0.0; angle <= M_PI * 2.0; angle += circlestep) {
    glVertex3d(sin(angle) * thisradius, cos(angle) * thisradius, 0.0);
  }
  glEnd();

  glPopMatrix();                            // restore position & rotation
}

void body::render_visible() {
  /// Render in the visible spectrum
  std::cout << "DEBUG: " << __PRETTY_FUNCTION__ << " not yet implemented" << std::endl;
}

void body::render_radio() {
  /// Render in the radio spectrum, i.e. radar reflection
  std::cout << "DEBUG: " << __PRETTY_FUNCTION__ << " not yet implemented" << std::endl;
}

void body::render_infrared() {
  /// Render in the infrared spectrum
  std::cout << "DEBUG: " << __PRETTY_FUNCTION__ << " not yet implemented" << std::endl;
}

void body::render_ultraviolet() {
  /// Render in the ultraviolet spectrum
  std::cout << "DEBUG: " << __PRETTY_FUNCTION__ << " not yet implemented" << std::endl;
}
