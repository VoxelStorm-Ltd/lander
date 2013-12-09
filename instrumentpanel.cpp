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
  for(auto &it : devices) {
    //it->vessel->devices.remove(it);
    //delete it;
    //it = nullptr;
    it->remove_panel();
  }
  devices.clear();
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
  //remove();            // remove this panel
  delete this;
}

void instrumentpanel::render() {
  /// Render this panel and all its instruments
  glPushMatrix();
  // render the cabin panel
  glTranslated(position.x, position.y, position.z);
  glMultMatrixd(rotation.transform());

  //glColor4dv(Vector4d(0.5, 0.5, 0.5, 1.0));
  //glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,  Vector4f(1.0, 1.0, 1.0, 1.0));
  //glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,  Vector4f(1.0, 1.0, 1.0, 1.0));
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, Vector4f(0.5, 0.5, 0.5, 1.0));
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,            Vector4f(0.5, 0.5, 0.5, 1.0));
  glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION,            Vector4f(0.0, 0.0, 0.0, 1.0));
  glMaterialf(GL_FRONT_AND_BACK,  GL_SHININESS,           2.0);                           // 0 to 127

  glBegin(GL_QUADS);
  // front
  glNormal3d(0.0, 0.0, 1.0);
  //glVertex3d(0.0,    0.0,    0.0);
  //glVertex3d(size.x, 0.0,    0.0);
  //glVertex3d(size.x, size.y, 0.0);
  //glVertex3d(0.0,    size.y, 0.0);
  double const xstep = size.x / 100;
  double const ystep = size.y / 50;
  for(double x = 0.0; x <= size.x; x += xstep) {
    for(double y = 0.0; y <= size.y; y += ystep) {
      glVertex3d(x,         y,         0.0);
      glVertex3d(x + xstep, y,         0.0);
      glVertex3d(x + xstep, y + ystep, 0.0);
      glVertex3d(x,         y + ystep, 0.0);
    }
  }

  // top
  glNormal3d(0.0, 1.0, 0.0);
  glVertex3d(0.0,    size.y, -size.z);
  glVertex3d(0.0,    size.y, 0.0);
  glVertex3d(size.x, size.y, 0.0);
  glVertex3d(size.x, size.y, -size.z);
  // bottom
  glNormal3d(0.0, -1.0, 0.0);
  glVertex3d(0.0,    0.0,    -size.z);
  glVertex3d(size.x, 0.0,    -size.z);
  glVertex3d(size.x, 0.0,    0.0);
  glVertex3d(0.0,    0.0,    0.0);
  // right
  glNormal3d(1.0, 0.0, 0.0);
  glVertex3d(size.x, 0.0,    -size.z);
  glVertex3d(size.x, size.y, -size.z);
  glVertex3d(size.x, size.y, 0.0);
  glVertex3d(size.x, 0.0,    0.0);
  // left
  glNormal3d(-1.0, 0.0, 0.0);
  glVertex3d(0.0,    0.0,    -size.z);
  glVertex3d(0.0,    0.0,    0.0);
  glVertex3d(0.0,    size.y, 0.0);
  glVertex3d(0.0,    size.y, -size.z);
  glEnd();

  for(auto const &it : devices) {
    it->render();
  }

  // show connecting lines between components
  glPushAttrib(GL_LIGHTING_BIT);
  glDisable(GL_LIGHTING);
  glColor4d(0.0, 1.0, 0.0, 1.0);
  for(auto const &it : devices) {
    for(unsigned int i = 0; i != it->get_port_in_count(); ++i) {
      if(it->ports_in[i].target) {
        instrument *target = dynamic_cast<instrument*>(it->ports_in[i].target);
        if(target) {            // only proceed to link other instruments
          glBegin(GL_LINES);
          glVertex3d(it->get_position().x + it->get_size().x / 2,
                     it->get_position().y,
                     it->get_position().z + it->get_size().z / 2);
          glVertex3d(target->get_position().x + target->get_size().x / 2,
                     target->get_position().y + target->get_size().y,
                     target->get_position().z + target->get_size().z / 2);
          glEnd();
        } else {
          // show all other connections as exiting down
          glBegin(GL_LINES);
          glVertex3d(it->get_position().x + it->get_size().x / 2,
                     it->get_position().y,
                     it->get_position().z + it->get_size().z / 2);
          glVertex3d(it->get_position().x + it->get_size().x / 2, 0.0, 0.0);
          glEnd();
        }
      }
    }
  }
  glPopAttrib();

  glPopMatrix();
}
