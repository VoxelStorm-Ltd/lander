#include "astronaut.h"

astronaut::astronaut()
  : state(statetype::INACTIVE),
    vessel_in(nullptr),
    walking_on(nullptr) {
  /// Default constructor
  set_mass(0.0);         // average weight 70kg + spacesuit 10kg
  set_radius(0.0);      // average height * average centre of gravity ratio
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
    // take a guess at radius (aka half height) based on human mass
    // assume spacesuit weighs 10kg
    double const nakedmass = get_mass() - 10.0;
    // adjust range 60kg to 1.65m and 90kg to 1.84m
    double const height = (((nakedmass - 60.0) / (90.0 - 60.0) * (1.84 - 1.65)) + 1.65);
    return height / 2;
  }
}
