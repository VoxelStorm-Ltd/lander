#ifndef DISPLAY_CONVERTER_DIGITAL_ANALOGUE_H_INCLUDED
#define DISPLAY_CONVERTER_DIGITAL_ANALOGUE_H_INCLUDED

#include "instrument.h"

class display_converter_digital_analogue : public device {
public:
  display_converter_digital_analogue();
  ~display_converter_digital_analogue();

  std::string  get_name() override final;
  std::string  get_model() override final;
  std::string  get_description() override final;
  double       get_mass() override final;
  vector3d     get_size() override final;
  unsigned int get_port_in_count() override final;
  std::string  get_port_in_name(           unsigned int port) override final;
  std::string  get_port_in_description(    unsigned int port) override final;
  bool         get_port_in_required(       unsigned int port) override final;
  unsigned int get_port_out_count() override final;
  std::string  get_port_out_name(          unsigned int port) override final;
  std::string  get_port_out_description(   unsigned int port) override final;
  GLuint       get_port_out_video_analogue(unsigned int port) override final;
};

#endif // DISPLAY_CONVERTER_DIGITAL_ANALOGUE_H_INCLUDED
