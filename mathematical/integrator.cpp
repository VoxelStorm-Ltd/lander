#include "integrator.h"
//#include <iostream>

integrator::integrator() {
  /// Default constructor
}

integrator::~integrator() {
  /// Default destructor
}

Vector3d integrator::get_acceleration(Vector3d const &position __attribute__((__unused__)),
                                      Vector3d const &velocity __attribute__((__unused__)),
                                      double t                 __attribute__((__unused__))) {
  /// virtual function to be specialised by accelerating objects
  std::cout << "WARNING: virtual function " << __PRETTY_FUNCTION__ << " called without specialisation - this should never happen." << std::endl;
  return Vector3d(0, 0, 0);
}

void integrator::integrate(Vector3d &position __attribute__((__unused__)),
                           Vector3d &velocity __attribute__((__unused__)),
                           double t           __attribute__((__unused__)),
                           double dt          __attribute__((__unused__))) {
  /// virtual function to be specialised by integrators
  std::cout << "WARNING: virtual function " << __PRETTY_FUNCTION__ << " called without specialisation - this should never happen." << std::endl;
}
