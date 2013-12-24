#ifndef MEMORY_H_INCLUDED
#define MEMORY_H_INCLUDED

#include "instrument.h"

class memory : public instrument {
  /// Memory chip capable of remembering the last inputs of each kind it got
private:
  // stored values
  double memory_value;
  GLuint memory_image;
  std::string memory_text;

  bool loop_safe;          // to detect circular references and stick with cached values

public:
  memory();
  ~memory();

  std::string  get_name();
  std::string  get_model();
  std::string  get_description();
  double       get_mass();
  unsigned int get_port_in_count();
  std::string  get_port_in_name(           unsigned int port);
  std::string  get_port_in_description(    unsigned int port);
  bool         get_port_in_required(       unsigned int port);
  unsigned int get_port_out_count();
  std::string  get_port_out_name(          unsigned int port);
  std::string  get_port_out_description(   unsigned int port);
  double       get_port_out_data(          unsigned int port);
  std::string  get_port_out_text(          unsigned int port);
  GLuint       get_port_out_video_digital( unsigned int port);
  GLuint       get_port_out_video_analogue(unsigned int port);
  void         get_port_out_sound(         unsigned int port);
  Vector3d     get_size();
  void update();

  // memory-chip specific setters:
  void set_memory_value(double newvalue);
  void set_memory_text(std::string const &newstring);
};

#endif // MEMORY_H_INCLUDED
