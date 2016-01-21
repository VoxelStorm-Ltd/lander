#include "integrator_rungekuttafehlberg54.h"

integrator_rungekuttafehlberg54::integrator_rungekuttafehlberg54() {
  /// Default constructor
}

integrator_rungekuttafehlberg54::~integrator_rungekuttafehlberg54() {
  /// Default destructor
}

void integrator_rungekuttafehlberg54::integrate(Vector3d &thisposition,
                                                Vector3d &thisvelocity,
                                                double time,
                                                double deltatime) {
  /// adapted from http://www.ma.utexas.edu/CNA/cheney-kincaid/Ccode/CHP10/rk45.c
  /// also see http://people.sc.fsu.edu/~jburkardt/cpp_src/rkf45/rkf45.html
  double const finaltime = time + deltatime;

  // void rk45(float f(float, float), float *t, float *x, float deltatime, float *epsilon)
  Vector3d const temp1 = get_acceleration(thisposition,
                                          thisvelocity,                   finaltime) * deltatime;
  Vector3d const temp2 = get_acceleration(thisposition + temp1 * c21,
                                          thisvelocity + c20 * deltatime, finaltime) * deltatime;
  Vector3d const temp3 = get_acceleration(thisposition + temp1 * c31
                                                       + temp2 * c32,
                                          thisvelocity + c30 * deltatime, finaltime) * deltatime;
  Vector3d const temp4 = get_acceleration(thisposition + temp1 * c41
                                                       + temp2 * c42
                                                       + temp3 * c43,
                                          thisvelocity + c40 * deltatime, finaltime) * deltatime;
  Vector3d const temp5 = get_acceleration(thisposition + temp1 * c51
                                                       + temp2 * c52
                                                       + temp3 * c53
                                                       + temp4 * c54,
                                          thisvelocity + c50 * deltatime, finaltime) * deltatime;
  Vector3d const temp6 = get_acceleration(thisposition + temp1 * c61
                                                       + temp2 * c62
                                                       + temp3 * c63
                                                       + temp4 * c64
                                                       + temp5 * c65,
                                          thisvelocity + c60 * deltatime, finaltime) * deltatime;

  Vector3d x4 = thisposition + (temp1 * a1) + (temp3 * a3) + (temp4 * a4) + (temp5 * a5);
  Vector3d dx = (temp1 * b1) + (temp3 * b3) + (temp4 * b4) + (temp5 * b5) + (temp6 * b6);
  // calculate the error in 3 dimensions
  Vector3d epsilon(fabs(thisposition.x - x4.x),
                   fabs(thisposition.y - x4.y),
                   fabs(thisposition.z - x4.z));

  thisposition += dx;
  /// NOT COMPLETE - needs to get velocity term properly
  //thisvelocity += get_acceleration(thisposition, thisvelocity, finaltime);
}

