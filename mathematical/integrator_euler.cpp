#include "integrator_euler.h"

integrator_euler::integrator_euler() {
  /// Default constructor
}

integrator_euler::~integrator_euler() {
  /// Default destructor
}

void integrator_euler::integrate(Vector3d &position,
                                 Vector3d &velocity,
                                 double time,
                                 double deltatime) {
  /// Very simplistic test integrator
  position += (velocity * deltatime);
  velocity += get_acceleration(position, velocity, time) * deltatime;
}
