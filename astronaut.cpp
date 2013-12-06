#include "astronaut.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "spacecraft.h"

extern astronaut *player;

astronaut::astronaut()
  : state(statetype::INACTIVE),
    vessel_in(nullptr),
    walking_on(nullptr) {
  /// Default constructor
  set_mass(0.0);
  set_radius(0.0);
}

astronaut::~astronaut() {
  /// Default destructor
}

double astronaut::get_mass() {
  if(mass != 0.0) {
    return mass;
  } else {
    // no mass assigned, take a potshot at one around 70kg, plus ~10kg for a spacesuit
    random_reset();
    return get_random_double(70.0, 90.0);
  }
}
double astronaut::get_radius() {
  if(radius != 0.0) {
    return radius;
  } else {
    // take a guess at radius (aka half height) based on human mass ~ 70kg
    // assume spacesuit weighs 10kg
    double const nakedmass = get_mass() - 10.0;
    // adjust range 60kg to 1.65m and 90kg to 1.84m
    double const height = (((nakedmass - 60.0) / (90.0 - 60.0) * (1.84 - 1.65)) + 1.65);
    return height * 0.550;   // average height * average centre of gravity ratio
  }
}

void astronaut::enter_ship(spacecraft *ship) {
  /// Put the astronaut in the specified ship
  state = statetype::IN_VESSEL;
  vessel_in = ship;
  walking_on = nullptr;
  position.assign();      // clear - assign defaults to 0
  velocity.assign();
  vessel_in->occupants.push_back(this);
}

void astronaut::exit_ship() {
  /// Exit the current ship, if any
  if(state != statetype::IN_VESSEL) {
    std::cout << "WARNING: astronaut " << get_name() << " told to exit ship while not in one, state is " << static_cast<int>(state) << std::endl;
    return;
  }
  position = vessel_in->position;
  velocity = vessel_in->velocity;
  vessel_in->occupants.remove(this);
  vessel_in = nullptr;
  // TODO: decide if we're on the ground or in space or what
  state = statetype::EVA;
}

void astronaut::kill() {
  /// Kill this astronaut, and perform whatever consequences are required
  std::cout << get_name() << " has been killed." << std::endl;
  state = statetype::DEAD;
  // check if it's the player
  if(this == player) {
    std::cout << "Congratulations!  You died." << std::endl;
  }
}

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
