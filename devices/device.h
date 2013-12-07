#ifndef DEVICE_H_INCLUDED
#define DEVICE_H_INCLUDED

#include <string>
#include <vector>
#include <boost/chrono.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "vmath.h"
#include "random_engine.h"

class spacecraft;

class device : public random_engine {
private:
  Vector2i static const screensize_static_analogue;    // screen size for tv type static generation
  Vector2i static const screensize_static_digital;     // screen size for digital noise type static
  GLuint static image_static_analogue;           // global texture for tv type static
  GLuint static image_static_digital;            // global texture for digital noise type static
  boost::chrono::time_point<boost::chrono::high_resolution_clock, boost::chrono::duration<double>> static time_next_static_analogue;
  boost::chrono::time_point<boost::chrono::high_resolution_clock, boost::chrono::duration<double>> static time_next_static_digital;
  boost::chrono::duration<double> static const time_interval_static_analogue;
  boost::chrono::duration<double> static const time_interval_static_digital;

protected:
  boost::chrono::time_point<boost::chrono::high_resolution_clock, boost::chrono::duration<double>> time_nextupdate;

public:
  struct port_in_type {
    device *target;                   // output device this is connected to
    unsigned int target_port;         // connected port number on the target
  };

  std::vector<port_in_type> ports_in;

  spacecraft *vessel;                   // what vessel it belongs to

  bool functional;                      // whether it's currently working

  device();
  virtual ~device();

  virtual std::string  get_name();
  virtual std::string  get_manufacturer();
  virtual std::string  get_model();
  virtual std::string  get_description();
  virtual double       get_mass();
  virtual unsigned int get_port_in_count();
  virtual std::string  get_port_in_name(           unsigned int port);
  virtual std::string  get_port_in_description(    unsigned int port);
  virtual bool         get_port_in_required(       unsigned int port);
  virtual std::string  get_port_in_connstatus(     unsigned int port);
  virtual unsigned int get_port_out_count();
  virtual std::string  get_port_out_name(          unsigned int port);
  virtual std::string  get_port_out_description(   unsigned int port);
  virtual double       get_port_out_data(          unsigned int port);
  virtual std::string  get_port_out_text(          unsigned int port);
  virtual GLuint       get_port_out_video_digital( unsigned int port);
  virtual GLuint       get_port_out_video_analogue(unsigned int port);
  virtual void         get_port_out_sound(         unsigned int port);
  virtual void update();
  virtual void update_if_time();

  GLuint generate_static_analogue();
  GLuint generate_static_digital();

  virtual void attach(spacecraft *to_vessel);
  virtual void remove();
  void connect(unsigned int port_in, device *target, unsigned int target_port_out);
  void disconnect(unsigned int port_in);
  void disconnect_all();

  void destroy();

  void describe_to_console();
};

#endif // DEVICE_H_INCLUDED
