#include "integrator_rungekutta4.h"

integrator_rungekutta4::integrator_rungekutta4() {
  /// Default constructor
}

integrator_rungekutta4::~integrator_rungekutta4() {
  /// Default destructor
}

void integrator_rungekutta4::integrate(Vector3d &thisposition,
                                       Vector3d &thisvelocity,
                                       double time,
                                       double deltatime) {
  /// see http://gafferongames.com/game-physics/integration-basics/
  //std::cout << "  DEBUG: integrate before:  pos " << thisposition << " vel " << thisvelocity << std::endl;
  integrator_derivative a = evaluate(thisposition, thisvelocity, time);
  integrator_derivative b = evaluate(thisposition, thisvelocity, time, deltatime * 0.5, a);
  integrator_derivative c = evaluate(thisposition, thisvelocity, time, deltatime * 0.5, b);
  integrator_derivative d = evaluate(thisposition, thisvelocity, time, deltatime, c);

  //Vector3d const dxdt((((a.dx + 2.0) * (b.dx + c.dx)) + d.dx) * (1.0 / 6.0));
  //Vector3d const dvdt((((a.dv + 2.0) * (b.dv + c.dv)) + d.dv) * (1.0 / 6.0));
  Vector3d const dxdt((a.dx + ((b.dx + c.dx) * 2.0) + d.dx) * (1.0 / 6.0));
  Vector3d const dvdt((a.dv + ((b.dv + c.dv) * 2.0) + d.dv) * (1.0 / 6.0));

  //std::cout << "  DEBUG: integrate during:  dxdt " << dxdt << " dvdt " << dvdt << std::endl;
  thisposition += dxdt * deltatime;
  thisvelocity += dvdt * deltatime;
  //std::cout << "  DEBUG: integrate after:   pos " << thisposition << " vel " << thisvelocity << std::endl;
}

integrator::integrator_derivative integrator_rungekutta4::evaluate(Vector3d const &position_initial,
                                                                   Vector3d const &velocity_initial,
                                                                   double time) {
  /// internal helper function called repeatedly by the integrator - initial version
  integrator_derivative output;
  output.dx = velocity_initial;
  output.dv = get_acceleration(position_initial, velocity_initial, time);
  return output;
}

integrator::integrator_derivative integrator_rungekutta4::evaluate(Vector3d const &position_initial,
                                                                   Vector3d const &velocity_initial,
                                                                   double time,
                                                                   double deltatime,
                                                                   integrator_derivative const &derivative) {
  /// internal helper function called repeatedly by the integrator
  Vector3d const position_new(position_initial + (derivative.dx * deltatime));
  Vector3d const velocity_new(velocity_initial + (derivative.dv * deltatime));

  integrator_derivative output;
  output.dx = velocity_new;
  output.dv = get_acceleration(position_new, velocity_new, time + deltatime);
  return output;
}
