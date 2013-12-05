#include "device.h"
#include <sstream>
#include <iostream>
#include "spacecraft.h"

device::device()
  : vessel(nullptr),
    functional(true) {
  /// Default constructor
}

device::~device() {
  /// Default destructor
  disconnect_all();
}

std::string device::get_name() {
  /// Return the name of this device
  // virtual placeholder
  std::cout << "WARNING: virtual function " << __PRETTY_FUNCTION__ << " called without specialisation - this should never happen." << std::endl;
  return "Device";
}

std::string device::get_manufacturer() {
  /// Return a manufacturer name for this device
  random_reset();
  return get_random_name_corporation();
}

std::string device::get_model() {
  /// Return a model name for this device
  // virtual placeholder
  std::cout << "WARNING: virtual function " << __PRETTY_FUNCTION__ << " called without specialisation - this should never happen." << std::endl;
  return "DEV0001a";
}

std::string device::get_description() {
  /// Return a detailed description of this device
  // virtual placeholder
  std::cout << "WARNING: virtual function " << __PRETTY_FUNCTION__ << " called without specialisation - this should never happen." << std::endl;
  return "A device of some sort.  It doesn't appear to do anything.";
}

unsigned int device::get_port_in_count() {
  /// How many input ports it has
  return 0;
}

std::string device::get_port_in_name(unsigned int port __attribute__((__unused__))) {
  /// Name of the input port numbered n
  // virtual placeholder
  std::cout << "WARNING: virtual function " << __PRETTY_FUNCTION__ << " called without specialisation - this should never happen." << std::endl;
  return "";
}

std::string device::get_port_in_description(unsigned int port __attribute__((__unused__))) {
  /// Description of the input port numbered n
  // virtual placeholder
  std::cout << "WARNING: virtual function " << __PRETTY_FUNCTION__ << " called without specialisation - this should never happen." << std::endl;
  return "";
}

bool device::get_port_in_required(unsigned int port __attribute__((__unused__))) {
  /// Whether an input on this port is necessary for this device to operate
  return false;
}

std::string device::get_port_in_connstatus(unsigned int port) {
  /// Get a verbal description of the connection status of this input port
  std::stringstream desc;
  if(ports_in[port].target) {
    device *target = ports_in[port].target;
    desc << "Connected to \"" << target->get_port_out_name(ports_in[port].target_port)
         << "\" port of " << target->get_name();
    if(!target->functional) {
      desc << " (not functional)";
    }
    desc << ".";
  } else {
    desc << "Not connected";
    if(get_port_in_required(port)) {
      desc << " (required).";
    } else {
      desc << " (optional).";
    }
  }
  return desc.str();
}

unsigned int device::get_port_out_count() {
  /// How many output ports it has
  return 0;
}

std::string device::get_port_out_name(unsigned int port __attribute__((__unused__))) {
  /// Name of the output port numbered n
  // virtual placeholder
  std::cout << "WARNING: virtual function " << __PRETTY_FUNCTION__ << " called without specialisation - this should never happen." << std::endl;
  return "";
}

std::string device::get_port_out_description(unsigned int port __attribute__((__unused__))) {
  /// Description of the output port numbered n
  // virtual placeholder
  std::cout << "WARNING: virtual function " << __PRETTY_FUNCTION__ << " called without specialisation - this should never happen." << std::endl;
  return "";
}

double device::get_port_out_data(unsigned int port __attribute__((__unused__))) {
  /// Query the value data on the specified out port
  // virtual placeholder - this may get called if nothing specialises it
  return 0.0;
}

std::string device::get_port_out_text(unsigned int port) {
  /// Query the text data on the specified out port
  // generate a string of random junk
  std::stringstream ss;
  for(unsigned int i = 0; i != 8; ++i) {
    ss << static_cast<char>(get_random_uint(33, 126));
  }
  return ss.str();
}

void device::get_port_out_video_analogue(unsigned int port          __attribute__((__unused__)),
                                         Vector2i const &windowsize __attribute__((__unused__))) {
  /// Query the analogue video data on the specified out port
  // virtual placeholder - this may get called if nothing specialises it
  // this gets called if someone connects an analogue video display to a non-video output
  // TODO: render static
}

void device::get_port_out_video_digital(unsigned int port          __attribute__((__unused__)),
                                        Vector2i const &windowsize __attribute__((__unused__))) {
  /// Query the digital video data on the specified out port
  // virtual placeholder - this may get called if nothing specialises it
  // this gets called if someone connects a digital video display to a non-video output
  // TODO: render digital junk
}

void device::get_port_out_sound(unsigned int port __attribute__((__unused__))) {
  /// Query the audio data on the specified out port
  // virtual placeholder - this may get called if nothing specialises it
  // this gets called if someone connects a audio speaker to a non-audio output
  // TODO: output hissing / buzzing / glitch noises
}

void device::attach(spacecraft *to_vessel) {
  /// Attach this device to the specified ship
  if(!to_vessel) {
    std::cout << "ERROR: tried to attach " << get_name() << " to null vessel." << std::endl;
    return;
  }
  vessel = to_vessel;
  vessel->devices.push_back(this);
}

void device::remove() {
  /// Remove this device from whatever it's attached to
  if(!vessel) {
    std::cout << "ERROR: tried to remove " << get_name() << " which is already not attached to anything." << std::endl;
    return;
  }
  vessel = nullptr;
  // remove it from the list of the vessel's devices
  //vessel->devices.erase(std::remove(vessel->devices.begin(), vessel->devices.end(), this), vessel->devices.end());
  vessel->devices.remove(this);
  // make sure there are no connections dangling to this device
  for(auto const &it : vessel->devices) {
    for(unsigned int i = 0; i != it->get_port_in_count(); ++i) {
      if(it->ports_in[i].target == this) {
        it->disconnect(i);
      }
    }
  }
}

void device::connect(unsigned int port_in, device *target, unsigned int target_port_out) {
  /// Connect an input port on this device to an output port elsewhere
  if(port_in >= get_port_in_count()) {
    std::cout << "ERROR: tried to connect to input port " << port_in << " on device " << get_name() << " which only has " << get_port_in_count() << " ports." << std::endl;
    return;
  }
  if(!target) {
    std::cout << "ERROR: tried to connect to port on null target." << std::endl;
    return;
  }
  if(target_port_out >= target->get_port_out_count()) {
    std::cout << "ERROR: tried to connect " << get_name() << " to output port " << target_port_out << " on device " << target->get_name() << " which only has " << target->get_port_out_count() << " ports." << std::endl;
    return;
  }
  ports_in[port_in].target = target;
  ports_in[port_in].target_port = target_port_out;
  update();
  target->update();
}

void device::disconnect(unsigned int port_in) {
  /// Disconnect an input port on this device
  if(port_in >= get_port_in_count()) {
    std::cout << "ERROR: tried to disconnect input port " << port_in << " on device " << get_name() << " which only has " << get_port_in_count() << " ports." << std::endl;
    return;
  }
  device *cachedtarget = ports_in[port_in].target;
  ports_in[port_in].target = nullptr;
  ports_in[port_in].target_port = 0;
  update();
  cachedtarget->update();
}

void device::disconnect_all() {
  // make sure there are no dangling connections from other devices to this
  for(auto const &it : vessel->devices) {
    if(!it || it == this) {
      continue;         // don't check for connection to ourself
    }
    unsigned int numports = it->get_port_in_count();
    for(unsigned int i = 0; i != numports; ++i) {
      if(it->ports_in[i].target == this) {
        std::cout << it->get_name() << " was disconnected from " << get_name();
        it->disconnect(i);
      }
    }
  }
}

void device::update() {
  /// Update the output states and respond to changes in input
  // virtual placeholder - may be called by classes that don't specialise this
}

void device::destroy() {
  /// Put this device out of commission
  std::cout << get_name() << " is no longer operative." << std::endl;
  functional = false;
}

void device::describe_to_console() {
  /// Dump a vebrose description of this device and all its connections to the console
  std::cout << "*** " << get_manufacturer() << " model " << get_model() << ": " << get_name() << " ***" << std::endl;
  std::cout << get_description() << std::endl;
  std::cout << "Input ports: " << get_port_in_count() << std::endl;
  for(unsigned int i = 0; i != get_port_in_count(); ++i) {
    std::cout << "  " << i + 1 << ": " << get_port_in_name(i) << std::endl;
    std::cout << "    " << get_port_in_description(i) << std::endl;
  }
  std::cout << "Output ports: " << get_port_out_count() << std::endl;
  for(unsigned int i = 0; i != get_port_out_count(); ++i) {
    std::cout << "  " << i + 1 << ": " << get_port_out_name(i) << std::endl;
    std::cout << "    " << get_port_out_description(i) << std::endl;
  }
}
