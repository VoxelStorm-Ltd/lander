#include "corner.h"

corner::corner() {
  /// Default constructor
}

corner::corner(double x, double y, double z)
  : coords(x, y, z) {
  /// Specific constructor
}

corner::~corner() {
  /// Default destructor
}

void corner::set_coords(double x, double y, double z) {
  /// Update the coords
  coords.x = x;
  coords.y = y;
  coords.z = z;
}
