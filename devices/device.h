#ifndef DEVICE_H_INCLUDED
#define DEVICE_H_INCLUDED

#include <string>
#include <vector>
#include "random_engine.h"

class spacecraft;

class device : public random_engine {
public:
  struct port_in_type {
    //std::string name;                 // short name for what input is expected
    //std::string description;          // verbose description of what input is expected
    device *target;                   // output device this is connected to
    unsigned int target_port;         // connected port number on the target
  };
  //struct port_out_type {
  //  //std::string name;                 // short name for what output is provided
  //  //std::string description;          // verbose description of what output is provided
  //};

  std::vector<port_in_type> ports_in;
  //std::vector<port_out_type> ports_out;

  spacecraft *vessel;                   // what vessel it belongs to

  bool functional;                      // whether it's currently working

  device();
  virtual ~device();

  virtual std::string  get_name();
  virtual std::string  get_description();
  virtual unsigned int get_port_in_count();
  virtual std::string  get_port_in_name(           unsigned int port);
  virtual std::string  get_port_in_description(    unsigned int port);
  virtual bool         get_port_in_required(       unsigned int port);
  virtual std::string  get_port_in_connstatus(     unsigned int port);
  virtual unsigned int get_port_out_count();
  virtual std::string  get_port_out_name(          unsigned int port);
  virtual std::string  get_port_out_description(   unsigned int port);
  virtual double       get_port_out_data(          unsigned int port);
  virtual void         get_port_out_video_digital( unsigned int port);
  virtual void         get_port_out_video_analogue(unsigned int port);
  virtual void         get_port_out_sound(         unsigned int port);
  virtual void update();

  virtual void attach(spacecraft *to_vessel);
  virtual void remove();
  void connect(unsigned int port_in, device *target, unsigned int target_port_out);
  void disconnect(unsigned int port_in);

  void destroy();

  void describe_to_console();
};

#endif // DEVICE_H_INCLUDED
