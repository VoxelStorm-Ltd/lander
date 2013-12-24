#ifndef ORBITALBODY_H_INCLUDED
#define ORBITALBODY_H_INCLUDED

#include "body.h"

class orbitalbody : public body {
  /// A body that follows orbital physics (using pre-defined orbits)
public:
  struct orbittype {                // orbits are have reference plane y = 0
    union {
      double a;                     // semi-major axis
      double semimajor_axis;        // = (periapsis + apoapsis) / 2
    };
    union {
      double e;                     // eccentricity: 0 = circular, 1 = pendulum
      double eccentricity;
    };
    union {
      double i;                     // inclination (in rad)
      double inclination;
    };
    union {
      double o;                     // omega - longitude of ascending node (in rad)
      double longitude_asc_node;
    };
    union {
      double w;                     // omicron - argument of periapsis (in rad)
      double argument_periapsis;
    };
    union {
      double m;                     // Mo - mean anomaly at epoch (time 0) (in rad)
      double mean_anomaly_epoch;
    };
  };
  orbittype orbit;

  body *parent;                     // what its orbit is around

  orbitalbody();
  virtual ~orbitalbody();

  // getters and setters
  virtual std::string get_description();
  virtual bool check_within_physical_influence(double thisradius);
  virtual double get_atmos_pressure(           double thisradius);
  virtual double get_atmos_temperature(        double thisradius);

  // update internal states
  void update_state(double time, double deltatime);

  // orbital functions
  double get_mean_anomaly(double time);
  double get_eccentric_anomaly(double time);
  double get_true_anomaly(double time);
  double get_longitude_periapsis();
  //double get_mean_longitude();
  double get_periapsis();
  double get_apoapsis();
  double get_orbital_period();
  //double get_mean_motion();
};

#endif // ORBITALBODY_H_INCLUDED
