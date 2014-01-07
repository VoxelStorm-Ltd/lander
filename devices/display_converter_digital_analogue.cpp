#include "display_converter_digital_analogue.h"

display_converter_digital_analogue::display_converter_digital_analogue() {
  /// Default constructor
  ports_in.resize(get_port_in_count());     // anything with input ports needs this
  update_vbo();                             // every device with a custom size needs this
}

display_converter_digital_analogue::~display_converter_digital_analogue() {
  /// Default destructor
}

std::string display_converter_digital_analogue::get_name() {
  /// Return the name of this device
  return "digital to analogue video converter";
}

std::string display_converter_digital_analogue::get_model() {
  /// Return a model name for this device
  random_reset();
  std::stringstream model;
  model << "VidDAC-" << get_random_char_alpha_upper() << get_random_char_alpha_upper() << get_random_int(1, 9) << "00";
  return model.str();
}

std::string display_converter_digital_analogue::get_description() {
  /// Return a detailed description of this device
  return "A converter from digital video to analogue signals."
         "  A pocket sized device for converting digital electronic signals "
         "to analogue output that can be displayed on older CRT monitors.";
}

double display_converter_digital_analogue::get_mass() {
  /// Return the weight of the device, in kilograms
  return 0.8;
}

Vector3d display_converter_digital_analogue::get_size() {
  /// Return a size for this object, in metres - hardcoded
  return Vector3d(0.1, 0.12, 0.03);
}

unsigned int display_converter_digital_analogue::get_port_in_count() {
  /// How many input ports it has
  return 1;
}

std::string display_converter_digital_analogue::get_port_in_name(unsigned int port __attribute__((__unused__))) {
  /// Name of the input port numbered n
  return "digital video in";
}

std::string display_converter_digital_analogue::get_port_in_description(unsigned int port) {
  /// Description of the input port numbered n
  std::stringstream desc;
  desc << "A digital video or still image signal."
          "  " << get_port_in_connstatus(port);
  return desc.str();
}

bool display_converter_digital_analogue::get_port_in_required(unsigned int port __attribute__((__unused__))) {
  /// Whether an input on this port is necessary for this device to operate
  return false;
}

unsigned int display_converter_digital_analogue::get_port_out_count() {
  /// How many output ports it has
  return 1;
}

std::string display_converter_digital_analogue::get_port_out_name(unsigned int port __attribute__((__unused__))) {
  /// Name of the output port numbered n
  return "analogue video out";
}

std::string display_converter_digital_analogue::get_port_out_description(unsigned int port __attribute__((__unused__))) {
  /// Description of the output port numbered n
  return "An analogue video signal.";
}

GLuint display_converter_digital_analogue::get_port_out_video_analogue(unsigned int port __attribute__((__unused__))) {
  if(ports_in[0].target) {
    return ports_in[0].target->get_port_out_video_digital(ports_in[0].target_port);
  } else {
    return generate_static_analogue();
  }
}
