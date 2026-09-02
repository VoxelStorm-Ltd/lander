#ifndef DISPLAY_SMALL_H_INCLUDED
#define DISPLAY_SMALL_H_INCLUDED

#include <string>
#include "display.h"

class display_small : public display {
  /// Specialisation of display - small screen version
public:
  display_small();
  ~display_small();

  std::string get_name() override final;
  std::string get_model() override final;
  std::string get_description() override final;
  double      get_mass() override final;
  vector3d    get_size() override final;
};

#endif // DISPLAY_SMALL_H_INCLUDED
