#include "instrument.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "spacecraft.h"
#include "instrumentpanel.h"

instrument::instrument() {
  /// Default constructor
  size.x = 0.01;    // fallback 1cm cube
  size.y = 0.01;
  size.z = 0.01;
}

instrument::~instrument() {
  /// Default destructor
}

void instrument::attach(spacecraft *to_vessel) {
  /// Attach this device to the specified ship
  if(!to_vessel) {
    std::cout << "ERROR: tried to attach instrument " << get_name() << " to null vessel." << std::endl;
    return;
  }
  vessel = to_vessel;
  vessel->devices.push_back(this);
  // don't attach it to any panel by default
}

void instrument::attach(instrumentpanel *to_panel) {
  /// Attach this device to the specified panel
  if(!to_panel) {
    std::cout << "ERROR: tried to attach instrument " << get_name() << " to null panel." << std::endl;
    return;
  }
  panel = to_panel;
  panel->instruments.push_back(this);
}

void instrument::remove() {
  /// Remove this device from whatever ship it's attached to
  /// Note: not safe to be called in an iteration of instruments or devices!
  if(!vessel) {
    std::cout << "ERROR: tried to remove instrument " << get_name() << " which is already not attached to anything." << std::endl;
    return;
  }
  // detach it from whatever panel it's connected to
  remove_panel();
  // remove it from the list of the vessel's devices
  //vessel->devices.erase(std::remove(vessel->devices.begin(), vessel->devices.end(), this), vessel->devices.end());
  //vessel->instruments.erase(std::remove(vessel->instruments.begin(), vessel->devices.end(), this), vessel->instruments.end());
  vessel->devices.remove(this);
  disconnect_all();
  vessel = nullptr;     // this must obviously come last
}

void instrument::remove_panel() {
  /// Remove this instrument from its panel only (leave it attached to any ship)
  /// Note: not safe to be called in an iteration of instruments!
  if(!vessel) {
    //std::cout << "ERROR: tried to remove instrument " << get_name() << " which is already not attached to a panel." << std::endl;
    return;
  }
  panel->instruments.remove(this);
  panel = nullptr;     // this must obviously come last
}

//void instrument::update() {
//  /// Update the readings on this instrument and re-cache new values
//}

void instrument::render() {
  /// Show this instrument on the control panel
  // render a basic placeholder for unspecified instruments
  glPushMatrix();

  glTranslated(position.x,
               position.y,
               position.z);

  glColor4dv(Vector4d(0.2, 0.2, 0.2, 1.0));
  glBegin(GL_QUADS);
  // front
  glVertex3d(0.0,    0.0,    size.z);
  glVertex3d(size.x, 0.0,    size.z);
  glVertex3d(size.x, size.y, size.z);
  glVertex3d(0.0,    size.y, size.z);
  // top
  glVertex3d(0.0,    size.y, 0.0);
  glVertex3d(0.0,    size.y, size.z);
  glVertex3d(size.x, size.y, size.z);
  glVertex3d(size.x, size.y, 0.0);
  // bottom
  glVertex3d(0.0,    0.0,    0.0);
  glVertex3d(size.x, 0.0,    0.0);
  glVertex3d(size.x, 0.0,    size.z);
  glVertex3d(0.0,    0.0,    size.z);
  // right
  glVertex3d(size.x, 0.0,    0.0);
  glVertex3d(size.x, size.y, 0.0);
  glVertex3d(size.x, size.y, size.z);
  glVertex3d(size.x, 0.0,    size.z);
  // left
  glVertex3d(0.0,    0.0,    0.0);
  glVertex3d(0.0,    0.0,    size.z);
  glVertex3d(0.0,    size.y, size.z);
  glVertex3d(0.0,    size.y, 0.0);
  glEnd();

  glPopMatrix();
}
