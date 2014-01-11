#ifndef DISPLAY_H_INCLUDED
#define DISPLAY_H_INCLUDED

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "instrument.h"

class display : public instrument {
  /// General display device that takes a video signal and displays 2D imagery
protected:
  GLuint vbo_screen_v;                // vertex buffer object for vertices
  GLuint vbo_screen_n;                // vertex buffer object for normals
  GLuint vbo_screen_t;                // vertex buffer object for texture coords
  GLuint ibo_screen;                  // index buffer object
  GLuint display_image;               // cached reference to the texture we use

public:
  display();
  virtual ~display();

  virtual std::string get_name();
  virtual std::string get_model();
  virtual std::string get_description();
  virtual double      get_mass();
  unsigned int        get_port_in_count();
  std::string         get_port_in_name(       unsigned int port);
  std::string         get_port_in_description(unsigned int port);
  bool                get_port_in_required(   unsigned int port);
  virtual Vector3d    get_size();

  virtual void update();
  virtual void update_vbo();
  virtual void render();
};

#endif // DISPLAY_H_INCLUDED
