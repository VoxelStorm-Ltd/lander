#ifndef TERMINAL_H_INCLUDED
#define TERMINAL_H_INCLUDED

#include <deque>
#include <string>
#include "device.h"

class terminal : public device {
  /// TTY style serial terminal, reading text from a text device and outputting to display
private:
  vector2<unsigned int> static const windowsize;
  vector2<unsigned int> static const windowsize_text;                           // rows and columns
  double static constexpr lineheight = 10.0;
  double static constexpr margin_bottom = 16.0;
  double static constexpr margin_left   = 16.0;
  GLuint display_image;
  GLuint framebuffer;

  std::deque<std::string> buffer;

public:
  terminal();
  ~terminal();

  std::string  get_name() override final;
  std::string  get_model() override final;
  std::string  get_description() override final;
  vector3d     get_size() override final;
  double       get_mass() override final;
  unsigned int get_port_in_count() override final;
  std::string  get_port_in_name(           unsigned int port) override final;
  std::string  get_port_in_description(    unsigned int port) override final;
  bool         get_port_in_required(       unsigned int port) override final;
  unsigned int get_port_out_count() override final;
  std::string  get_port_out_name(          unsigned int port) override final;
  std::string  get_port_out_description(   unsigned int port) override final;
  GLuint       get_port_out_video_analogue(unsigned int port) override final;
  std::string  get_port_out_text(          unsigned int port) override final;
  void update() override final;
  void update_if_time() override final;
  void refresh();
};

#endif // TERMINAL_H_INCLUDED
