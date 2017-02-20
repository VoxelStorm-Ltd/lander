#ifndef DISPLAY_DIGITAL_H_INCLUDED
#define DISPLAY_DIGITAL_H_INCLUDED

#include <string>
#include "display.h"

class display_digital : public display {
  /// Digital specialisation of the display
public:
  display_digital();
  ~display_digital();

  std::string get_name() override final;
  std::string get_model() override final;
  std::string get_description() override final;
  double      get_mass() override final;
  void update() override final;
  void render() override final;
};

#endif // DISPLAY_DIGITAL_H_INCLUDED
