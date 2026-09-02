#ifndef CAMERA_H_INCLUDED
#define CAMERA_H_INCLUDED

#include <string>
#include "device_external.h"

class camera : public device_external {
  /// A digital camera that observes in the visual range and outputs a suitable signal
private:
  vector2i static const windowsize;

  GLuint display_image;
  GLuint framebuffer;
  GLuint depthbuffer;

  double zoom;                                                                  // zoom multiplier
  double rotation_x;                                                            // map rotation around x axis
  double rotation_y;                                                            // map rotation around y axis
  double fov_angle;                                                             // cached field of view, degrees
  double fov_ratio;                                                             // updated by update_fov_ratio() from degrees
  double aspect_ratio;                                                          // updated by update_aspect_ratio() from windowsize

  double nearplane;
  double farplane;

public:
  camera();
  ~camera();

  std::string  get_name() override final;
  std::string  get_model() override final;
  std::string  get_description() override final;
  double       get_mass() override final;
  vector3d     get_size() override final;
  unsigned int get_port_in_count() override final;
  std::string  get_port_in_name(           unsigned int port) override final;
  std::string  get_port_in_description(    unsigned int port) override final;
  bool         get_port_in_required(       unsigned int port) override final;
  unsigned int get_port_out_count() override final;
  std::string  get_port_out_name(          unsigned int port) override final;
  std::string  get_port_out_description(   unsigned int port) override final;
  GLuint       get_port_out_video_digital( unsigned int port) override final;

  void update() override final;
  void update_if_time() override final;
  void update_fov(double fov = 90);
  void update_fov_ratio();
  void refresh();
  //void render() override final;
};

#endif // CAMERA_H_INCLUDED
