#include "orbitalbody.h"

orbitalbody::orbitalbody()
  : orbit(),
    parent(nullptr) {
  /// Default constructor
  orbit.a = 0.0;
  orbit.e = 0.0;
  orbit.i = 0.0;
  orbit.o = 0.0;
  orbit.w = 0.0;
  orbit.m = 0.0;
}

orbitalbody::~orbitalbody() {
  /// Default destructor
}

std::string orbitalbody::get_type() {
  /// Return the type of object this is
  return "orbital body";
}

std::string orbitalbody::get_description() {
  /// Return the description of this body
  if(description.size() != 0) {
    return description;
  } else {
    return "An orbiting body of some kind, perhaps an asteroid, meteorite or planetoid.";
  }
}

bool orbitalbody::check_within_physical_influence(double thisradius) {
  /// Check if we're within range for physical interaction (atmosphere / collision)
  if(thisradius <= get_radius()) {
    // assume a rocky body with no atmosphere by default
    return true;
  } else {
    return false;
  }
}

double orbitalbody::get_atmos_pressure(double thisradius __attribute__((__unused__))) {
  /// Return the atmospheric pressure at this radius from the body
  return 0.0;                                                                   // vacuum
}

double orbitalbody::get_atmos_temperature(double thisradius __attribute__((__unused__))) {
  /// Return the atmospheric temperature at this radius from the body
  return 0.0;                                                                   // vacuum
}

void orbitalbody::update_state(double time, double deltatime) {
  /// Re-calculate current velocity and position based on orbital data
  //std::cout << "DEBUG: called update_state on orbitalbody " << name << std::endl;
  // reference direction is +z
  //vector3d ascending_node(0.0, 1.0, 0.0);
  //ascending_node.rotate(0.0, orbit.longitude_asc_node, 0.0);
  // ...

  // placeholder simple circular orbits, zero eccentricity only
  double finaltime = time + deltatime;

  vector3d orbitposition;
  orbitposition.z = orbit.semimajor_axis;
  double orbitangle = ((M_PI * 2.0) / get_orbital_period() * finaltime) + orbit.mean_anomaly_epoch;
  orbitangle = fmod(orbitangle, M_PI * 2.0);
  orbitposition.rotate_rad(0.0, orbitangle, 0.0);

  vector3d oldposition(position);
  position = parent->position + orbitposition;
  velocity = (position - oldposition) / deltatime;
  //std::cout << "Pos: " << position << " vel " << velocity << std::endl;

  rotation = rotation * (spin * deltatime);
}

double orbitalbody::get_mean_anomaly(double time) {
  /// Return the true anomaly based on mean anomaly at epoch and the current time
  // M = n * d + L - p
  // needs to be clamped to 360
  //double mean_anomaly = fmod(((daily_motion * time) + get_mean_longitude() - get_longitude_periapsis()), 360.0);
  //if(mean_anomaly < 0) {
  //  mean_anomaly = 360.0 - mean_anomaly;
  //}

  // M = E - (e * sin(E))
  //double const mean_anomaly = eccentric_anomaly - (orbit.eccentricity * sin(eccentric_anomaly));

  // see http://ccar.colorado.edu/ASEN5070/primers/kep2cart_2000/kep2cart.htm
  // n = sqrt(u / (a^3))
  // M = n(t - T)
  if(parent) {
    // mean_anomaly = sqrt(parent.gm / pow(semimajor_axis, 3)) * time;
  } else {
    return 0.0;
  }
}

double orbitalbody::get_eccentric_anomaly(double time) {
  //// Calculate the eccentric anomaly based on semi-major axis and a point's coords
  // cos(E) = (x / a)
  // sin(E) = (y / b)

  // this is the bit they do iteratively...
  // mean_anomaly = eccentric_anomaly - orbit.eccentricity * sin(eccentric_anomaly);
  ///double eccentric_anomaly = get_mean_anomaly(time) + (orbit.eccentricity * sin(eccentric_anomaly));
}

double orbitalbody::get_true_anomaly(double time) {
  /// Return the true anomaly based on mean anomaly at epoch and the current time
  //double v = acos()
  // tan(true_anomaly / 2) = ((1 + orbit.eccentricity) / (1 - orbit.eccentricity))^(1/2) * tan(get_eccentric_anomaly(time) / 2)
  // from http://www.stargazing.net/kepler/kepler.html:
  ///double const s1 = sin(m);
  ///double const s2 = sin(m * 2.0);
  ///double const s3 = sin(m * 3.0);
  ///double const s4 = sin(m * 4.0);
  ///double const s5 = sin(m * 5.0);
  ///double const a1 = 2.0          * s1;
  ///double const a2 = 1.25         * s2;
  ///double const a3 = 1.083333333  * s3 - 0.25         * s1;
  ///double const a4 = 1.072916667  * s4 - 0.4583333333 * s2;
  ///double const a5 = 0.0520833333 * s1 - 0.671875     * s3 + 1.142708333 * s5;
  ///return m + e * (a1 + e * (a2 + e * (a3 + e * (a4 + e * a5))));
}

double orbitalbody::get_longitude_periapsis() {
  /// Calculate ongitude of the periapsis
  return orbit.longitude_asc_node + orbit.argument_periapsis;
}

//double orbitalbody::get_mean_longitude() {
//  /// Mean longitude
//  return mean_anomaly + get_longitude_periapsis();
//}

double orbitalbody::get_periapsis() {
  /// Calculate the periapsis distance
  return orbit.a * (1 - orbit.e);
}

double orbitalbody::get_apoapsis() {
  /// Calculate the apoapsis distance
  return orbit.a * (1 + orbit.e);
}

double orbitalbody::get_orbital_period() {
  /// Calculate orbital period
  if(parent) {
    return 2.0 * M_PI * sqrt(pow(orbit.semimajor_axis, 3) / (parent->gm));
  } else {
    return 0.0;
  }
}

//double orbitalbody::get_mean_motion() {
//  /// Hyperbolic mean motion in radians per second
//  // u = sqrt(GM/(-a^3))
//  if(parent) {
//    return sqrt(parent->gm / pow(-orbit.a, 3.0));
//  } else {
//    return 0.0;
//  }
//}


// TODO: http://www.stargazing.net/kepler/ellipse.html
//       http://www.bruce-shapiro.com/pair/ElementConversionRecipes.pdf
//       http://ccar.colorado.edu/ASEN5070/primers/kep2cart_2000/kep2cart.htm
//       http://www.elsevierdirect.com/companions/9780126836301/appendices/Chapter_2_-_Kepler_Orbit_Elements_to_Eci_Cartesian_Coordinates_Conversion.pdf
// cartesian to kepler:
//       http://downloads.rene-schwarz.com/download/M001-Keplerian_Orbit_Elements_to_Cartesian_State_Vectors.pdf
