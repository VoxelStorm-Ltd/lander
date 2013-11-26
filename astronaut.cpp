#include "astronaut.h"

astronaut::astronaut()
  : state(statetype::INACTIVE),
    vessel_in(nullptr),
    walking_on(nullptr) {
  /// Default constructor
  set_mass(80.0);         // average weight 70kg + spacesuit 10kg
  set_radius(0.963);      // average height * average centre of gravity ratio
}

astronaut::~astronaut() {
  /// Default destructor
}
