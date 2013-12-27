#include "control.h"

// static members:
std::vector<control::boundlist> control::keys;
std::vector<control::boundlist> control::mousebuttons;
std::vector<control::boundlist> control::joybuttons;

control::control()
  : sensitivity(1.0),
    deadzone(0.0),
    range(100.0),
    boundto(boundtype::UNBOUND) {
  /// Default constructor
  // initialise the statics only once
  if(keys.empty()) {
    keys.resize(maxkeys);
    mousebuttons.resize(maxmousebuttons);
    joybuttons.resize(maxjoybuttons);
  }
}

control::~control() {
  /// Default destructor
}

void control::unbind() {
  /// Unbind this control device from any input devices it may be bound to
  // search through anything it may have been attached to and remove all instances
  switch(boundto) {
  case boundtype::UNBOUND:
    return;
  case boundtype::KEYBOARD:
    for(auto &it : keys) {
      it.remove(this);
    }
    break;
  case boundtype::MOUSEBUTTON:
    for(auto &it : mousebuttons) {
      it.remove(this);
    }
    break;
  case boundtype::MOUSEAXIS_X:
    break;
  case boundtype::MOUSEAXIS_Y:
    break;
  case boundtype::MOUSEWHEEL:
    break;
  case boundtype::MOUSEWHEEL_X:
    break;
  case boundtype::JOYBUTTON:
    for(auto &it : joybuttons) {
      it.remove(this);
    }
    break;
  case boundtype::JOYAXIS:
    break;
  }
  boundto = boundtype::UNBOUND;
}

void control::bind_key(unsigned int key) {
  /// Bind this control to a keyboard key
  if(key >= maxkeys) {
    std::cout << "ERROR: trying to bind key " << key << " which is numbered higher than the max keys value " << maxkeys << ".  Please report this error!" << std::endl;
    return;
  }
  unbind();
  boundto = boundtype::KEYBOARD;
  keys[key].push_back(this);
}

void control::bind_mousebutton(unsigned int button) {
  /// Bind this control to a mouse button
  if(button >= maxmousebuttons) {
    std::cout << "ERROR: trying to bind button " << button << " which is numbered higher than the max mouse buttons value " << maxmousebuttons << ".  Please report this error!" << std::endl;
    return;
  }
  unbind();
  boundto = boundtype::MOUSEBUTTON;
  mousebuttons[button].push_back(this);
}

void control::bind_mouseaxis_x(double new_sensitivity) {
  /// Bind this control to the horizontal mouse axis
  unbind();
  sensitivity = new_sensitivity;
  boundto = boundtype::MOUSEAXIS_X;
}

void control::bind_mouseaxis_y(double new_sensitivity) {
  /// Bind this control to the horizontal mouse axis
  unbind();
  sensitivity = new_sensitivity;
  boundto = boundtype::MOUSEAXIS_Y;
}

void control::bind_mousewheel(double new_sensitivity) {
  /// Bind this control to the mouse wheel
  unbind();
  sensitivity = new_sensitivity;
  boundto = boundtype::MOUSEWHEEL;
}

void control::bind_mousewheel_x(double new_sensitivity) {
  /// Bind this control to the horizontal scroll of the mousewheel
  unbind();
  sensitivity = new_sensitivity;
  boundto = boundtype::MOUSEWHEEL_X;
}

void control::bind_joybutton(unsigned int button) {
  /// Bind this control to a joystick or gamepad button
  if(button >= maxjoybuttons) {
    std::cout << "ERROR: trying to bind button " << button << " which is numbered higher than the max joystick buttons value " << maxjoybuttons << ".  Please report this error!" << std::endl;
    return;
  }
  unbind();
  boundto = boundtype::JOYBUTTON;
  joybuttons[button].push_back(this);
}

void control::bind_joyaxis(unsigned int axis,
                           double new_sensitivity,
                           double new_deadzone,
                           double new_range) {
  /// Bind this control to a joystick, gamepad or other controller axis
  unbind();
  sensitivity = new_sensitivity;
  deadzone = new_deadzone;
  range = new_range;
  boundto = boundtype::JOYAXIS;
}

void control::test_key(unsigned int key) {
  /// Activate the devices associated with this key
  for(auto const &it : keys[key]) {
    it->activate();
  }
}

void control::test_mousebutton(unsigned int button) {
  for(auto const &it : mousebuttons[button]) {
    it->activate();
  }
}

void control::test_mouseaxis_x(int value) {
  // TODO
}

void control::test_mouseaxis_y(int value) {
  // TODO
}

void control::test_mousewheel(int value) {
  // TODO
}

void control::test_mousewheel_x(int value) {
  // TODO
}

void control::test_joybutton(unsigned int button) {
  for(auto const &it : joybuttons[button]) {
    it->activate();
  }
}

void control::test_joyaxis(unsigned int axis, double value) {
  // TODO
}
