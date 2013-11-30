#include "spacecraft.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "vmath.h"

void spacecraft::render_diagram(double scale, bool labels) {
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
  if(thisradius * scale < 2.0) {
    thisradius = 2.0 / scale;
  }
  // draw an asteroids-style arrow at the radius
  glColor4dv(Vector4d(1.0, 1.0, 1.0, 1.0));
  glBegin(GL_LINE_LOOP);
  glVertex3d(0.0, radius, 0.0);
  glVertex3d(-radius / sqrt(2.0), -radius / sqrt(2.0), 0.0);
  glVertex3d(0.0, 0.0, 0.0);
  glVertex3d( radius / sqrt(2.0), -radius / sqrt(2.0), 0.0);
  glEnd();

  // restore rotation
  glPopMatrix();
}
void spacecraft::render_visible() {
  /// Render in the visible spectrum
  std::cout << "DEBUG: " << __PRETTY_FUNCTION__ << " not yet implemented" << std::endl;
}
void spacecraft::render_radio() {
  /// Render in the radio spectrum, i.e. radar reflection
  std::cout << "DEBUG: " << __PRETTY_FUNCTION__ << " not yet implemented" << std::endl;
}
void spacecraft::render_infrared() {
  /// Render in the infrared spectrum
  std::cout << "DEBUG: " << __PRETTY_FUNCTION__ << " not yet implemented" << std::endl;
}
void spacecraft::render_ultraviolet() {
  /// Render in the ultraviolet spectrum
  std::cout << "DEBUG: " << __PRETTY_FUNCTION__ << " not yet implemented" << std::endl;
}

void spacecraft::render_cockpit() {
  /// Render the interior of the cockpit with instruments etc (visible spectrum)
  for(auto &it : instruments) {
    it->render();
  }
}
