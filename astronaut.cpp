#include "astronaut.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "spacecraft.h"

extern astronaut *player;

astronaut::astronaut()
  : state(statetype::INACTIVE),
    vessel_in(nullptr),
    walking_on(nullptr),
    strappeddown(false),
    fov_ratio(1.0),
    aspect_ratio(1.0) {
  /// Default constructor
  set_mass(0.0);
  set_radius(0.0);
  rotation_head_yaw   = 0.0;
  rotation_head_pitch = 0.0;
  mouse_sensitivity.x = mouse_sensitivity.y = 0.005;
  update_fov(90.0);
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

void astronaut::render_firstperson() {
  /// Render from this astronaut's first person view
  switch(state) {
  case statetype::IN_VESSEL:
    // translate and rotate to player's view in the cabin
    setup_render_perspective(0.1, 20.0);
    // draw cabin view of the current ship
    player->vessel_in->render_cabin();
    break;
  case statetype::EVA:
  case statetype::ATMOSPHERIC:     // "or" equiv
  case statetype::SURFACE:         // "or" equiv
    // translate and rotate to player's view in the universe
    setup_render_perspective(0.5, 1406000000000.0);    // far = heliopause
    // draw first person view from outside
    // TODO
    break;
  case statetype::INACTIVE:
  case statetype::DEAD:
    // pearly gates
    // TODO
    break;
  }
}

void astronaut::update_fov(double new_fov) {
  fov_angle = new_fov;
  update_fov_ratio();
}

void astronaut::update_fov_ratio() {
  /// Helper function to calculate field of view ratio from a field of view angle
  // fov_ratio = 1.0;
  fov_ratio = tan(fov_angle / 360.0 * M_PI);
  //std::cout << "New FOV ratio: " << fov_ratio << std::endl;
}

void astronaut::update_window(Vector2i newwindowsize) {
  windowsize = newwindowsize;
  glViewport(0, 0, windowsize.x, windowsize.y);
  //glViewport(-windowsize.x, -windowsize.y, windowsize.x, windowsize.y);
  update_aspect_ratio();
}

void astronaut::update_aspect_ratio() {
  // aspect_ratio = 1.0;
  aspect_ratio = static_cast<double>(windowsize.y) / static_cast<double>(windowsize.x);
  //std::cout << "New window size " << windowsize << " aspect ratio " << aspect_ratio << std::endl;
}

void astronaut::setup_render_perspective(double nearplane,
                                         double farplane) {
  /// Set up the projection matrix to the correct specifications
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glFrustum(nearplane * -fov_ratio,
            nearplane *  fov_ratio,
            nearplane * -fov_ratio * aspect_ratio,
            nearplane *  fov_ratio * aspect_ratio,
            nearplane, farplane);
  // TODO: cache this matrix in a separate update function called on windowresize only, and load it here

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  //glMultMatrixd(rotation_head.transform());         // head rotation
  glRotated(rotation_head_pitch, 1.0, 0.0, 0.0);    // head rotation
  if(rotation_head_yaw != 0.0) {
    glRotated(rotation_head_yaw,   0.0, 1.0, 0.0);
  }
  glTranslated(0.0, -1.7, 0.0);                     // eye height
  glMultMatrixd(rotation.transform().inverse());    // body rotation

  glTranslated(-position.x,                         // position
               -position.y,
               -position.z);
}
