#include "spacecraft.h"
#include <boost/units/systems/si/velocity.hpp>
#include <boost/units/base_units/imperial/mile.hpp>
#include <boost/units/base_units/metric/hour.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "vmath.h"
#include "universe.h"
#include "starsystem.h"
#include "device.h"
#include "astronaut.h"
#include "instrumentpanel.h"
#include "dustcloud.h"

extern universe root;

spacecraft::spacecraft()
  : temperature_hull(285.18),     // http://www.learnthermo.com/examples/ch04/p-4b-3.php
    temperature_cabin(21.0 + 273.15) {
  /// Default constructor
}

spacecraft::~spacecraft() {
  /// Default destructor
  for(auto &it : panels) {
    delete it;
    it = nullptr;
  }
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
}

std::string spacecraft::get_name() {
  if(name.size() != 0) {
    return name;
  } else {
    // random ship name
    random_reset();                                 // reset the generator to its seed
    std::stringstream randomname;
    randomname << get_random_name_ancient() << " Mk" << get_random_uint(1, 5);
    return randomname.str();
  }
}

std::string spacecraft::get_designation() {
  if(designation.size() != 0) {
    return designation;
  } else {
    // generate a random elite-style ship registration or star trek NCC-1701
    random_reset();                                 // reset the generator to its seed
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
    return get_random_double(15000000.0, 45000000.0);  // averaging 650 tons +- 50%
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
    return pow(volume / ((4.0 / 3.0) * M_PI), 1.0 / 3.0);     // radius from volume of sphere
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

void spacecraft::update_state(double time, double deltatime) {
  /// update all relevant state information
  // the default integrator for position and velocity
  integrate(position, velocity, time, deltatime);

  // update temperatures
  // search local star system for planets and check for atmospheric interaction
  // search local star system for suns and calculate their cumulative radiation
  // take into account shadowing by planets
  //temperature_hull =
  //temperature_cabin =
  // TODO

  // check collisions
  for(auto const &it : root.currentsystem->bodies) {
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

      if(velocity_delta_mag > 22) {   // ~50mph
        std::cout << "INFO: " << get_name() << " collided with " << it->get_name() << " at a fatal " << velocity_delta_mag << "m/s" << std::endl;
        // if the ship was destroyed, give a snide message relating to the kinetic energy and collateral damage
        double const ke = 0.5 * get_mass() * velocity_delta_mag * velocity_delta_mag;
        Vector3d lastposition(position);    // cache position for after we delete "this"
        destroy();
        root.make_explosion(lastposition, ke);
      }
    }
  }
}

void spacecraft::destroy() {
  /// Blow up or otherwise annihilate a ship destructively
  std::cout << "Spacecraft " << get_name() << " has been fatally damaged." << std::endl;
  //root.currentsystem->bodies.remove(this);        // this breaks the iterator, and is always called from inside it...
  // replace it in the list with a dustcloud
  for(auto &it : root.currentsystem->bodies) {
    if(it == this) {
      spacecraft *oldship = static_cast<spacecraft*>(it);   // we know a static cast is safe since we've already confirmed it's a pointer to this object
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
  // render the cockpit walls

  // cycle through the instrument panels and render them
  for(auto const &it : panels) {
    it->render();
  }
  for(auto const &it : devices_cabin) {
    it->render();
  }
  glPopMatrix();
}
