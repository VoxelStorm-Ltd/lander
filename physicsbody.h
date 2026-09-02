#ifndef PHYSICSBODY_H_INCLUDED
#define PHYSICSBODY_H_INCLUDED

#include "body.h"
//#include "integrator_euler.h"
#include "integrator_rungekutta4.h"
//#include "integrator_rungekuttafehlberg54.h"

//class physicsbody : public body, public integrator_euler {
class physicsbody : public body, public integrator_rungekutta4 {
//class physicsbody : public body, public integrator_rungekuttafehlberg54 {
  /// A body that follows integrated n-body physics
protected:
  physicsbody();                                                                // prevent this class being instantiated directly
public:
  virtual ~physicsbody();

  virtual vector3d get_acceleration(vector3d const &position, vector3d const &velocity, double time);
  virtual void update_state(double t, double dt);
};

#endif // PHYSICSBODY_H_INCLUDED
