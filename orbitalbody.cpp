#include "orbitalbody.h"

orbitalbody::orbitalbody()
  : orbit(),
    gm(0.0) {
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

void orbitalbody::update_state(double time, double deltatime) {
  /// Re-calculate current velocity and position based on orbital data
  //std::cout << "DEBUG: called update_state on orbitalbody " << name << std::endl;
  //position = ;
  //velocity = ;
  position += velocity * deltatime;
  // TODO
}

std::string orbitalbody::get_description() {
  if(description.size() != 0) {
    return description;
  } else {
    return "An orbiting body of some kind, perhaps an asteroid, meteorite or planetoid.";
  }
}

void orbitalbody::set_mass(double newmass) {
  set_mass(newmass);
  update_gm();
}

void orbitalbody::update_gm() {
  // re-cache the standard gravitational parameter
  gm = (gravitational_constant * get_mass());
}

double orbitalbody::get_true_anomaly(double time) {
  /// Return the true anomaly based on mean anomaly at epoch and the current time
  //double v = acos()
  // TODO
}

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
  return 2 * M_PI * sqrt(pow(orbit.a, 3) / (gm));
}
