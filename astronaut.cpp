#include "astronaut.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <FTGL/ftgl.h>
#include <iostream>
#include "vectorstorm/vectorstorm.h"
#include "universe.h"
#include "spacecraft.h"
#include "starsystem.h"
#include "device.h"

extern astronaut *player;
extern universe root;
extern menu *menu_target;
extern FTFont *font_title;
extern FTFont *font_title_huge;
extern FTFont *font_text;
extern FTFont *font_title3d;
extern FTFont *font_text3d;
extern unsigned int fps;

astronaut::astronaut()
  : state(statetype::INACTIVE),
    vessel_in(nullptr),
    walking_on(nullptr),
    strappeddown(false),
    fov_ratio(1.0),
    aspect_ratio(1.0),
    picked_device(nullptr),
    picked_body(nullptr) {
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
    return height * 0.550;                                                      // average height * average centre of gravity ratio
  }
}

void astronaut::enter_ship(spacecraft *ship) {
  /// Put the astronaut in the specified ship
  state = statetype::IN_VESSEL;
  vessel_in = ship;
  walking_on = nullptr;
  position.assign();                                                            // clear - assign defaults to 0
  velocity.assign();
  vessel_in->occupants.push_back(this);
  picked_body = vessel_in;
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
  picked_body = nullptr;
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
  matrix4d modelview;
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
  glColor4dv(vector4d(1.0, 1.0, 1.0, 1.0));
  glBegin(GL_LINES);
  glVertex3d(-thisradius, -thisradius, 0.0);
  glVertex3d( thisradius,  thisradius, 0.0);
  glEnd();
  glBegin(GL_LINES);
  glVertex3d(-thisradius,  thisradius, 0.0);
  glVertex3d( thisradius, -thisradius, 0.0);
  glEnd();

  glPopMatrix();                                                                // restore position & rotation
}

void astronaut::render_visible() {
  /// Render in the visible spectrum
  // TODO
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
    vessel_in->render_cabin();
    break;
  case statetype::EVA:
  case statetype::ATMOSPHERIC:                                                  // "or" equiv
  case statetype::SURFACE:                                                      // "or" equiv
    // translate and rotate to our view in the universe
    setup_render_perspective(0.5, 1406000000000.0);                             // far = heliopause
    // draw first person view from outside
    // TODO
    break;
  case statetype::INACTIVE:
  case statetype::DEAD:
    // pearly gates
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, windowsize.x, 0, windowsize.y, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);

    glColor4d(1.0, 1.0, 1.0, 1.0);
    std::string const s = "DEAD";
    double const l = font_title_huge->Advance(s.c_str(), s.length());
    font_title_huge->Render(s.c_str(), s.length(), FTPoint((windowsize.x / 2) - (l / 2), windowsize.y / 2), FTPoint(), FTGL::RENDER_FRONT);

    // TODO: scores and other interesting things

    glPopAttrib();
    return;
  }

  // draw HUD
  glPushAttrib(GL_ALL_ATTRIB_BITS);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0, windowsize.x, 0, windowsize.y, -1, 1);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_LIGHTING);
  //glEnable(GL_BLEND);
  //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  // Spacesuit status
  // TODO
  // Environment status
  // TODO
  // Ship status
  // TODO
  // Targeted device
  if(picked_device) {
    std::string s(picked_device->get_type());
    s += ": ";
    s += picked_device->get_name();
    double const l = font_text->Advance(s.c_str(), s.length());
    glColor4d(0.0, 1.0, 0.0, 0.5);
    font_text->Render(s.c_str(), s.length(), FTPoint((windowsize.x / 2) - (l / 2), 10), FTPoint(), FTGL::RENDER_FRONT);
  } else if(picked_body) {
    std::string s(picked_body->get_type());
    s += ": ";
    s += picked_body->get_name();
    double const l = font_text->Advance(s.c_str(), s.length());
    glColor4d(0.0, 1.0, 0.0, 0.5);
    font_text->Render(s.c_str(), s.length(), FTPoint((windowsize.x / 2) - (l / 2), 10), FTPoint(), FTGL::RENDER_FRONT);
  }
  // Menu
  if(menu_target) {
    // try to alias it to a device
    device *targetdevice = dynamic_cast<device*>(menu_target);
    if(targetdevice) {
      glColor4d(1.0, 1.0, 1.0, 1.0);
      std::string const s = targetdevice->get_name();
      double const l = font_text->Advance(s.c_str(), s.length());
      font_text->Render(s.c_str(), s.length(), FTPoint((windowsize.x / 2) - (l / 2), windowsize.y / 2), FTPoint(), FTGL::RENDER_FRONT);
    } else {
      // it's not a device, so try to alias it to a spacecraft
      body *targetbody = dynamic_cast<body*>(menu_target);
      if(targetbody) {
        glColor4d(1.0, 1.0, 1.0, 1.0);
        std::string const s = targetbody->get_name();
        double const l = font_text->Advance(s.c_str(), s.length());
        font_text->Render(s.c_str(), s.length(), FTPoint((windowsize.x / 2) - (l / 2), windowsize.y / 2), FTPoint(), FTGL::RENDER_FRONT);
      }
    }
  }

  // Debugging display
  glColor4d(0.0, 1.0, 0.0, 0.5);
  std::stringstream fpsss;
  fpsss << fps << " FPS";
  font_text->Render(fpsss.str().c_str(), -1, FTPoint(0, windowsize.y - 15), FTPoint(), FTGL::RENDER_FRONT);

  glPopAttrib();
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

void astronaut::update_window(vector2i newwindowsize) {
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

  //glMultMatrixd(rotation_head.transform());                                     // head rotation
  glRotated(rotation_head_pitch, 1.0, 0.0, 0.0);                                // head rotation
  if(rotation_head_yaw != 0.0) {
    glRotated(rotation_head_yaw, 0.0, 1.0, 0.0);
  }
  glTranslated(0.0, -1.7, 0.0);                                                 // eye height
  glMultMatrixd(rotation.transform().inverse());                                // body rotation
  glTranslated(-position.x,                                                     // position
               -position.y,
               -position.z);
}

void astronaut::rotate_mouse(vector2d mouse_pos) {
  /// React to mouse position updates
  // Quake mouselook code is here for reference: https://github.com/id-Software/Quake/blob/bf4ac424ce754894ac8f1dae6a3981954bc9852d/WinQuake/in_win.c
  vector2d mouse_diff = mouse_pos - mouse_last;
  mouse_last = mouse_pos;

  // apply sensitivity
  //mouse_diff *= mouse_sensitivity;
  mouse_diff *= mouse_sensitivity * fov_angle;

  // generate a rotation quaternion
  //quatd temp = quatd::from_euler_angles(mouse_diff.y, mouse_diff.x, 0.0);
  //quatd temp = quatd::from_axis_rot(vector3d(1, 0, 0), mouse_diff.y) *
  //             quatd::from_axis_rot(vector3d(0, 1, 0), mouse_diff.x);
  //rotation_head = temp * rotation_head;
  //rotation_head.normalise();

  if(strappeddown) {
    // while strapped down mouse turns only the head
    rotation_head_yaw += mouse_diff.x;
  } else {
    // while walking mouse turns the body, not the head
    quatd yaw = quatd::from_axis_rot(vector3d(0, -1, 0), mouse_diff.x);
    rotation = yaw * rotation;
    rotation_head_yaw = 0.0;
    // TODO: turn head instantly but have body follow gradually
  }

  rotation_head_pitch += mouse_diff.y;
  if(rotation_head_pitch > 80) {
    rotation_head_pitch = 80;
  } else if(rotation_head_pitch < -80) {
    rotation_head_pitch = -80;
  }
  //rotation = quatd::from_axis_rot(vector3d(-1, 0, 0), mouse_diff.y) * rotation;

  // update our picked objects
  pick();
}

void astronaut::pick() {
  /// Update the picked objects with whatever's under the current cursor
  switch(state) {
  case statetype::IN_VESSEL:
    {
      vector3d head_vector(0.0, 1.7, 0.0);
      head_vector.rotate(rotation);
      vector3d const head_position = position + head_vector;

      vector3d facing_vector(0.0, 0.0, 1.0);
      if(rotation_head_yaw != 0.0) {
        facing_vector.rotate(0.0, rotation_head_yaw, 0.0);
      }
      facing_vector.rotate(rotation_head_pitch, 0.0, 0.0);
      facing_vector.rotate(rotation);

      picked_device = vessel_in->pick_cabin(head_position, facing_vector);
      //picked_device = vessel_in->pick_cabin(position, facing_vector);
      //picked_body = vessel_in;
      // the above is set when we get in a ship and unset when we get out
    }
    break;
  case statetype::EVA:
  case statetype::ATMOSPHERIC:                                                  // "or" equiv
  case statetype::SURFACE:                                                      // "or" equiv
    for(auto const &it : root.currentsystem->bodies) {
      // TODO: move this to starsystem class
      // TODO: check for intersections and then pick within them depending on what we get, use polymorphism ftw
    }
    break;
  default:
    // nothing doing
    break;
  }
  //std::cout << "DEBUG: picked_device " << picked_device << " picked_body " << picked_body << std::endl;
}

void astronaut::cursor_activate() {
  /// Activate the object that we are currently looking at
  if(picked_device) {
    // we're looking at a device, either in a ship or outside
    std::cout << "DEBUG: clicked on device " << picked_device->get_name() << std::endl;
    picked_device->activate();
    return;
  } else if(picked_body) {
    // we're looking at some sort of body - act based on what it is
    std::cout << "DEBUG: clicked on body " << picked_body->get_name() << std::endl;
    // TODO
  } else {
    // we're picking nothing local - whatever it is, we can't affect it with a normal click
  }
}

void astronaut::cursor_menu() {
  /// Open a menu on the object we are currently looking at
  if(picked_device) {
    // we're looking at a device, either in a ship or outside
    std::cout << "DEBUG: requested menu for device " << picked_device->get_name() << std::endl;
    menu_target = picked_device;
    return;
  } else if(picked_body) {
    // we're looking at some sort of body - act based on what it is
    std::cout << "DEBUG: requested menu for body " << picked_body->get_name() << std::endl;
    menu_target = picked_body;
  } else {
    // we're picking nothing local - check what's here in the wider universe
    // TODO
  }
}

void astronaut::cursor_menu_close() {
  /// Close the currently open menu if there is one, and carry out whatever function we ended on
  std::cout << "DEBUG: closed menu" << std::endl;
  // TODO: take any action as appropriate when closing the menu
  menu_target = nullptr;
}
