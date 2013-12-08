#include "memory.h"

memory::memory()
  : memory_value(0.0) {
  /// Default constructor
  ports_in.resize(get_port_in_count());     // anything with input ports needs this
  random_reset();
  // set up initial values for outputs
  std::stringstream ss;
  ss << "Thank you for purchasing the " << get_manufacturer() << " " << get_model() << "."
        "  Connect input 1 to any device to format and start recording."
        "  Serial #"
     << get_random_uint(1000, 1000) << "-"
     << get_random_uint(1000, 1000) << "-"
     << get_random_uint(1000, 1000) << "-"
     << get_random_uint(1000, 1000);
  memory_text = ss.str();
}

memory::~memory() {
  /// Default destructor
}

std::string memory::get_name() {
  /// Return the name of this device
  return "memory module";
}

std::string memory::get_model() {
  /// Return a model name for this device
  random_reset();
  unsigned int const random_option = get_random_uint(0, 10);
  std::stringstream model;
  switch(random_option) {
  case 0:
    model << "SmartMem-" << get_random_uint(10, 99) << "L";
    break;
  case 1:
    model << "MemoryStar-" << get_random_char_alpha_upper() << get_random_char_alpha_upper() << get_random_uint(100, 999);
    break;
  case 2:
    model << "IntelliMem " << get_random_uint(1, 9) << "000";
    break;
  case 3:
    model << "RemOmniMember " << get_random_uint(1, 9);
    break;
  case 4:
    model << "DuraMem-" << get_random_uint(1, 9999) << get_random_char_alpha_upper();
    break;
  case 5:
    model << "SecureMem " << get_random_uint(2, 20) << ".0";
    break;
  case 6:
    model << "ImpervaMem " << get_random_uint(1, 9) << "00+";
    break;
  case 7:
    model << "PermaMem-" << get_random_uint(1, 9999) << get_random_char_alpha_upper();
    break;
  case 8:
    model << "DuraMem " << get_random_char_alpha_upper() << get_random_uint(40, 49) << get_random_char_alpha_upper();
    break;
  case 9:
    model << "MemVery V." << get_random_uint(2, 20) << ".0";
    break;
  case 10:
    model << "MEM" << get_random_uint(100, 999) << "0";
    break;
  }
  return model.str();
}

std::string memory::get_description() {
  /// Return a detailed description of this device
  return "An intelligent memory chip capable of retreiving and storing a "
         "snapshot of data from any device it's connected to, and outputting "
         "the same data, regardless of format, to any device connected to it."
         "  Engineered to survive in a variety of extreme environments, such "
         "chips are designed for maximum durability and are often the only "
         "functional systems after a serious accident.  As a result, they have "
         "taken on the role of black box recorder, although they can also be "
         "used in delay loops, or just to send a fixed value to an input, such "
         "is their flexibility.";
}

double memory::get_mass() {
  /// Return the weight of the device, in kilograms
  return 0.0085;    // 8.5g
}

Vector3d memory::get_size() {
  /// Return a size for this object, in metres - hardcoded
  return Vector3d(0.02, 0.005, 0.04);    // thumbstick shape...ish
}

unsigned int memory::get_port_in_count() {
  /// How many input ports it has
  return 1;
}

std::string memory::get_port_in_name(unsigned int port __attribute__((__unused__))) {
  /// Name of the input port numbered n
  return "input value";
}

std::string memory::get_port_in_description(unsigned int port __attribute__((__unused__))) {
  /// Description of the input port numbered n
  std::stringstream desc;
  desc << "An input value to remember.  Will retain value in memory after disconnection."
          "  " << get_port_in_connstatus(port);
  return desc.str();
}

bool memory::get_port_in_required(unsigned int port __attribute__((__unused__))) {
  /// Whether an input on this port is necessary for this device to operate
  // all inputs are optional
  return false;
}

unsigned int memory::get_port_out_count() {
  /// How many output ports it has
  return 1;
}

std::string memory::get_port_out_name(unsigned int port __attribute__((__unused__))) {
  /// Name of the output port numbered n
  return "remembered data";
}

std::string memory::get_port_out_description(unsigned int port __attribute__((__unused__))) {
  /// Description of the output port numbered n
  return "The last input value this memory module received - either numerical, text or still image.";
}


double memory::get_port_out_data(unsigned int port __attribute__((__unused__))) {
  /// Query the value data on the specified out port
  return memory_value;
}

std::string memory::get_port_out_text(unsigned int port __attribute__((__unused__))) {
  /// Query the text data on the specified out port
  return memory_text;
}

GLuint memory::get_port_out_video_analogue(unsigned int port __attribute__((__unused__))) {
  /// Query the analogue video data on the specified out port
  return memory_image;
}

GLuint memory::get_port_out_video_digital(unsigned int port __attribute__((__unused__))) {
  /// Query the digital video data on the specified out port
  return memory_image;
}

void memory::get_port_out_sound(unsigned int port __attribute__((__unused__))) {
  /// Query the audio data on the specified out port
  // TODO
}

void memory::update() {
  /// Pull the relevant inputs into our memory
  if(!ports_in[0].target) {
    // not connected - don't change any values
    return;
  }
  memory_value = ports_in[0].target->get_port_out_data(ports_in[0].target_port);
  memory_text  = ports_in[0].target->get_port_out_text(ports_in[0].target_port);
  // todo: sort out image and sound recordings
}

void memory::set_memory_value(double newvalue) {
  /// Write the memory numerical value arbitrarily
  memory_value = newvalue;
}

void memory::set_memory_text(std::string const &newstring) {
  /// Write the memory text arbitrarily
  memory_text = newstring;
}
