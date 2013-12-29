#ifndef TERMINAL_H_INCLUDED
#define TERMINAL_H_INCLUDED

#include <deque>
#include <string>
#include "device.h"

class terminal : public device {
  /// TTY style serial terminal, reading text from a text device and outputting to display
private:
  Vector2<unsigned int> static const windowsize;
  Vector2<unsigned int> static const windowsize_text;    // rows and columns
  double static constexpr lineheight = 10.0;
  double static constexpr margin_bottom = 16.0;
  double static constexpr margin_left   = 16.0;
  GLuint display_image;
  GLuint framebuffer;

  std::deque<std::string> buffer;

public:
  terminal();
  ~terminal();

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
  GLuint       get_port_out_video_analogue(unsigned int port);
  std::string  get_port_out_text(          unsigned int port);
  void update();
  void update_if_time();
  void refresh();
};

#endif // TERMINAL_H_INCLUDED
