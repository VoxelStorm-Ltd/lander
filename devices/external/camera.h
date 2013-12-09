#ifndef CAMERA_H_INCLUDED
#define CAMERA_H_INCLUDED

#include <string>
#include "device_external.h"

class camera : public device_external {
  /// A digital camera that observes in the visual range and outputs a suitable signal
private:
  Vector2i static const windowsize;

  GLuint display_image;
  GLuint framebuffer;
  GLuint depthbuffer;

  double zoom;                                // zoom multiplier
  double rotation_x;                          // map rotation around x axis
  double rotation_y;                          // map rotation around y axis

public:
  camera();
  ~camera();

  std::string  get_name();
  std::string  get_model();
  std::string  get_description();
  double       get_mass();
  Vector3d     get_size();
  unsigned int get_port_in_count();
  std::string  get_port_in_name(           unsigned int port);
  std::string  get_port_in_description(    unsigned int port);
  bool         get_port_in_required(       unsigned int port);
  unsigned int get_port_out_count();
  std::string  get_port_out_name(          unsigned int port);
  std::string  get_port_out_description(   unsigned int port);
  GLuint       get_port_out_video_digital( unsigned int port);

  void update();
  void update_if_time();
  void refresh();
  //void render();
};

#endif // CAMERA_H_INCLUDED
