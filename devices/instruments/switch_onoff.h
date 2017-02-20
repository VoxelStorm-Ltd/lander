#ifndef SWITCH_ONOFF_H_INCLUDED
#define SWITCH_ONOFF_H_INCLUDED

#include "control.h"

class switch_onoff : public control {
private:
  unsigned int setting;
public:
  switch_onoff();
  ~switch_onoff();

  std::string  get_name() override final;
  std::string  get_model() override final;
  std::string  get_description() override final;
  Vector3d     get_size() override final;
  double       get_mass() override final;
  unsigned int get_port_in_count() override final;
  std::string  get_port_in_name(           unsigned int port) override final;
  std::string  get_port_in_description(    unsigned int port) override final;
  bool         get_port_in_required(       unsigned int port) override final;
  unsigned int get_port_out_count() override final;
  std::string  get_port_out_name(          unsigned int port) override final;
  std::string  get_port_out_description(   unsigned int port) override final;
  double       get_port_out_data(          unsigned int port) override final;
  std::string  get_port_out_text(          unsigned int port) override final;
  GLuint       get_port_out_video_digital( unsigned int port) override final;
  GLuint       get_port_out_video_analogue(unsigned int port) override final;
  void         get_port_out_sound(         unsigned int port) override final;

  void render() override final;

  void activate() override final;
};

#endif // SWITCH_ONOFF_H_INCLUDED
