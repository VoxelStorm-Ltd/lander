#ifndef DISPLAY_H_INCLUDED
#define DISPLAY_H_INCLUDED

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "instrument.h"

class display : public instrument {
  /// General display device that takes a video signal and displays 2D imagery
private:
  GLuint display_image;
  GLuint framebuffer;

  GLuint static const screen_width = 256;
  GLuint static const screen_height = screen_width;

public:
  display();
  ~display();

  std::string get_name();
  std::string get_description();
  unsigned int get_port_in_count();
  std::string get_port_in_name(       unsigned int port);
  std::string get_port_in_description(unsigned int port);
  bool get_port_in_required(          unsigned int port);

  //void update();
  void render();
};

#endif // DISPLAY_H_INCLUDED
