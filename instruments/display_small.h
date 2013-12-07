#ifndef DISPLAY_SMALL_H_INCLUDED
#define DISPLAY_SMALL_H_INCLUDED

#include <string>
#include "display.h"

class display_small : public display {
  /// Specialisation of display - small screen version
public:
  display_small();
  ~display_small();

  std::string get_name();
  std::string get_model();
  std::string get_description();
  double      get_mass();
  Vector3d    get_size();
};

#endif // DISPLAY_SMALL_H_INCLUDED
