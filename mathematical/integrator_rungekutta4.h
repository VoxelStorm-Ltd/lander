#ifndef INTEGRATOR_RUNGEKUTTA4_H_INCLUDED
#define INTEGRATOR_RUNGEKUTTA4_H_INCLUDED

#include "integrator.h"

class integrator_rungekutta4 : public integrator {
protected:
  integrator_rungekutta4();                               // this base class is not to be instantiated
public:
  virtual ~integrator_rungekutta4();

  void integrate(Vector3d &position, Vector3d &velocity, double time, double deltatime);
private:
  integrator_derivative evaluate(Vector3d const &position_initial,
                                 Vector3d const &velocity_initial,
                                 double time);
  integrator_derivative evaluate(Vector3d const &position_initial,
                                 Vector3d const &velocity_initial,
                                 double time,
                                 double deltatime,
                                 integrator_derivative const &derivative);
};

#endif // INTEGRATOR_RUNGEKUTTA4_H_INCLUDED
