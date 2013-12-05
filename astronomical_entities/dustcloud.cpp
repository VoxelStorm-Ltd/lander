#include "dustcloud.h"

dustcloud::dustcloud() {
  /// Default constructor
}

dustcloud::~dustcloud() {
  /// Default destructor
}

std::string dustcloud::get_description() {
  if(description.size() != 0) {
    return description;
  } else {
    return "A sparse cloud of fine dust, composed of random elements.  Perhaps it used to be something, once...";
  }
}
