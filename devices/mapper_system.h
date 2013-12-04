#ifndef MAPPER_SYSTEM_H_INCLUDED
#define MAPPER_SYSTEM_H_INCLUDED

#include "device.h"
#include "vmath.h"
#include "body.h"

class mapper_system : public device {
  /// A mapping computer rendering an orthographic view of the present (solar) system
private:
  double scale;
  Matrix4d rotation;
  body *trail_ref;

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
  void         get_port_out_video_analogue(unsigned int port);
  void update();
};

#endif // MAPPER_SYSTEM_H_INCLUDED
