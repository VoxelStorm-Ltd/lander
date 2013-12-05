#include "instrumentpanel.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <spacecraft.h>
#include <instrument.h>

instrumentpanel::instrumentpanel() {
  /// Default constructor
}

instrumentpanel::~instrumentpanel() {
  /// Default destructor
  for(auto &it : instruments) {
    it->vessel->devices.remove(it);
    delete it;
    it = nullptr;
  }
  instruments.clear();
}


void instrumentpanel::attach(spacecraft *to_vessel) {
  /// Attach this panel to the specified ship
  if(!to_vessel) {
    std::cout << "ERROR: tried to attach instrument panel to null vessel." << std::endl;
    return;
  }
  vessel = to_vessel;
  vessel->panels.push_back(this);
}

void instrumentpanel::remove() {
  /// Remove this panel from whatever ship it's attached to
  /// Note: this cannot be safely called inside an iterator of panels!
  if(!vessel) {
    std::cout << "ERROR: tried to remove instrument panel which is already not attached to anything." << std::endl;
    return;
  }
  // remove it from the list of the vessel's panels
  vessel->panels.remove(this);
  vessel = nullptr;     // this must obviously come last
}

void instrumentpanel::destroy() {
  /// Destroy this panel and everything attached to it
  /// Note: this cannot be safely called inside an iterator of panels!
  std::cout << "Instrument panel is destroyed." << std::endl;
  //remove();                 // remove this panel
  delete this;
}

void instrumentpanel::render() {
  /// Render this panel and all its instruments
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

  for(auto const &it : instruments) {
    it->render();
  }

  glPopMatrix();
}
