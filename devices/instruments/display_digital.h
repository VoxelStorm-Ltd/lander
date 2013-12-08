#ifndef DISPLAY_DIGITAL_H_INCLUDED
#define DISPLAY_DIGITAL_H_INCLUDED

#include <string>
#include "display.h"

class display_digital : public display {
  /// Digital specialisation of the display
public:
  display_digital();
  ~display_digital();

  std::string get_name();
  std::string get_model();
  std::string get_description();
  double      get_mass();
  void update();
};

#endif // DISPLAY_DIGITAL_H_INCLUDED
