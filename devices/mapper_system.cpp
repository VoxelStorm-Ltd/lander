#include "mapper_system.h"
#include "universe.h"
#include "starsystem.h"

extern universe root;

mapper_system::mapper_system()
  //: scale(0.00001),           // earth scale
  : scale(0.000000002),       // solar system scale
    trail_ref(nullptr) {
  /// Default constructor
  ports_in.resize(get_port_in_count());     // anything with input ports needs this
}

mapper_system::~mapper_system() {
  /// Default destructor
}


std::string mapper_system::get_name() {
  /// Return the name of this device
  return "star system mapper";
}

std::string mapper_system::get_description() {
  /// Return a detailed description of this device
  random_reset();
  std::stringstream desc;
  desc << get_random_name_corporation() << " model MAP-S" << get_random_int(100, 999) << "0a."
       << "  A mapping computer that keeps track of the orbital positions of all "
          "known bodies within the current star system, including planets, "
          "spacecraft, and all known radar and visual contacts."
          "  The display is orthographic."
          "  Capable of rotation and zoom."
          "  Displays object trails relative to a selectable reference frame.";
  return desc.str();
}

unsigned int mapper_system::get_port_in_count() {
  /// How many input ports it has
  return 4;
}

std::string mapper_system::get_port_in_name(unsigned int port) {
  /// Name of the input port numbered n
  switch(port) {
  case 0:
    return "scale";
  case 1:
    return "rotation around y";
  case 2:
    return "rotation around x";
  case 3:
    return "trail reference frame";
  default:
    return "";
  }
}

std::string mapper_system::get_port_in_description(unsigned int port) {
  /// Description of the input port numbered n
  switch(port) {
  case 0:
    {
      std::stringstream desc;
      desc << "How much to multiply the scale for display."
              "  " << get_port_in_connstatus(port);
      return desc.str();
    }
  case 1:
    {
      std::stringstream desc;
      desc << "Rotation of the display parallel to the ecliptic, in degrees."
              "  " << get_port_in_connstatus(port);
      return desc.str();
    }
  case 2:
    {
      std::stringstream desc;
      desc << "Tilt of the display relative to the ecliptic, in degrees."
              "  " << get_port_in_connstatus(port);
      return desc.str();
    }
  case 3:
    {
      std::stringstream desc;
      desc << "An integer value for which orbital object's reference frame to display trails relative to."
              "  0 is the centre of mass of the system, 1 is the first body out, etc."
              "  " << get_port_in_connstatus(port);
      return desc.str();
    }
  default:
    return "";
  }
}

bool mapper_system::get_port_in_required(unsigned int port) {
  /// Whether an input on this port is necessary for this device to operate
  // all inputs are optional
  return false;
}

unsigned int mapper_system::get_port_out_count() {
  /// How many output ports it has
  return 1;
}

std::string mapper_system::get_port_out_name(unsigned int port) {
  /// Name of the output port numbered n
  switch(port) {
  case 0:
    return "analogue video";
  default:
    return "";
  }
}

std::string mapper_system::get_port_out_description(unsigned int port) {
  /// Description of the output port numbered n
  switch(port) {
  case 0:
    return "An analogue video signal showing a diagram of the star system's layout.";
  default:
    return "";
  }
}

void mapper_system::get_port_out_video_analogue(unsigned int port __attribute__((__unused__))) {
  /// Render the star map on an analogue monitor
  // TODO
}


void mapper_system::update() {
  /// Update the output states and respond to changes in input
  // update the scale
  if(ports_in[0].target) {
    scale = ports_in[0].target->get_port_out_data(ports_in[0].target_port);
  } else {
    //double scale = 0.00001;           // earth scale
    double scale = 0.000000002;       // solar system scale
  }
  // update the rotation
  double rotation_y = 0.0;
  double rotation_x = 0.0;
  if(ports_in[1].target) {
    rotation_y = ports_in[2].target->get_port_out_data(ports_in[2].target_port);
  }
  if(ports_in[2].target) {
    rotation_x = ports_in[1].target->get_port_out_data(ports_in[1].target_port);
  }
  rotation = Matrix4f::createRotationAroundAxis(rotation_x, rotation_y, 0.0);
  // update the trail reference target
  if(ports_in[3].target) {
    double const ntarget_pre = ports_in[2].target->get_port_out_data(ports_in[2].target_port);
    // sanity checks
    if(ntarget_pre >= 1.0) {
      // iterate through the bodies to try to find the nth
      unsigned int n = 1;
      unsigned int const ntarget = static_cast<unsigned int>(ntarget_pre);
      for(auto const &it : root.currentsystem->bodies) {
        if(ntarget == n) {
          trail_ref = it;
          break;
        }
        ++n;
      }
    } else {
      trail_ref = nullptr;
    }
  } else {
    trail_ref = nullptr;
  }
}
