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
public:
  physicsbody();
  ~physicsbody();

  virtual Vector3d get_acceleration(Vector3d const &position, Vector3d const &velocity, double time);
  void update_state(double t, double dt);
};

#endif // PHYSICSBODY_H_INCLUDED
