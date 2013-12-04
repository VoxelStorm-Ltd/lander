#include "display.h"

display::display() {
  /// Default constructor
  ports_in.resize(get_port_in_count());     // anything with input ports needs this
}

display::~display() {
  /// Default destructor
}

std::string display::get_name() {
  /// Return the name of this device
  return "graphical display";
}

std::string display::get_description() {
  /// Return a detailed description of this device
  random_reset();
  std::stringstream desc;
  desc << get_random_name_corporation() << " model CRT0606-G."
          "  A very cheap and old but reliable analogue CRT monitor."
          "  Capable of withstanding a lot of punishment, both physical and electromagnetic."
          "  Technically capable of displaying any colour, but for some reason tends to tint everything with a sickly greenish hue."
          "  Slightly increases the pilot's overall radiation exposure.";
  return desc.str();
}

unsigned int display::get_port_in_count() {
  /// How many input ports it has
  return 1;
}

std::string display::get_port_in_name(unsigned int port) {
  /// Name of the input port numbered n
  switch(port) {
  case 0:
    return "analogue video";
  default:
    return "";
  }
}

std::string display::get_port_in_description(unsigned int port) {
  /// Description of the input port numbered n
  switch(port) {
  case 0:
    {
      std::stringstream desc;
      desc << "An analogue video or still image signal."
              "  " << get_port_in_connstatus(port);
      return desc.str();
    }
  default:
    return "";
  }
}

bool display::get_port_in_required(unsigned int port __attribute__((__unused__))) {
  /// Whether an input on this port is necessary for this device to operate
  // all ports are needed
  return true;
}
