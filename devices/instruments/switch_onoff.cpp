#include "switch_onoff.h"

switch_onoff::switch_onoff()
  : setting(0) {
  /// Default constructor
  ports_in.resize(get_port_in_count());     // anything with input ports needs this
}

switch_onoff::~switch_onoff() {
  /// Default destructor
}

std::string switch_onoff::get_name() {
  /// Return the name of this device
  return "on/off switch";
}

std::string switch_onoff::get_model() {
  /// Return a model name for this device
  random_reset();
  std::stringstream model;
  model << "SW" << get_random_int(10, 90);
  return model.str();
}

std::string switch_onoff::get_description() {
  /// Return a detailed description of this device
  return "A simple but durable two setting switch.  "
         "By default it switches between outputting values of 0 and 1, but can be configured to output any pair of values.";
}

double switch_onoff::get_mass() {
  /// Return the weight of the device, in kilograms
  return 0.05;
}

Vector3d switch_onoff::get_size() {
  /// Return a size for this object, in metres - hardcoded
  return Vector3d(0.02, 0.02, 0.01);
}

unsigned int switch_onoff::get_port_in_count() {
  /// How many input ports it has
  return 2;
}

std::string switch_onoff::get_port_in_name(unsigned int port) {
  /// Name of the input port numbered n
  switch(port) {
  case 0:
    return "off value";
  case 1:
    return "on value";
  default:
    return "";
  }
}

std::string switch_onoff::get_port_in_description(unsigned int port) {
  /// Description of the input port numbered n
  switch(port) {
  case 0:
    {
      std::stringstream desc;
      desc << "What to output when set to \"off\" position.  Defaults to 0 if not connected."
              "  " << get_port_in_connstatus(port);
      return desc.str();
    }
  case 1:
    {
      std::stringstream desc;
      desc << "What to output when set to \"on\" position.  Defaults to 1 if not connected."
              "  " << get_port_in_connstatus(port);
      return desc.str();
    }
  default:
    return "";
  }
}

bool switch_onoff::get_port_in_required(unsigned int port __attribute__((__unused__))) {
  /// Whether an input on this port is necessary for this device to operate
  // all inputs are optional
  return false;
}

unsigned int switch_onoff::get_port_out_count() {
  /// How many output ports it has
  return 1;
}

std::string switch_onoff::get_port_out_name(unsigned int port __attribute__((__unused__))) {
  /// Name of the output port numbered n
  return "output";
}

std::string switch_onoff::get_port_out_description(unsigned int port  __attribute__((__unused__))) {
  /// Description of the output port numbered n
  return "A numerical value of 0 or 1, or a passthrough for whatever input is connected.  Passthrough values can be any data type, not just numerical.";
}

double switch_onoff::get_port_out_data(unsigned int port __attribute__((__unused__))) {
  /// Query the value data on the specified out port
  if(ports_in[setting].target) {
    return ports_in[setting].target->get_port_out_data(ports_in[setting].target_port);
  } else {
    return static_cast<double>(setting);
  }
}

std::string switch_onoff::get_port_out_text(unsigned int port __attribute__((__unused__))) {
  /// Query the text data on the specified out port
  if(ports_in[setting].target) {
    return ports_in[setting].target->get_port_out_text(ports_in[setting].target_port);
  } else {
    if(setting == 0) {
      return "off";
    } else {
      return "on";
    }
  }
}

GLuint switch_onoff::get_port_out_video_analogue(unsigned int port __attribute__((__unused__))) {
  /// Query the analogue video data on the specified out port - returns a texture id
  if(ports_in[setting].target) {
    return ports_in[setting].target->get_port_out_video_analogue(ports_in[setting].target_port);
  } else {
    if(setting == 0) {
      return 0;
    } else {
      return generate_static_analogue();
    }
  }
}

GLuint switch_onoff::get_port_out_video_digital(unsigned int port __attribute__((__unused__))) {
  /// Query the digital video data on the specified out port - returns a texture id
  if(ports_in[setting].target) {
    return ports_in[setting].target->get_port_out_video_digital(ports_in[setting].target_port);
  } else {
    if(setting == 0) {
      return 0;
    } else {
      return generate_static_digital();
    }
  }
}

void switch_onoff::get_port_out_sound(unsigned int port __attribute__((__unused__))) {
  /// Query the audio data on the specified out port
  if(ports_in[setting].target) {
    return ports_in[setting].target->get_port_out_sound(ports_in[setting].target_port);
  } else {
    if(setting == 0) {
      return;
    } else {
      return generate_static_sound();
    }
  }
}

void device::activate() {
  /// Activate the current device, whatever that means for this particular one
  setting = (setting + 1) % get_port_in_count();
  std::cout << "New setting: " << setting << std::endl;
}
