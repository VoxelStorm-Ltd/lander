#include "astronaut.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "vmath.h"

void astronaut::render_diagram(double scale, bool labels) {
  /// Render in the orthographic diagram view
  // only render in EVA mode
  if(state == statetype::IN_VESSEL) {
     return;
  }

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
  // draw a cross at the radius
  glColor4dv(Vector4d(1.0, 1.0, 1.0, 1.0));
  glBegin(GL_LINES);
  glVertex3d(-thisradius, -thisradius, 0.0);
  glVertex3d( thisradius,  thisradius, 0.0);
  glEnd();
  glBegin(GL_LINES);
  glVertex3d(-thisradius,  thisradius, 0.0);
  glVertex3d( thisradius, -thisradius, 0.0);
  glEnd();

  glPopMatrix();                            // restore position & rotation
}

void astronaut::render_visible() {
  /// Render in the visible spectrum
  std::cout << "DEBUG: " << __PRETTY_FUNCTION__ << " not yet implemented" << std::endl;
}

void astronaut::render_radio() {
  /// Render in the radio spectrum, i.e. radar reflection
  std::cout << "DEBUG: " << __PRETTY_FUNCTION__ << " not yet implemented" << std::endl;
}

void astronaut::render_infrared() {
  /// Render in the infrared spectrum
  std::cout << "DEBUG: " << __PRETTY_FUNCTION__ << " not yet implemented" << std::endl;
}

void astronaut::render_ultraviolet() {
  /// Render in the ultraviolet spectrum
  std::cout << "DEBUG: " << __PRETTY_FUNCTION__ << " not yet implemented" << std::endl;
}
