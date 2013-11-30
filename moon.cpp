#include "moon.h"

moon::moon() {
  /// Default constructor
}

moon::~moon() {
  /// Default destructor
}


double moon::get_mass() {
  if(mass != 0.0) {
    return mass;
  } else {
    // no mass assigned, take a potshot at one - in the scale of solar satellite moons
    // range by solar system standards: http://en.wikipedia.org/wiki/Planetary_mass
    random_reset();
    return get_random_double(1480000000000000.0, 148190000000000000000000.0);  // from deimos to ganymede
  }
}
double moon::get_radius() {
  if(radius != 0.0) {
    return radius;
  } else {
    // make up a radius for this based on what's likely for the mass
    // get a random density for the range
    random_reset();
    double const density = get_random_double(1834.4, 3528.0);    // from callisto to io
    double const volume = get_mass() * (1.0 / density);
    return pow(volume / ((4.0 / 3.0) * M_PI), 1.0 / 3.0);       // radius from volume of sphere
  }
}
