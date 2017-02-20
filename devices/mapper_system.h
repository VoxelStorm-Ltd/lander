#ifndef MAPPER_SYSTEM_H_INCLUDED
#define MAPPER_SYSTEM_H_INCLUDED

#include <deque>
#include "vectorstorm/vectorstorm.h"
#include "instrument.h"

class body;

class mapper_system : public device {
  /// A mapping computer rendering an orthographic view of the present (solar) system
private:
  Vector2i static const windowsize;

  GLuint display_image;
  GLuint framebuffer;
  GLuint depthbuffer;

  double scale;                                                                 // scale multiplier (zoom)
  double rotation_x;                                                            // map rotation around x axis
  double rotation_y;                                                            // map rotation around y axis
  body *trail_ref;
  body *trail_ref_last;                                                         // cached version of the pointer

  struct trailtype {
    Vector3d linestart;
    Vector3d lineend;
    double fade;
  };
  unsigned int static constexpr trailperiod = 2;                                // draw a dash in this many frames
  double static constexpr trailfade = 0.999;                                    // how much the trails fade every tick
  //double static constexpr trailfade = 0.9995;                                   // how much the trails fade every tick
  unsigned int trailcounter = trailperiod;

  std::deque<trailtype> trails;

  Vector3d centreoffset;

public:
  mapper_system();
  ~mapper_system();

  std::string  get_name() override final;
  std::string  get_model() override final;
  std::string  get_description() override final;
  Vector3d     get_size() override final;
  double       get_mass() override final;
  unsigned int get_port_in_count() override final;
  std::string  get_port_in_name(           unsigned int port) override final;
  std::string  get_port_in_description(    unsigned int port) override final;
  bool         get_port_in_required(       unsigned int port) override final;
  unsigned int get_port_out_count() override final;
  std::string  get_port_out_name(          unsigned int port) override final;
  std::string  get_port_out_description(   unsigned int port) override final;
  GLuint       get_port_out_video_analogue(unsigned int port) override final;
  void update() override final;
  void update_if_time() override final;
  void refresh();
};

#endif // MAPPER_SYSTEM_H_INCLUDED
