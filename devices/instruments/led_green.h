#ifndef LED_GREEN_H_INCLUDED
#define LED_GREEN_H_INCLUDED

#include "instrument.h"

class led_green : public instrument {
  /// Just a simple switchable green light
public:
  led_green();
  ~led_green();

  std::string  get_name() override final;
  std::string  get_model() override final;
  std::string  get_description() override final;
  Vector3d     get_size() override final;
  double       get_mass() override final;
  unsigned int get_port_in_count() override final;
  std::string  get_port_in_name(       unsigned int port) override final;
  std::string  get_port_in_description(unsigned int port) override final;
  bool         get_port_in_required(   unsigned int port) override final;

  void render() override final;
};

#endif // LED_GREEN_H_INCLUDED
