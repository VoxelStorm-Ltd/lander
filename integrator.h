#ifndef INTEGRATOR_H_INCLUDED
#define INTEGRATOR_H_INCLUDED

#include "vmath.h"

class integrator {
public:
  struct integrator_derivative {
    Vector3d dx;                             // derivative of position: velocity
    Vector3d dv;                             // derivative of velocity: acceleration
  };

  integrator();
  ~integrator();

  // TODO: turn this to Vector3d
  virtual Vector3d get_acceleration(Vector3d const &position, Vector3d const &velocity, double time);
  virtual void integrate(Vector3d &position, Vector3d &velocity, double time, double deltatime);
};

#endif // INTEGRATOR_H_INCLUDED
