#include "display_small.h"

display_small::display_small() {
  /// Default constructor
  update_vbo();                             // anything with a custom model needs this
}

display_small::~display_small() {
  /// Default destructor
}

std::string display_small::get_name() {
  /// Return the name of this device
  return "small graphical display";
}

std::string display_small::get_model() {
  /// Return a model name for this device
  random_reset();
  unsigned int const modelnum = get_random_int(1, 9);
  std::stringstream model;
  model << "CRT0" << modelnum << modelnum << "-S";
  return model.str();
}

std::string display_small::get_description() {
  /// Return a detailed description of this device
  return "A small analogue CRT monitor, as used in cockpit displays in the early years of the space race."
         "  Capable of withstanding a lot of punishment, both physical and electromagnetic."
         "  Technically capable of displaying any colour, but for some reason tends to tint everything with a sickly greenish hue."
         "  Slightly increases the pilot's overall radiation exposure.";
}

double display_small::get_mass() {
  /// Return the weight of the device, in kilograms
  return 4.0;
}

Vector3d display_small::get_size() {
  /// Return a size for this object, in metres - hardcoded
  return Vector3d(0.1, 0.1, 0.02);
}
