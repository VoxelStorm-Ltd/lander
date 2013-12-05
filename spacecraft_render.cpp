#include "spacecraft.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "vmath.h"

void spacecraft::render_diagram(double scale, bool labels) {
  /// Render in the orthographic diagram view
  glPushMatrix();
  // move into position
  glTranslated(position.x, position.y, position.z);

  // target vector
  glColor4dv(Vector4d(1.0, 0.6, 0.2, 1.0));
  glBegin(GL_LINES);
  glVertex3d(0.0, 0.0, 0.0);
  glVertex3dv(target * 1000000);
  glEnd();

  double thisradius = get_radius();
  if(thisradius * scale < 4.0) {
    thisradius = 4.0 / scale;
  }
  // draw an asteroids-style arrow at the radius
  glColor4dv(Vector4d(1.0, 1.0, 1.0, 1.0));
  glBegin(GL_LINE_LOOP);
  glVertex3d(0.0, -thisradius, 0.0);
  glVertex3d(-thisradius / sqrt(2.0), thisradius / sqrt(2.0), 0.0);
  glVertex3d(0.0, thisradius / 2.0, 0.0);
  glVertex3d( thisradius / sqrt(2.0), thisradius / sqrt(2.0), 0.0);
  glEnd();

  glPopMatrix();                            // restore position & rotation
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
  glPushMatrix();
  // render the cockpit panel
  Vector3d panelsize(2.0, 1.0, 0.5);

  glTranslated(-panelsize.x / 2.0, -panelsize.y, -1.1);
  glRotated(-60.0, 1.0, 0.0, 0.0);

  glColor4dv(Vector4d(0.5, 0.5, 0.5, 1.0));
  glBegin(GL_QUADS);
  // front
  glVertex3d(0.0,         0.0,         0.0);
  glVertex3d(panelsize.x, 0.0,         0.0);
  glVertex3d(panelsize.x, panelsize.y, 0.0);
  glVertex3d(0.0,         panelsize.y, 0.0);
  glColor4dv(Vector4d(0.4, 0.4, 0.4, 1.0));
  // top
  glVertex3d(0.0,         panelsize.y, -panelsize.z);
  glVertex3d(0.0,         panelsize.y, 0.0);
  glVertex3d(panelsize.x, panelsize.y, 0.0);
  glVertex3d(panelsize.x, panelsize.y, -panelsize.z);
  // bottom
  glVertex3d(0.0,         0.0,         -panelsize.z);
  glVertex3d(panelsize.x, 0.0,         -panelsize.z);
  glVertex3d(panelsize.x, 0.0,         0.0);
  glVertex3d(0.0,         0.0,         0.0);
  // right
  glVertex3d(panelsize.x, 0.0,         -panelsize.z);
  glVertex3d(panelsize.x, panelsize.y, -panelsize.z);
  glVertex3d(panelsize.x, panelsize.y, 0.0);
  glVertex3d(panelsize.x, 0.0,         0.0);
  // left
  glVertex3d(0.0,         0.0,         -panelsize.z);
  glVertex3d(0.0,         0.0,         0.0);
  glVertex3d(0.0,         panelsize.y, 0.0);
  glVertex3d(0.0,         panelsize.y, -panelsize.z);
  glEnd();

  // cycle through the instruments and render them
  for(auto const &it : instruments) {
    it->render();
  }
  glPopMatrix();
}
