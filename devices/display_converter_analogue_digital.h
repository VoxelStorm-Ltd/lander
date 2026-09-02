#ifndef DISPLAY_CONVERTER_ANALOGUE_DIGITAL_H_INCLUDED
#define DISPLAY_CONVERTER_ANALOGUE_DIGITAL_H_INCLUDED

#include "instrument.h"

class display_converter_analogue_digital : public device {
public:
  display_converter_analogue_digital();
  ~display_converter_analogue_digital();

  std::string  get_name();
  std::string  get_model();
  std::string  get_description();
  double       get_mass();
  vector3d     get_size();
  unsigned int get_port_in_count();
  std::string  get_port_in_name(          unsigned int port);
  std::string  get_port_in_description(   unsigned int port);
  bool         get_port_in_required(      unsigned int port);
  unsigned int get_port_out_count();
  std::string  get_port_out_name(         unsigned int port);
  std::string  get_port_out_description(  unsigned int port);
  GLuint       get_port_out_video_digital(unsigned int port);
};

#endif // DISPLAY_CONVERTER_ANALOGUE_DIGITAL_H_INCLUDED
