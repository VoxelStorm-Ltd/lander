#ifndef PLANET_H_INCLUDED
#define PLANET_H_INCLUDED

#include "orbitalbody.h"

class planet : public orbitalbody {
  /// An astronomical object large enough to be rounded by its own gravity but not to undergo fusion
public:
  planet();
  ~planet();

  virtual double get_mass();
  virtual double get_radius();
};

#endif // PLANET_H_INCLUDED
