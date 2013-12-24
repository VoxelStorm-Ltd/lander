#ifndef LED_GREEN_H_INCLUDED
#define LED_GREEN_H_INCLUDED

#include "instrument.h"

class led_green : public instrument {
  /// Just a simple switchable green light
public:
  led_green();
  ~led_green();

  std::string  get_name();
  std::string  get_model();
  std::string  get_description();
  Vector3d     get_size();
  double       get_mass();
  unsigned int get_port_in_count();
  std::string  get_port_in_name(       unsigned int port);
  std::string  get_port_in_description(unsigned int port);
  bool         get_port_in_required(   unsigned int port);

  virtual void render();
};

#endif // LED_GREEN_H_INCLUDED
