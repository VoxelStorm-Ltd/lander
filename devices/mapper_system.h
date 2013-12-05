#ifndef MAPPER_SYSTEM_H_INCLUDED
#define MAPPER_SYSTEM_H_INCLUDED

#include <deque>
#include "vmath.h"
#include "device.h"

class body;

class mapper_system : public device {
  /// A mapping computer rendering an orthographic view of the present (solar) system
private:
  //Vector2i windowsize;

  double scale;
  Matrix4d rotation;
  body *trail_ref;

  struct trailtype {
    Vector3d linepoint;
    double fade;
  };
  unsigned int const trailperiod = 60;       // draw a dash in this many frames
  unsigned int trailcounter = trailperiod;

  std::deque<trailtype> trails;

  Vector3d centreoffset;

public:
  mapper_system();
  ~mapper_system();

  std::string  get_name();
  std::string  get_description();
  unsigned int get_port_in_count();
  std::string get_port_in_name(            unsigned int port);
  std::string get_port_in_description(     unsigned int port);
  bool get_port_in_required(               unsigned int port);
  unsigned int get_port_out_count();
  std::string  get_port_out_name(          unsigned int port);
  std::string  get_port_out_description(   unsigned int port);
  void         get_port_out_video_analogue(unsigned int port, Vector2i windowsize);
  void update();
};

#endif // MAPPER_SYSTEM_H_INCLUDED
