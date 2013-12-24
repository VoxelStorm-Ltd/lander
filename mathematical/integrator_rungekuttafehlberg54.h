#ifndef INTEGRATOR_RUNGEKUTTAFEHLBERG54_H_INCLUDED
#define INTEGRATOR_RUNGEKUTTAFEHLBERG54_H_INCLUDED

#include "integrator.h"

class integrator_rungekuttafehlberg54 : public integrator {
protected:
  integrator_rungekuttafehlberg54();                               // this base class is not to be instantiated
public:
  virtual ~integrator_rungekuttafehlberg54();

  void integrate(Vector3d &position, Vector3d &velocity, double time, double deltatime);
private:
  // integrator constants adapted from http://www.ma.utexas.edu/CNA/cheney-kincaid/Ccode/CHP10/rk45.c
  static double constexpr a1  = 25.0 / 216.0;
  static double constexpr a2  = 0.0;
  static double constexpr a3  = 1408.0 / 2565.0;
  static double constexpr a4  = 2197.0 / 4104.0;
  static double constexpr a5  = -0.2;
  static double constexpr b1  = 16.0 / 135.0;
  static double constexpr b2  = 0.0;
  static double constexpr b3  = 6656.0 / 12825.0;
  static double constexpr b4  = 28561.0 / 56430.0;
  static double constexpr b5  = -0.1;
  static double constexpr b6  = 2.0 / 55.0;
  static double constexpr c20 = 0.25;
  static double constexpr c21 = 0.25;
  static double constexpr c30 = 0.375;
  static double constexpr c31 = 0.09375;
  static double constexpr c32 = 0.28125;
  static double constexpr c40 = 12.0 / 13.0;
  static double constexpr c41 = 1932.0 / 2197.0;
  static double constexpr c42 = -7200.0 / 2197.0;
  static double constexpr c43 = 7296.0 / 2197.0;
  static double constexpr c50 = 1.0;
  static double constexpr c51 = 439.0 / 216.0;
  static double constexpr c52 = -8.0;
  static double constexpr c53 = c51;
  static double constexpr c54 = -845.0 / 4104.0;
  static double constexpr c60 = 0.5;
  static double constexpr c61 = -8.0 / 27.0;
  static double constexpr c62 = 2.0;
  static double constexpr c63 = -3544.0 / 2565.0;
  static double constexpr c64 = 1859.0 / 4104.0;
  static double constexpr c65 = -0.275;
};

#endif // INTEGRATOR_RUNGEKUTTAFEHLBERG54_H_INCLUDED
