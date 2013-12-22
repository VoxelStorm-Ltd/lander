#include "instrumentpanel.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <FTGL/ftgl.h>
#include <spacecraft.h>
#include <instrument.h>

extern FTFont *fontconsole;          // global font definitions

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
  double const xstep = size.x / 40;
  double const ystep = size.y / 20;
  double const zstep = size.z / 5;
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
  for(double x = 0.0; x <= size.x; x += xstep) {
    for(double z = 0.0; z < size.z; z += zstep) {
      glVertex3d(x,         0.0, -(z + zstep));
      glVertex3d(x + xstep, 0.0, -(z + zstep));
      glVertex3d(x + xstep, 0.0, -z);
      glVertex3d(x,         0.0, -z);
    }
  }
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
  glColor4d(0.0, 1.0, 0.0, 0.75);
  for(auto const &it : devices) {
    for(unsigned int i = 0; i != it->get_port_in_count(); ++i) {
      if(it->ports_in[i].target) {
        device *target = it->ports_in[i].target;
        if(target->panel == this) {            // only proceed to link other instruments on this panel
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

  // DEBUG ONLY: font testing
  glPushAttrib(GL_ALL_ATTRIB_BITS);
  glDisable(GL_LIGHTING);
  glDisable(GL_DEPTH_TEST);
  for(auto const &it : devices) {
    glPushMatrix();

    glTranslated(it->get_position().x,
                 it->get_position().y,
                 it->get_position().z + it->get_size().z);

    glScaled(0.00035277777, 0.00035277777, 0.00035277777);   // 1m / (72dpi * 39.3700787in) = 0.00035277777

    glColor4d(1.0, 1.0, 1.0, 0.75);
    //glColor4d(1.0, 0.0, 0.0, 1.0);
    std::stringstream ss;
    ss << it->get_manufacturer() << " " << it->get_model();

    ///fontconsole->Render(ss.str().c_str(), -1, FTPoint(), FTPoint(), FTGL::RENDER_FRONT);

    glPopMatrix();
  }
  glPopAttrib();

  glPopMatrix();
}

device *instrumentpanel::pick(Vector3d const &origin, Vector3d const &pickvector) {
  /// Pick into this panel and return a pointer to a device if there is one
  // rotate our test vector by the device's orientation
  Vector3d local_vect(pickvector);
  Vector3d offset = origin - position;
  local_vect.rotate(rotation);
  offset.rotate(rotation.conjugate_copy());

  Vector2d pickpos;
  pickpos.x = offset.x + (offset.z / local_vect.z * -local_vect.x);
  pickpos.y = offset.y + (offset.z / local_vect.z *  local_vect.y);

  // edge clamping
  //if(pickpos.x < 0.0) {
  //  pickpos.x = 0.0;
  //} else if(pickpos.x > it->size.x - picktestdevice->get_size().x) {
  //  pickpos.x = it->size.x - picktestdevice->get_size().x;
  //}
  //if(pickpos.y < 0.0) {
  //  pickpos.y = 0.0;
  //} else if(pickpos.y > it->size.y - picktestdevice->get_size().y) {
  //  pickpos.y = it->size.y - picktestdevice->get_size().y;
  //}

  // check against the panel's bounding rectangle
  if(pickpos.x < 0.0 ||
     pickpos.y < 0.0 ||
     pickpos.x > size.x ||
     pickpos.y > size.y) {
    return nullptr;
  }
  //std::cout << "DEBUG: picking panel " << itd->get_name() << std::endl;

  // we're looking at this panel, so iterate through its devices
  for(auto const &it : devices) {
    if(it->pick(Vector2d(pickpos.x, pickpos.y))) {
      // we've found our device
      //std::cout << "DEBUG: picking device " << itd->get_name() << std::endl;
      return it;
    }
  }
  // nothing found
  return nullptr;
}
