#ifndef MOON_H_INCLUDED
#define MOON_H_INCLUDED

#include "planet.h"

class moon : public planet {
  /// A natural satellite of another planet
public:
  moon();
  virtual ~moon();

  // getters and setters
  virtual std::string get_type();
  virtual double get_mass();
  virtual double get_radius();
};

#endif // MOON_H_INCLUDED
