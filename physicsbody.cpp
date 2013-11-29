#include "physicsbody.h"
#include "universe.h"
#include "starsystem.h"

universe extern root;

physicsbody::physicsbody() {
  /// Default constructor
}

physicsbody::~physicsbody() {
  /// Default destructor
}

Vector3d physicsbody::get_acceleration(Vector3d const &thisposition,
                                       Vector3d const &thisvelocity __attribute__((__unused__)),
                                       double time __attribute__((__unused__))) {
  /// placeholder acceleration due to gravity
  //std::cout << "      DEBUG: get_acceleration before: pos " << thisposition << std::endl;
  //return get_gravity_accel_v3(thisposition);
  Vector3d acceleration;
  // iterate through every sufficiently significant body
  for(auto *it : root.currentsystem->bodies) {
    if(it == this) {
      // don't calculate gravitational effect of ourselves
      continue;
    }
    acceleration += it->get_gravity_accel_v3(thisposition);
    //std::cout << "DEBUG: acceleration after " << it->name << " is " << acceleration << std::endl;
  }
  //std::cout << "      DEBUG: get_acceleration after:  acc " << acceleration << std::endl;
  return acceleration;
}

void physicsbody::update_state(double time, double deltatime) {
  //std::cout << "DEBUG: physics_step before: pos " << position << " vel " << velocity << std::endl;
  //std::cout << "DEBUG: called update_state on physicsbody " << name << std::endl;
  integrate(position, velocity, time, deltatime);
  //std::cout << "DEBUG: physics_step after:  pos " << position << " vel " << velocity << std::endl;
}
