#include "spacecraft.h"
#include <boost/units/systems/si/velocity.hpp>
#include <boost/units/base_units/imperial/mile.hpp>
#include <boost/units/base_units/metric/hour.hpp>
#include "vmath.h"
#include "universe.h"
#include "starsystem.h"

extern universe root;

spacecraft::spacecraft()
  : temperature_hull(285.18),     // http://www.learnthermo.com/examples/ch04/p-4b-3.php
    temperature_cabin(21.0 + 273.15) {
  /// Default constructor
}

spacecraft::~spacecraft() {
  /// Default destructor
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
        exit(0);
      }
    }
  }
}
