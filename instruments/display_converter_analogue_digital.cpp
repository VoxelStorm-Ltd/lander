#include "display_converter_analogue_digital.h"

display_converter_analogue_digital::display_converter_analogue_digital() {
  /// Default constructor
  ports_in.resize(get_port_in_count());     // anything with input ports needs this
}

display_converter_analogue_digital::~display_converter_analogue_digital() {
  /// Default destructor
}

std::string display_converter_analogue_digital::get_name() {
  /// Return the name of this device
  return "analogue to digital video converter";
}

std::string display_converter_analogue_digital::get_model() {
  /// Return a model name for this device
  random_reset();
  std::stringstream model;
  model << "VidADC-" << get_random_int(1, 9) << "0" << get_random_int(1, 9);
  return model.str();
}

std::string display_converter_analogue_digital::get_description() {
  /// Return a detailed description of this device
  return "A digitiser for analogue video signals."
         "  This small device converts analogue signals from older equipment "
         "to a digital format suitable for modern displays."
         "  Now you can plug in your ZX Spectrum!";
}

double display_converter_analogue_digital::get_mass() {
  /// Return the weight of the device, in kilograms
  return 0.8;
}

Vector3d display_converter_analogue_digital::get_size() {
  /// Return a size for this object, in metres - hardcoded
  return Vector3d(0.08, 0.1, 0.04);
}

unsigned int display_converter_analogue_digital::get_port_in_count() {
  /// How many input ports it has
  return 1;
}

std::string display_converter_analogue_digital::get_port_in_name(unsigned int port __attribute__((__unused__))) {
  /// Name of the input port numbered n
  switch(port) {
  case 0:
    return "analogue video in";
  default:
    return "";
  }
}

std::string display_converter_analogue_digital::get_port_in_description(unsigned int port __attribute__((__unused__))) {
  /// Description of the input port numbered n
  switch(port) {
  case 0:
    return "An analogue video or still image signal.";
  default:
    return "";
  }
}

bool display_converter_analogue_digital::get_port_in_required(unsigned int port __attribute__((__unused__))) {
  /// Whether an input on this port is necessary for this device to operate
  return false;
}

unsigned int display_converter_analogue_digital::get_port_out_count() {
  /// How many output ports it has
  return 1;
}

std::string display_converter_analogue_digital::get_port_out_name(unsigned int port __attribute__((__unused__))) {
  /// Name of the output port numbered n
  switch(port) {
  case 0:
    return "digital video out";
  default:
    return "";
  }
}

std::string display_converter_analogue_digital::get_port_out_description(unsigned int port __attribute__((__unused__))) {
  /// Description of the output port numbered n
  switch(port) {
  case 0:
    return "A digital video signal.";
  default:
    return "";
  }
}

GLuint display_converter_analogue_digital::get_port_out_video_digital(unsigned int port __attribute__((__unused__))) {
  /// Take an input signal and "convert" it
  if(ports_in[0].target) {
    return ports_in[0].target->get_port_out_video_analogue(ports_in[0].target_port);
  } else {
    return generate_static_digital();
  }
}
