#ifndef ORBITALBODY_H_INCLUDED
#define ORBITALBODY_H_INCLUDED

#include "body.h"

class orbitalbody : public body {
  /// A body that follows orbital physics (using pre-defined orbits)
public:
  struct orbittype {                // orbits are have reference plane y = 0
    union {
      double a;                     // semi-major axis
      double semimajor_axis;
    };
    union {
      double e;                     // eccentricity: 0 = circular, 1 = pendulum
      double eccentricity;
    };
    union {
      double i;                     // inclination
      double inclination;
    };
    union {
      double o;                     // omega - longitude of ascending node
      double longitude_asc_node;
    };
    union {
      double w;                     // omicron - argument of periapsis
      double argument_periapsis;
    };
    union {
      double m;                     // Mo - mean anomaly at epoch (time 0)
      double mean_anomaly_epoch;
    };
  };
  orbittype orbit;
  double gm;                        // GM - standard gravitational parameter, cached

  body *parent;                     // what its orbit is around

  orbitalbody();
  ~orbitalbody();

  // getters and setters
  virtual std::string get_description();
  virtual void set_mass(double newmass);

  // update internal states
  void update_state(double time, double deltatime);
  void update_gm();

  // orbital functions
  double get_true_anomaly(double time);
  double get_periapsis();
  double get_apoapsis();
  double get_orbital_period();
};

#endif // ORBITALBODY_H_INCLUDED
