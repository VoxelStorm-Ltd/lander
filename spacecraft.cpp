#include "spacecraft.h"
#include <boost/units/systems/si/velocity.hpp>
#include <boost/units/base_units/imperial/mile.hpp>
#include <boost/units/base_units/metric/hour.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "vectorstorm/vectorstorm.h"
#include "universe.h"
#include "starsystem.h"
#include "device.h"
#include "astronaut.h"
#include "instrumentpanel.h"
#include "dustcloud.h"
#include "thruster.h"

extern universe root;
extern astronaut *player;

spacecraft::spacecraft()
  : temperature_hull(285.18),                                                   // http://www.learnthermo.com/examples/ch04/p-4b-3.php
    temperature_cabin(21.0 + 273.15) {
  /// Default constructor
}

spacecraft::~spacecraft() {
  /// Default destructor
  //for(auto &it : panels) {
  //  delete it;
  //  it = nullptr;
  //}
  panels.clear();
  // every system is destroyed
  for(auto &it : devices) {
    delete it;
    it = nullptr;
  }
  devices.clear();
  // everybody on board dies
  for(auto const &it : occupants) {
    it->kill();
  }

  //for(auto &it : root.currentsystem->bodies) {
  //  // make sure we don't leave a dangling pointer to this
  //  if(it == this) {
  //    it = nullptr;
  //  }
  //}
  root.currentsystem->bodies.remove(this);
}

std::string spacecraft::get_name() {
  if(name.size() != 0) {
    return name;
  } else {
    // random ship name
    random_reset();                                                             // reset the generator to its seed
    std::stringstream randomname;
    randomname << get_random_name_ancient() << " Mk" << get_random_uint(1, 5);
    return randomname.str();
  }
}

std::string spacecraft::get_type() {
  /// Return the type of object this is
  return "spacecraft";
}

std::string spacecraft::get_designation() {
  if(designation.size() != 0) {
    return designation;
  } else {
    // generate a random elite-style ship registration or star trek NCC-1701
    random_reset();                                                             // reset the generator to its seed
    std::stringstream designation;
    designation << get_random_char_alpha_upper()
                << get_random_char_alpha_upper()
                << get_random_char_alpha_upper()
                << "-"
                << get_random_uint(1000, 9999);
    return designation.str();
  }
}

std::string spacecraft::get_description() {
  return description;
}

double spacecraft::get_mass() {
  if(mass != 0.0) {
    return mass;
  } else {
    // no mass assigned, make a guess for a lander type vessel, 1 to 3x the mass of the apollo 11
    random_reset();
    return get_random_double(15000.0, 45000.0);                                 // averaging 30 tons +- 50%
  }
}

double spacecraft::get_radius() {
  if(radius != 0.0) {
    return radius;
  } else {
    // make up a radius for this based on what's likely for the mass
    // i.e. 253 Mathilde = 10330000000000000kg (1.033 * 10^17), radius ~= 25000m
    // assuming volume as sphere ~= 4/3 PI * r^3 ~= 65449846949787m^3
    // asteroid density = mass / volume ~= 157.83
    // inverse = 0.00633
    double const volume = get_mass() * 0.00633;
    return pow(volume / ((4.0 / 3.0) * M_PI), 1.0 / 3.0);                       // radius from volume of sphere
  }
}

double spacecraft::get_temperature_hull() {
  /// Calculate and return the (average) hull temperature
  return temperature_hull;
}

double spacecraft::get_temperature_cabin() {
  /// Calculate and return the cabin temperature
  return temperature_cabin;
}

device *spacecraft::pick_cabin(Vector3d const &origin, Vector3d const &pickvector) {
  /// try to find the device pointed at by the current cursor in the cabin
  for(auto const &it : panels) {
    device *const pickeddevice = it->pick(origin, pickvector);
    if(pickeddevice) {
      return pickeddevice;
    }
  }
  for(auto const &it : devices_cabin) {
    // TODO
  }
  return nullptr;                                                               // not found
}

device *spacecraft::pick_hull(Vector3d const &origin, Vector3d const &pickvector) {
  /// try to find the device pointed at by the current cursor on the hull
  for(auto const &it : devices_hull) {
    // TODO
  }
  return nullptr;
}

Vector3d spacecraft::get_acceleration(Vector3d const &thisposition,
                                      Vector3d const &thisvelocity __attribute__((__unused__)),
                                      double time __attribute__((__unused__))) {
  /// placeholder acceleration due to gravity
  Vector3d acceleration;
  // iterate through every sufficiently significant body
  for(auto *it : root.currentsystem->bodies) {
    if(it == this) {
      // don't calculate gravitational effect of ourselves
      continue;
    }
    acceleration += it->get_gravity_accel_v3(thisposition);
  }
  // add propulsion effects
  acceleration += acceleration_last;
  //std::cout << "DEBUG: thrust = " << thrust << "N, mass = " << get_mass() << "kg, accel = " << thrust.length() / get_mass() << "m/s^2" << std::endl;
  return acceleration;
}

void spacecraft::update_state(double time, double deltatime) {
  /// update all relevant state information
  // update temperatures
  // search local star system for planets and check for atmospheric interaction
  // search local star system for suns and calculate their cumulative radiation
  // take into account shadowing by planets
  //temperature_hull =
  //temperature_cabin =
  // TODO

  // update cabin atmospheric situation
  // TODO

  // update our connected devices, if appropriate
  for(auto const &it : devices_hull) {
    it->update_if_time();
  }
  //for(auto const &it : devices_cabin) {
  //  it->update_if_time();
  //}

  // add propulsion effects
  Vector3d thrust;
  for(auto *it : thrusters) {
    // add individual contribution of each engine
    thrust += it->get_thrust();
    // TODO: rotation
  }
  thrust.rotate(rotation);
  acceleration_last = thrust / get_mass();

  // check collisions
  for(auto const &it : root.currentsystem->bodies) {
    // skip checking ourselves
    if(it == this) {
      continue;
    }
    // check spheres of influence / bounding boxes first
    if(!it->check_within_physical_influence(position)) {
      continue;
    }

    // TODO: check collision on all corners
    Vector3d const collision_normal = it->get_collision(position);
    if(collision_normal != Vector3d(0.0, 0.0, 0.0)) {
      // we've collided with something
      Vector3d const velocity_delta = velocity - it->velocity;
      double const velocity_delta_mag = velocity_delta.length();

      if(velocity_delta_mag > 22) {                                             // ~50mph
        std::cout << "INFO: " << get_name() << " collided with " << it->get_name() << " at a fatal " << velocity_delta_mag << "m/s" << std::endl;
        // if the ship was destroyed, give a snide message relating to the kinetic energy and collateral damage
        double const ke = 0.5 * get_mass() * velocity_delta_mag * velocity_delta_mag;
        Vector3d const lastposition(position);                                  // cache position for after we delete "this"
        destroy();
        root.make_explosion(lastposition, ke);
        return;
      } else {
        std::cout << "INFO: " << get_name() << " landed safely on " << it->get_name() << " at a gentle " << velocity_delta_mag << "m/s" << std::endl;
      }
    }

    // check atmospheric effects
    // TODO
  }

  // the default integrator for position and velocity
  integrate(position, velocity, time, deltatime);
}

void spacecraft::destroy() {
  /// Blow up or otherwise annihilate a ship destructively
  std::cout << "Spacecraft " << get_name() << " has been fatally damaged." << std::endl;
  //root.currentsystem->bodies.remove(this);                                      // this breaks the iterator, and is always called from inside it...
  // replace it in the list with a dustcloud
  for(auto &it : root.currentsystem->bodies) {
    if(it == this) {
      spacecraft *oldship = static_cast<spacecraft*>(it);                       // we know a static cast is safe since we've already confirmed it's a pointer to this object
      it = new dustcloud;
      it->position = oldship->position;
      it->rotation = oldship->rotation;
      it->spin     = oldship->spin;
    }
  }

  delete this;
}

void spacecraft::render_diagram(double scale, bool labels) {
  /// Render in the orthographic diagram view
  glPushMatrix();
  // move into position
  glTranslated(position.x, position.y, position.z);
  glMultMatrixd(rotation.transform());

  if(this == player->vessel_in) {                                               // show navigation vectors only for the player ship
    glColor4dv(Vector4d(1.0, 0.6, 0.2, 0.75));                                  // orange
    glBegin(GL_LINES);
    glVertex3d(0.0, -100 / scale, 0.0);                                         // heading vector
    glVertex3d(0.0, -120 / scale, 0.0);
    glColor4dv(Vector4d(1.0, 0.2, 0.2, 0.75));                                  // port light (red)
    glVertex3d(-100 / scale, -5 / scale, 0.0);
    glVertex3d(-100 / scale,  5 / scale, 0.0);
    glColor4dv(Vector4d(0.2, 1.0, 0.0, 0.75));                                  // starboard light (green)
    glVertex3d( 100 / scale, -5 / scale, 0.0);
    glVertex3d( 100 / scale,  5 / scale, 0.0);
    glEnd();
  }

  double thisradius = get_radius();
  if(thisradius * scale < 4.0) {
    thisradius = 4.0 / scale;
  }
  // draw an asteroids-style arrow at the radius
  glColor4dv(Vector4d(1.0, 1.0, 1.0, 1.0));
  // 2D, y = up:
  //glBegin(GL_LINE_LOOP);
  //glVertex3d(0.0, -thisradius, 0.0);
  //glVertex3d(-thisradius / sqrt(2.0), thisradius / sqrt(2.0), 0.0);
  //glVertex3d(0.0, thisradius / 2.0, 0.0);
  //glVertex3d( thisradius / sqrt(2.0), thisradius / sqrt(2.0), 0.0);
  //glEnd();
  // 2D, z = up:
  //glBegin(GL_LINE_LOOP);
  //glVertex3d(0.0, 0.0, -thisradius);
  //glVertex3d(-thisradius / sqrt(2.0), 0.0, thisradius / sqrt(2.0));
  //glVertex3d(0.0, 0.0, thisradius / 2.0);
  //glVertex3d( thisradius / sqrt(2.0), 0.0, thisradius / sqrt(2.0));
  //glEnd();
  // 3D, y = up, X shape:
  glBegin(GL_LINE_LOOP);
  glVertex3d(0.0, -thisradius, 0.0);
  glVertex3d(-thisradius / sqrt(2.0), thisradius / sqrt(2.0), -thisradius / sqrt(2.0));
  glVertex3d(0.0, thisradius / 2.0, 0.0);
  glVertex3d( thisradius / sqrt(2.0), thisradius / sqrt(2.0),  thisradius / sqrt(2.0));
  glEnd();
  glBegin(GL_LINE_LOOP);
  glVertex3d(0.0, -thisradius, 0.0);
  glVertex3d(-thisradius / sqrt(2.0), thisradius / sqrt(2.0),  thisradius / sqrt(2.0));
  glVertex3d(0.0, thisradius / 2.0, 0.0);
  glVertex3d( thisradius / sqrt(2.0), thisradius / sqrt(2.0), -thisradius / sqrt(2.0));
  glEnd();

  glPopMatrix();                                                                // restore position & rotation
}

void spacecraft::render_visible() {
  /// Render in the visible spectrum
  // TODO
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

void spacecraft::render_cabin() {
  /// Render the interior of the cabin with instruments etc (visible spectrum)
  glPushMatrix();
  // white light on front wall
  glLightfv(GL_LIGHT0, GL_DIFFUSE,               Vector4f(1.0, 1.0, 1.0, 1.0));
  glLightfv(GL_LIGHT0, GL_SPECULAR,              Vector4f(1.0, 1.0, 1.0, 1.0));
  glLightfv(GL_LIGHT0, GL_POSITION,              Vector4f(0.0, 1.95, -1.95, 1.0));
  glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION,        Vector3f(0.0, -1.0, 0.0));
  glLightf( GL_LIGHT0, GL_SPOT_EXPONENT,         10.0);                         // accepts 0-128
  glLightf( GL_LIGHT0, GL_SPOT_CUTOFF,           60.0);                         // width of angle of spotlight, 180 = all-round
  glLightf( GL_LIGHT0, GL_CONSTANT_ATTENUATION,  0.0);                          // attenuation factor = 1 / (constant + linear * d + quadratic * d^2)
  glLightf( GL_LIGHT0, GL_LINEAR_ATTENUATION,    0.0001);
  glLightf( GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 1.0 / 3.0);                    // effective brightness
  glEnable( GL_LIGHT0);

  // red side up-light
  glLightfv(GL_LIGHT1, GL_DIFFUSE,               Vector4f(1.0, 0.0, 0.0, 1.0));
  glLightfv(GL_LIGHT1, GL_SPECULAR,              Vector4f(1.0, 0.0, 0.0, 1.0));
  glLightfv(GL_LIGHT1, GL_POSITION,              Vector4f(-1.95, 0.0, 0.0, 1.0));
  glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION,        Vector3f(0.0, 1.0, 0.0));
  glLightf( GL_LIGHT1, GL_SPOT_EXPONENT,         1.0);                          // accepts 0-128
  glLightf( GL_LIGHT1, GL_SPOT_CUTOFF,           90.0);                         // width of angle of spotlight, 180 = all-round
  glLightf( GL_LIGHT1, GL_CONSTANT_ATTENUATION,  0.0);
  glLightf( GL_LIGHT1, GL_LINEAR_ATTENUATION,    0.0001);
  glLightf( GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 1.0 / 3.0);                    // effective brightness
  glEnable( GL_LIGHT1);

  // yellow right side-light
  glLightfv(GL_LIGHT2, GL_DIFFUSE,               Vector4f(1.0, 1.0, 0.5, 1.0));
  glLightfv(GL_LIGHT2, GL_SPECULAR,              Vector4f(1.0, 1.0, 0.5, 1.0));
  glLightfv(GL_LIGHT2, GL_POSITION,              Vector4f(1.95, 1.5, 0.0, 1.0));
  glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION,        Vector3f(-1.0, 0.0, 0.0));
  glLightf( GL_LIGHT2, GL_SPOT_EXPONENT,         0.0);                          // accepts 0-128
  glLightf( GL_LIGHT2, GL_SPOT_CUTOFF,           180.0);                        // width of angle of spotlight, 180 = all-round
  glLightf( GL_LIGHT2, GL_CONSTANT_ATTENUATION,  0.0);
  glLightf( GL_LIGHT2, GL_LINEAR_ATTENUATION,    0.0001);
  glLightf( GL_LIGHT2, GL_QUADRATIC_ATTENUATION, 1.0 / 1.0);                    // effective brightness
  glEnable( GL_LIGHT2);

  // white overhead panel light
  //glLightfv(GL_LIGHT3, GL_DIFFUSE,               Vector4f(0.2, 0.2, 0.2, 1.0));
  //glLightfv(GL_LIGHT3, GL_SPECULAR,              Vector4f(0.2, 0.2, 0.2, 1.0));
  //glLightfv(GL_LIGHT3, GL_DIFFUSE,               Vector4f(0.5, 0.5, 0.5, 1.0));
  //glLightfv(GL_LIGHT3, GL_SPECULAR,              Vector4f(0.5, 0.5, 0.5, 1.0));
  glLightfv(GL_LIGHT3, GL_DIFFUSE,               Vector4f(1.0, 1.0, 1.0, 1.0));
  glLightfv(GL_LIGHT3, GL_SPECULAR,              Vector4f(1.0, 1.0, 1.0, 1.0));
  glLightfv(GL_LIGHT3, GL_POSITION,              Vector4f(0.0, 1.75, -1.0, 1.0));
  glLightfv(GL_LIGHT3, GL_SPOT_DIRECTION,        Vector3f(0.0, -1.0, 0.0));
  glLightf( GL_LIGHT3, GL_SPOT_EXPONENT,         1.0);                          // accepts 0-128
  glLightf( GL_LIGHT3, GL_SPOT_CUTOFF,           90.0);                         // width of angle of spotlight, 180 = all-round
  glLightf( GL_LIGHT3, GL_CONSTANT_ATTENUATION,  0.0);
  glLightf( GL_LIGHT3, GL_LINEAR_ATTENUATION,    0.0001);
  glLightf( GL_LIGHT3, GL_QUADRATIC_ATTENUATION, 1.0 / 0.7);                    // effective brightness
  glEnable( GL_LIGHT3);

  // render the cabin walls - basic octagonal cabin
  //glColor4d(0.8, 0.8, 0.8, 1.0);
  //glMaterialfv(GL_FRONT, GL_AMBIENT,  Vector4f(1.0, 1.0, 1.0, 1.0));
  //glMaterialfv(GL_FRONT, GL_DIFFUSE,  Vector4f(1.0, 1.0, 1.0, 1.0));
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, Vector4f(0.4, 0.5, 0.4, 1.0));
  glMaterialfv(GL_FRONT, GL_SPECULAR,            Vector4f(0.5, 0.5, 0.5, 1.0));
  glMaterialfv(GL_FRONT, GL_EMISSION,            Vector4f(0.0, 0.0, 0.0, 1.0));
  glMaterialf(GL_FRONT,  GL_SHININESS,           2.0);                          // 0 to 127

  glBegin(GL_QUADS);
  // floor
  glNormal3d( 0.0,  1.0,  0.0);

  glVertex3d(-2.0,  0.0,  1.0);                                                 // middle
  glVertex3d( 2.0,  0.0,  1.0);
  glVertex3d( 2.0,  0.0, -1.0);
  glVertex3d(-2.0,  0.0, -1.0);

  glVertex3d(-2.0,  0.0, -1.0);                                                 // front
  glVertex3d( 2.0,  0.0, -1.0);
  glVertex3d( 1.0,  0.0, -2.0);
  glVertex3d(-1.0,  0.0, -2.0);

  glVertex3d(-1.0,  0.0,  2.0);                                                 // back
  glVertex3d( 1.0,  0.0,  2.0);
  glVertex3d( 2.0,  0.0,  1.0);
  glVertex3d(-2.0,  0.0,  1.0);

  // ceiling
  glNormal3d( 0.0, -1.0,  0.0);

  glVertex3d(-2.0,  2.0, -1.0);                                                 // middle
  glVertex3d( 2.0,  2.0, -1.0);
  glVertex3d( 2.0,  2.0,  1.0);
  glVertex3d(-2.0,  2.0,  1.0);

  glVertex3d(-1.0,  2.0, -2.0);                                                 // front
  glVertex3d( 1.0,  2.0, -2.0);
  glVertex3d( 2.0,  2.0, -1.0);
  glVertex3d(-2.0,  2.0, -1.0);

  glVertex3d(-2.0,  2.0,  1.0);                                                 // back
  glVertex3d( 2.0,  2.0,  1.0);
  glVertex3d( 1.0,  2.0,  2.0);
  glVertex3d(-1.0,  2.0,  2.0);

  // walls
  double const xsize = 2.0;
  double const ysize = 2.0;
  double const qsize = 1.0;
  double const xstep = xsize / 20;
  double const ystep = ysize / 20;
  double const qstep = qsize / 12;
  glNormal3d( 0.0,  0.0,  1.0);                                                 // north
  for(double x = 0.0; x < xsize; x += xstep) {
    for(double y = 0.0; y < ysize; y += ystep) {
      glVertex3d(-1.0 + x,         y,         -2.0);
      glVertex3d(-1.0 + x + xstep, y,         -2.0);
      glVertex3d(-1.0 + x + xstep, y + ystep, -2.0);
      glVertex3d(-1.0 + x,         y + ystep, -2.0);
    }
  }
  glNormal3dv(Vector3d(-1.0, 0.0, 1.0).normalise_copy());                       // north-east
  for(double q = 0.0; q < qsize; q += qstep) {
    for(double y = 0.0; y < ysize; y += ystep) {
      glVertex3d(1.0 + q        , y,         -2.0 + q        );
      glVertex3d(1.0 + q + qstep, y,         -2.0 + q + qstep);
      glVertex3d(1.0 + q + qstep, y + ystep, -2.0 + q + qstep);
      glVertex3d(1.0 + q        , y + ystep, -2.0 + q        );
    }
  }
  glNormal3d(-1.0,  0.0,  0.0);                                                 // east
  for(double x = 0.0; x < xsize; x += xstep) {
    for(double y = 0.0; y < ysize; y += ystep) {
      glVertex3d(2.0, y,         -1.0 + x        );
      glVertex3d(2.0, y,         -1.0 + x + xstep);
      glVertex3d(2.0, y + ystep, -1.0 + x + xstep);
      glVertex3d(2.0, y + ystep, -1.0 + x        );
    }
  }
  glNormal3dv(Vector3d(-1.0, 0.0, -1.0).normalise_copy());                      // south-east
  for(double q = 0.0; q < qsize; q += qstep) {
    for(double y = 0.0; y < ysize; y += ystep) {
      glVertex3d(2.0 - (q        ), y,         1.0 + q        );
      glVertex3d(2.0 - (q + qstep), y,         1.0 + q + qstep);
      glVertex3d(2.0 - (q + qstep), y + ystep, 1.0 + q + qstep);
      glVertex3d(2.0 - (q        ), y + ystep, 1.0 + q        );
    }
  }
  glNormal3d( 0.0,  0.0, -1.0);                                                 // south
  for(double x = 0.0; x < xsize; x += xstep) {
    for(double y = 0.0; y < ysize; y += ystep) {
      glVertex3d(-1.0 + x + xstep, y,         2.0);
      glVertex3d(-1.0 + x,         y,         2.0);
      glVertex3d(-1.0 + x,         y + ystep, 2.0);
      glVertex3d(-1.0 + x + xstep, y + ystep, 2.0);
    }
  }

  glNormal3dv(Vector3d(1.0, 0.0, -1.0).normalise_copy());                       // south-west
  for(double q = 0.0; q < qsize; q += qstep) {
    for(double y = 0.0; y < ysize; y += ystep) {
      glVertex3d(-1.0 - (q        ), y,         2.0 - (q        ));
      glVertex3d(-1.0 - (q + qstep), y,         2.0 - (q + qstep));
      glVertex3d(-1.0 - (q + qstep), y + ystep, 2.0 - (q + qstep));
      glVertex3d(-1.0 - (q        ), y + ystep, 2.0 - (q        ));
    }
  }
  glNormal3d( 1.0,  0.0,  0.0);                                                 // west
  for(double x = 0.0; x < xsize; x += xstep) {
    for(double y = 0.0; y < ysize; y += ystep) {
      glVertex3d(-2.0, y,         -1.0 + x + xstep);
      glVertex3d(-2.0, y,         -1.0 + x        );
      glVertex3d(-2.0, y + ystep, -1.0 + x        );
      glVertex3d(-2.0, y + ystep, -1.0 + x + xstep);
    }
  }
  glNormal3dv(Vector3d(1.0, 0.0, 1.0).normalise_copy());                        // north-west
  for(double q = 0.0; q < qsize; q += qstep) {
    for(double y = 0.0; y < ysize; y += ystep) {
      glVertex3d(-1.0 - (q + qstep), y,         -2.0 + q + qstep);
      glVertex3d(-1.0 - (q        ), y,         -2.0 + q        );
      glVertex3d(-1.0 - (q        ), y + ystep, -2.0 + q        );
      glVertex3d(-1.0 - (q + qstep), y + ystep, -2.0 + q + qstep);
    }
  }
  glEnd();

  // cycle through the instrument panels and render them
  for(auto const &it : panels) {
    it->render();
  }
  for(auto const &it : devices_cabin) {
    it->render();
  }
  glPopMatrix();
}
