#include "instrumentpanel.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <FTGL/ftgl.h>
#include <spacecraft.h>
#include <instrument.h>

extern FTFont *fontconsole;                                                     // global font definitions

instrumentpanel::instrumentpanel()
  : vessel(nullptr),
    pickpoint_show(false),
    pickeddevice(nullptr) {
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
  vessel = nullptr;                                                             // this must obviously come last
}

void instrumentpanel::destroy() {
  /// Destroy this panel and everything attached to it
  /// Note: this cannot be safely called inside an iterator of panels!
  std::cout << "Instrument panel is destroyed." << std::endl;
  //remove();                                                                     // remove this panel
  delete this;
}

void instrumentpanel::render() {
  /// Render this panel and all its instruments
  glPushMatrix();
  // render the cabin panel
  glTranslated(position.x, position.y, position.z);
  glMultMatrixd(rotation.transform());

  //glColor4dv(Vector4d(0.5, 0.5, 0.5, 1.0));
  //glMaterialfv(GL_FRONT, GL_AMBIENT,  Vector4f(1.0, 1.0, 1.0, 1.0));
  //glMaterialfv(GL_FRONT, GL_DIFFUSE,  Vector4f(1.0, 1.0, 1.0, 1.0));
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, Vector4f(0.5, 0.5, 0.5, 1.0));
  glMaterialfv(GL_FRONT, GL_SPECULAR,            Vector4f(0.5, 0.5, 0.5, 1.0));
  glMaterialfv(GL_FRONT, GL_EMISSION,            Vector4f(0.0, 0.0, 0.0, 1.0));
  glMaterialf(GL_FRONT,  GL_SHININESS,           2.0);                          // 0 to 127

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
        Vector3d thissize = it->get_size();
        Vector3d port_position = it->get_position() + Vector3d((thissize.x / (it->get_port_in_count() + 1)) * (i + 1),
                                                               0.0,
                                                               thissize.z / 2);
        device *target = it->ports_in[i].target;
        if(target->panel == this) {                                             // only proceed to link other instruments on this panel
          Vector3d targetsize = target->get_size();
          Vector3d target_position = target->get_position() + Vector3d((targetsize.x / (target->get_port_out_count() + 1) * (it->ports_in[i].target_port + 1)),
                                                                       targetsize.y,
                                                                       targetsize.z / 2);
          glBegin(GL_LINES);
          glVertex3dv(port_position);
          glVertex3dv(target_position);
          glEnd();
        } else {
          // show all other connections as exiting down
          glBegin(GL_LINES);
          glVertex3dv(port_position);
          glVertex3d(port_position.x, 0.0, 0.0);
          glEnd();
        }
      }
    }
  }

  // show a cursor if there's one to show
  double edge_left, edge_right, edge_bottom, edge_top;
  if(pickpoint_show) {
    if(pickeddevice) {
      Vector3d const &thispos  = pickeddevice->get_position();
      Vector3d const &thissize = pickeddevice->get_size();
      edge_left   = thispos.x              - 0.005;
      edge_right  = thispos.x + thissize.x + 0.005;
      edge_bottom = thispos.y              - 0.005;
      edge_top    = thispos.y + thissize.y + 0.005;
    } else {
      edge_left   = pickpoint.x - 0.02;
      edge_right  = pickpoint.x + 0.02;
      edge_bottom = pickpoint.y - 0.02;
      edge_top    = pickpoint.y + 0.02;

      // clamp the edges
      if(edge_left < 0.0) {
        edge_left = 0.0;
      } else if(edge_right > size.x) {
        edge_right = size.x;
      }
      if(edge_bottom < 0.0) {
        edge_bottom = 0.0;
      } else if(edge_top > size.y) {
        edge_top = size.y;
      }
    }
    glBegin(GL_LINES);
    glVertex3d(edge_left        , edge_bottom + 0.01, 0.01);                    // bottom left
    glVertex3d(edge_left        , edge_bottom       , 0.01);
    glVertex3d(edge_left        , edge_bottom       , 0.01);
    glVertex3d(edge_left  + 0.01, edge_bottom       , 0.01);

    glVertex3d(edge_right - 0.01, edge_bottom       , 0.01);                    // bottom right
    glVertex3d(edge_right       , edge_bottom       , 0.01);
    glVertex3d(edge_right       , edge_bottom       , 0.01);
    glVertex3d(edge_right       , edge_bottom + 0.01, 0.01);

    glVertex3d(edge_right       , edge_top    - 0.01, 0.01);                    // top right
    glVertex3d(edge_right       , edge_top          , 0.01);
    glVertex3d(edge_right       , edge_top          , 0.01);
    glVertex3d(edge_right - 0.01, edge_top          , 0.01);

    glVertex3d(edge_left  + 0.01, edge_top          , 0.01);                    // top left
    glVertex3d(edge_left        , edge_top          , 0.01);
    glVertex3d(edge_left        , edge_top          , 0.01);
    glVertex3d(edge_left        , edge_top    - 0.01, 0.01);
    glEnd();
  }

  glPopAttrib();

  //// DEBUG ONLY: font testing
  //glPushAttrib(GL_ALL_ATTRIB_BITS);
  //glDisable(GL_LIGHTING);
  //glDisable(GL_DEPTH_TEST);
  //for(auto const &it : devices) {
  //  glPushMatrix();
  //  glTranslated(it->get_position().x,
  //               it->get_position().y,
  //               it->get_position().z + it->get_size().z);
  //  glScaled(0.00035277777, 0.00035277777, 0.00035277777);   // 1m / (72dpi * 39.3700787in) = 0.00035277777
  //  glColor4d(1.0, 1.0, 1.0, 0.75);
  //  //glColor4d(1.0, 0.0, 0.0, 1.0);
  //  std::stringstream ss;
  //  ss << it->get_manufacturer() << " " << it->get_model();
  //  fontconsole->Render(ss.str().c_str(), -1, FTPoint(), FTPoint(), FTGL::RENDER_FRONT);
  //  glPopMatrix();
  //}
  //glPopAttrib();

  glPopMatrix();
}

device *instrumentpanel::pick(Vector3d const &origin, Vector3d const &pickvector) {
  /// Pick into this panel and return a pointer to a device if there is one
  // rotate our test vector by the device's orientation
  Vector3d local_vect(pickvector);
  Vector3d offset = origin - position;
  local_vect.rotate(rotation);
  offset.rotate(rotation.conjugate_copy());

  pickpoint.x = offset.x + (offset.z / local_vect.z * -local_vect.x);
  pickpoint.y = offset.y + (offset.z / local_vect.z *  local_vect.y);

  // edge clamping
  //if(pickpoint.x < 0.0) {
  //  pickpoint.x = 0.0;
  //} else if(pickpoint.x > it->size.x - picktestdevice->get_size().x) {
  //  pickpoint.x = it->size.x - picktestdevice->get_size().x;
  //}
  //if(pickpoint.y < 0.0) {
  //  pickpoint.y = 0.0;
  //} else if(pickpoint.y > it->size.y - picktestdevice->get_size().y) {
  //  pickpoint.y = it->size.y - picktestdevice->get_size().y;
  //}

  // check against the panel's bounding rectangle
  if(pickpoint.x < 0.0 ||
     pickpoint.y < 0.0 ||
     pickpoint.x > size.x ||
     pickpoint.y > size.y) {
    // don't show a cursor
    pickpoint_show = false;
    return nullptr;
  }

  // we're looking at this panel, so iterate through its devices
  for(auto const &it : devices) {
    if(it->pick(Vector2d(pickpoint.x, pickpoint.y))) {
      // we've found our device
      //std::cout << "DEBUG: picking device " << itd->get_name() << std::endl;
      pickeddevice = it;
      return it;
    }
  }

  // nothing found, show the cursor
  pickeddevice = nullptr;
  pickpoint_show = true;
  //std::cout << "DEBUG: picking panel " << itd->get_name() << std::endl;
  return nullptr;
}
