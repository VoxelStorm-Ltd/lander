#include "computer_mission.h"

computer_mission::computer_mission() {
  /// Default constructor
  ports_in.resize(get_port_in_count());                                         // anything with input ports needs this
  update_vbo();                                                                 // every device with a custom size needs this

  boot();                                                                       // run bootup sequence
}

computer_mission::~computer_mission() {
  /// Default destructor
}

std::string computer_mission::get_name() {
  /// Return the name of this device
  return "mission control computer";
}

std::string computer_mission::get_model() {
  /// Return a model name for this device
  return "MCC-PAL9000";
}

std::string computer_mission::get_description() {
  /// Return a detailed description of this device
  return "A computer the purpose of which is to keep track of the vessel's current "
         "status and objectives, log events, assist in mission planning, monitor "
         "risks and dangers, and suggest solutions to problems.";
}

double computer_mission::get_mass() {
  /// Return the weight of the device, in kilograms
  return 10.0;
}

vector3d computer_mission::get_size() {
  /// Return a size for this object, in metres - hardcoded
  return vector3d(0.2, 0.2, 0.2);
}

unsigned int computer_mission::get_port_in_count() {
  /// How many input ports it has
  return 1;
}

std::string computer_mission::get_port_in_name(unsigned int port) {
  /// Name of the input port numbered n
  switch(port) {
  case 0:
    return "input text";
  default:
    return "";
  }
}

std::string computer_mission::get_port_in_description(unsigned int port) {
  /// Description of the input port numbered n
  switch(port) {
  case 0:
    {
      std::stringstream desc;
      desc << "A text source for interactive input to the computer, such as directly from a keyboard or line-buffered via a terminal."
              "  " << get_port_in_connstatus(port);
      return desc.str();
    }
  default:
    return "";
  }
}

bool computer_mission::get_port_in_required(unsigned int port __attribute__((__unused__))) {
  /// Whether an input on this port is necessary for this device to operate
  // all ports are optional
  return false;
}

unsigned int computer_mission::get_port_out_count() {
  /// How many output ports it has
  return 1;
}

std::string computer_mission::get_port_out_name(unsigned int port) {
  /// Name of the output port numbered n
  switch(port) {
  case 0:
    return "text output";
  default:
    return "";
  }
}

std::string computer_mission::get_port_out_description(unsigned int port) {
  /// Description of the output port numbered n
  switch(port) {
  case 0:
    return "The output of the computer, in text format.  Can be displayed on a monitor with the use of a terminal.";
  default:
    return "";
  }
}

std::string computer_mission::get_port_out_text(unsigned int port) {
  /// Query the text data on the specified out port
  if(port == 0) {
    if(!buffer.empty()) {
      std::string const lastline = buffer.front();
      buffer.pop();
      return lastline;
    } else {
      return "";
    }
  } else {
    return "";
  }
}

void computer_mission::boot() {
  // bootup messages
  std::stringstream ss;
  ss << get_manufacturer() << " " << get_model() << " Mission Control Computer";
  buffer.push(ss.str());
  buffer.push("Revision 1.794 built 13 Apr 2024");
  buffer.push(" ");
  buffer.push("Main Processor : Pentium VIII 14.4GHz");
  buffer.push("Memory Testing : 1073741824K OK + 1024K Shared Memory");
  buffer.push(" ");
  buffer.push("Reward Plug and Play BIOS Extension v3.0A");
  buffer.push("Copyright (C) 2021, Reward Software, Inc.");
  buffer.push(" ");
  buffer.push("Fad ChipAwayVirus(R) On Guard Ver 1.72");
  buffer.push(" ");
  buffer.push("IDF Channel 0 Master  : WDC WD320000BJS-00RYA0 12.01B01");
  buffer.push("IDF Channel 0 Slave   : none");
  buffer.push("IDF Channel 1 Master  : WDC WD320000BJS-00RYA0 12.01B01");
  buffer.push("IDF Channel 1 Slave   : none");
  buffer.push("IDF Channel 2 Master  : none");
  buffer.push("IDF Channel 2 Slave   : none");
  buffer.push("IDF Channel 3 Master  : none");
  buffer.push("IDF Channel 3 Slave   : none");
  buffer.push(" ");
  buffer.push("WARNING: The computer did not shut down cleanly.");
  buffer.push("Reconstructing cache metadata.");
  buffer.push("Please do not interrupt this process.");
  buffer.push("Reading cached metadata into memory...Done");
  buffer.push("Processing Mission Log record.........Done");
  buffer.push(" ");
  buffer.push("ERROR: Missing mission log data.  Running self-diagnostics.");
  buffer.push("Checking CPU consistency................OK");
  buffer.push("Checking memory parity..................OK");
  buffer.push("Testing ROM data consistency............OK");
  buffer.push("Testing RAM data consistency............OK");
  buffer.push("Testing drive data consistency........FAIL");
  buffer.push("Measuring bus bandwidths................OK");
  buffer.push("Scanning peripheral devices.............OK");
  buffer.push("Testing power supply....................OK");
  buffer.push(" ");
  buffer.push("ERROR: One or more self-tests failed.  Press any key to start manual recovery.");
}
