#include "attitude_cmg.h"
#include "spacecraft.h"
#include "instrumentpanel.h"

attitude_cmg::attitude_cmg() {
  /// Default constructor
  ports_in.resize(get_port_in_count());                                         // anything with input ports needs this
  update_vbo();                                                                 // every device with a custom size needs this
}

attitude_cmg::~attitude_cmg() {
  /// Default destructor
}

std::string attitude_cmg::get_name() {
  /// Return the name of this device
  return "attitude_cmg";
}

std::string attitude_cmg::get_model() {
  /// Return a model name for this device
  random_reset();
  return "CMG-600";
}

std::string attitude_cmg::get_description() {
  /// Return a detailed description of this device
  std::stringstream ss;
  ss << "A Control Moment Gyro consists of a large flat stainless steel flywheel that rotates at "
        "a constant speed of 6,600 rpm and develops an angular momentum of 4,880 Nms "
        "about its spin axis. This rotating wheel is mounted in a two-degree-of-freedom "
        "gimbal system that can point the spin axis (momentum vector) of the wheel in "
        "any direction. Control motors change the orientation of the spinning rotors "
        "to produce torque on the spacecraft to balance the effects of gravity and aerodynamics, "
        "maintaining the spacecraft at an equilibrium attitude without using propellant.";
  return ss.str();
}

double attitude_cmg::get_mass() {
  /// Return the weight of the device, in kilograms
  return 272.155;                                                               // based on http://www.boeing.com/assets/pdf/defense-space/space/spacestation/systems/docs/ISS%20Motion%20Control%20System.pdf
}

Vector3d attitude_cmg::get_size() {
  /// Return a size for this object, in metres - hardcoded
  return Vector3d(1.3716, 1.2192, 1.143);                                       // based on http://www.boeing.com/assets/pdf/defense-space/space/spacestation/systems/docs/ISS%20Motion%20Control%20System.pdf
}


bool attitude_cmg::attach_panel(instrumentpanel *to_panel __attribute__((__unused__))) {
  /// Attempt to attach this device to an instrument panel, return success status
  // too big to fit on a panel
  return false;
}

bool attitude_cmg::attach_hull() {
  /// Attempt to attach this device to the hull, return success status
  vessel->devices_hull.push_back(this);
  status = statustype::ON_HULL;
  return true;
}

bool attitude_cmg::attach_cabin() {
  /// Attempt to attach this device to a wall of the cabin, return success status
  vessel->devices_hull.push_back(this);
  status = statustype::IN_CABIN;
  return true;
}

void attitude_cmg::remove_panel() {
  /// Remove this instrument from its panel only (leave it attached to any ship)
  if(!vessel) {
    std::cout << "ERROR: tried to remove hull device " << get_name() << " which is already not attached to a vessel." << std::endl;
    return;
  }
  panel->devices.remove(this);
  status = statustype::UNMOUNTED;
}

void attitude_cmg::remove_hull() {
  /// Remove this device from the hull only (leave it attached to any ship)
  /// Note: not safe to be called in an iteration of hull devices!
  if(!vessel) {
    std::cout << "ERROR: tried to remove hull device " << get_name() << " which is already not attached to a vessel." << std::endl;
    return;
  }
  vessel->devices_hull.remove(this);
  status = statustype::UNMOUNTED;
}

void attitude_cmg::remove_cabin() {
  /// Remove this device from the cabin only (leave it attached to any ship)
  if(!vessel) {
    std::cout << "ERROR: tried to remove hull device " << get_name() << " which is already not attached to a vessel." << std::endl;
    return;
  }
  vessel->devices_cabin.remove(this);
  status = statustype::UNMOUNTED;
}

unsigned int attitude_cmg::get_port_in_count() {
  /// How many input ports it has
  return 3;
}

std::string attitude_cmg::get_port_in_name(unsigned int port) {
  /// Name of the input port numbered n
  switch(port) {
  case 0:
    return "yaw";
  case 1:
    return "pitch";
  case 2:
    return "roll";
  default:
    return "";
  }
}

std::string attitude_cmg::get_port_in_description(unsigned int port) {
  /// Description of the input port numbered n
  switch(port) {
  case 0:
    {
      std::stringstream desc;
      desc << "The desired yaw rate, in degrees per second, positive or negative."
              "  " << get_port_in_connstatus(port);
      return desc.str();
    }
  case 1:
    {
      std::stringstream desc;
      desc << "The desired pitch rate, in degrees per second, positive or negative."
              "  " << get_port_in_connstatus(port);
      return desc.str();
    }
  case 2:
    {
      std::stringstream desc;
      desc << "The desired roll rate, in degrees per second, positive or negative."
              "  " << get_port_in_connstatus(port);
      return desc.str();
    }
  default:
    return "";
  }
}

bool attitude_cmg::get_port_in_required(unsigned int port __attribute__((__unused__))) {
  /// Whether an input on this port is necessary for this device to operate
  // all ports are optional
  return false;
}

unsigned int attitude_cmg::get_port_out_count() {
  /// How many output ports it has
  return 3;
}

std::string attitude_cmg::get_port_out_name(unsigned int port) {
  /// Name of the output port numbered n
  switch(port) {
  case 0:
    return "yaw rate";
  case 1:
    return "pitch rate";
  case 2:
    return "roll rate";
  default:
    return "";
  }
}

std::string attitude_cmg::get_port_out_description(unsigned int port) {
  /// Description of the output port numbered n
  // see https://en.wikipedia.org/wiki/TR-201
  switch(port) {
  case 0:
    return "The current rate of yaw of the spacecraft, in degrees per second.";
  case 1:
    return "The current rate of pitch of the spacecraft, in degrees per second.";
  case 2:
    return "The current rate of roll of the spacecraft, in degrees per second.";
  default:
    return "";
  }
}

double attitude_cmg::get_port_out_data(unsigned int port) {
  /// Query the data on the specified out port
  switch(port) {
  case 0:
    // TODO
    return 0.0;
  case 1:
    // TODO
    return 0.0;
  case 2:
    // TODO
    return 0.0;
  default:
    return 0.0;
  }
}
