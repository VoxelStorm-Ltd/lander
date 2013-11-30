#include "planet.h"

planet::planet() {
  /// Default constructor
}

planet::~planet() {
  /// Default destructor
}

double planet::get_mass() {
  if(mass != 0.0) {
    return mass;
  } else {
    // no mass assigned, take a potshot at one - in the terrestrial planetary scale
    // range by solar system standards: http://en.wikipedia.org/wiki/Planetary_mass
    random_reset();
    return get_random_double(300000000000000000000000.0, 2000000000000000000000000000.0);  // from mercury to jupiter
  }
}
double planet::get_radius() {
  if(radius != 0.0) {
    return radius;
  } else {
    // make up a radius for this based on what's likely for the mass
    // get a random density for the range
    random_reset();
    double const density = get_random_double(620.0, 5430.0);    // from saturn to mercury
    double const volume = get_mass() * (1.0 / density);
    return pow(volume / ((4.0 / 3.0) * M_PI), 1.0 / 3.0);       // radius from volume of sphere
  }
}
