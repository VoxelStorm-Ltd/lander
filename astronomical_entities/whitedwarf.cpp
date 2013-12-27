#include "whitedwarf.h"

whitedwarf::whitedwarf() {
  /// Default constructor
  description = "A stellar remnant composed mostly of electron-degenerate matter";
}

whitedwarf::~whitedwarf() {
  /// Default destructor
}

std::string whitedwarf::get_type() {
  /// Return the type of object this is
  return "white dwarf";
}
