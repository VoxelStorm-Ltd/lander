#ifndef MEMORY_H_INCLUDED
#define MEMORY_H_INCLUDED

#include "instrument.h"

class memory : public instrument {
  /// Memory chip capable of remembering the last inputs of each kind it got
private:
  // stored values
  double memory_value;
  GLuint memory_image;
  std::string memory_text;

  bool loop_safe;                                                               // to detect circular references and stick with cached values

public:
  memory();
  ~memory();

  std::string  get_name() override final;
  std::string  get_model() override final;
  std::string  get_description() override final;
  double       get_mass() override final;
  unsigned int get_port_in_count() override final;
  std::string  get_port_in_name(           unsigned int port) override final;
  std::string  get_port_in_description(    unsigned int port) override final;
  bool         get_port_in_required(       unsigned int port) override final;
  unsigned int get_port_out_count() override final;
  std::string  get_port_out_name(          unsigned int port) override final;
  std::string  get_port_out_description(   unsigned int port) override final;
  double       get_port_out_data(          unsigned int port) override final;
  std::string  get_port_out_text(          unsigned int port) override final;
  GLuint       get_port_out_video_digital( unsigned int port) override final;
  GLuint       get_port_out_video_analogue(unsigned int port) override final;
  void         get_port_out_sound(         unsigned int port) override final;
  Vector3d     get_size() override final;
  void update() override final;

  // memory-chip specific setters:
  void set_memory_value(double newvalue);
  void set_memory_text(std::string const &newstring);
};

#endif // MEMORY_H_INCLUDED
