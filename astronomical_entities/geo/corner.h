#ifndef CORNER_H_INCLUDED
#define CORNER_H_INCLUDED

#include "vmath.h"

class corner {
public:
  Vector3d coords;

  corner();
  corner(double x, double y, double z);
  ~corner();

  void set_coords(double x, double y, double z);
};

#endif // CORNER_H_INCLUDED
