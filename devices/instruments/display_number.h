#ifndef DISPLAY_NUMBER_H_INCLUDED
#define DISPLAY_NUMBER_H_INCLUDED

#include "instrument.h"

class display_number : public instrument {
  /// Seven segment style numerical display capable of floating point
private:
  std::string displaycontent;

  //double const labellength = font_7segment3d->Advance("8.8.8.8.8.8.8.8.", 16);
  static double constexpr sizex = 0.100;
  static double constexpr displaylength = 74;    // hard-coded result of above calculation
  static double constexpr displayscale = (sizex - 0.004) / displaylength;                // automatic scaling to fit in the space

public:
  display_number();
  ~display_number();

  std::string  get_name();
  std::string  get_model();
  std::string  get_description();
  double       get_mass();
  Vector3d     get_size();
  unsigned int get_port_in_count();
  std::string  get_port_in_name(       unsigned int port);
  std::string  get_port_in_description(unsigned int port);
  bool         get_port_in_required(   unsigned int port);
  void update();
  void update_if_time();
  void render();

  std::string trimstring(std::string const &oldstring);
  std::string trimstring(double input);
};

#endif // DISPLAY_NUMBER_H_INCLUDED
