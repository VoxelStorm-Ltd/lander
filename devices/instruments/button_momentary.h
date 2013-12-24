#ifndef BUTTON_MOMENTARY_H_INCLUDED
#define BUTTON_MOMENTARY_H_INCLUDED

#include "instrument.h"

class button_momentary : public instrument {
  /// A button that sends its on value only once (the first time it's queried) and then resets itself
private:
  bool pushed;
public:
  button_momentary();
  ~button_momentary();

  std::string  get_name();
  std::string  get_model();
  std::string  get_description();
  Vector3d     get_size();
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

  virtual void render();

  void activate();
};

#endif // BUTTON_MOMENTARY_H_INCLUDED
