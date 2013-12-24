#ifndef MAPPER_SYSTEM_H_INCLUDED
#define MAPPER_SYSTEM_H_INCLUDED

#include <deque>
#include "vmath.h"
#include "instrument.h"

class body;

class mapper_system : public device {
  /// A mapping computer rendering an orthographic view of the present (solar) system
private:
  Vector2i static const windowsize;

  GLuint display_image;
  GLuint framebuffer;
  GLuint depthbuffer;

  double scale;                               // scale multiplier (zoom)
  double rotation_x;                          // map rotation around x axis
  double rotation_y;                          // map rotation around y axis
  body *trail_ref;

  struct trailtype {
    Vector3d linepoint;
    double fade;
  };
  unsigned int static constexpr trailperiod = 2;    // draw a dash in this many frames
  //double static constexpr trailfade = 0.999;        // how much the trails fade every tick
  double static constexpr trailfade = 0.9995;       // how much the trails fade every tick
  unsigned int trailcounter = trailperiod;

  std::deque<trailtype> trails;

  Vector3d centreoffset;

public:
  mapper_system();
  ~mapper_system();

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
  void update();
  void update_if_time();
  void refresh();
};

#endif // MAPPER_SYSTEM_H_INCLUDED
