#ifndef INTEGRATOR_EULER_H_INCLUDED
#define INTEGRATOR_EULER_H_INCLUDED

#include "integrator.h"
#include "vmath.h"

class integrator_euler : public integrator {
protected:
  integrator_euler();                                                           // this base class is not to be instantiated
public:
  virtual ~integrator_euler();

  void integrate(Vector3d &position, Vector3d &velocity, double time, double deltatime);
};

#endif // INTEGRATOR_EULER_H_INCLUDED
