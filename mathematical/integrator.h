#ifndef INTEGRATOR_H_INCLUDED
#define INTEGRATOR_H_INCLUDED

#include "vectorstorm/vectorstorm.h"

class integrator {
protected:
  integrator();                                                                 // this base class is not to be instantiated
public:
  struct integrator_derivative {
    vector3d dx;                                                                // derivative of position: velocity
    vector3d dv;                                                                // derivative of velocity: acceleration
  };

  virtual ~integrator();

  virtual vector3d get_acceleration(vector3d const &position, vector3d const &velocity, double time);
  virtual void integrate(vector3d &position, vector3d &velocity, double time, double deltatime);
};

#endif // INTEGRATOR_H_INCLUDED
