#include "button_momentary.h"

button_momentary::button_momentary()
  : pushed(false) {
  /// Default constructor
  ports_in.resize(get_port_in_count());                                         // anything with input ports needs this
}

button_momentary::~button_momentary() {
  /// Default destructor
}

std::string button_momentary::get_name() {
  /// Return the name of this device
  return "button";
}

std::string button_momentary::get_model() {
  /// Return a model name for this device
  return "PB-M01a";
}

std::string button_momentary::get_description() {
  /// Return a detailed description of this device
  return "A push-button with momentary activation  "
         "By default it outputs a value of 1 when pressed and 0 otherwise, but can be configured to output any pair of values.";
}

double button_momentary::get_mass() {
  /// Return the weight of the device, in kilograms
  return 0.05;
}

Vector3d button_momentary::get_size() {
  /// Return a size for this object, in metres - hardcoded
  return Vector3d(0.02, 0.02, 0.01);
}

unsigned int button_momentary::get_port_in_count() {
  /// How many input ports it has
  return 2;
}

std::string button_momentary::get_port_in_name(unsigned int port) {
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

std::string button_momentary::get_port_in_description(unsigned int port) {
  /// Description of the input port numbered n
  switch(port) {
  case 0:
    {
      std::stringstream desc;
      desc << "What to output when not pressed.  Defaults to 0 if not connected."
              "  " << get_port_in_connstatus(port);
      return desc.str();
    }
  case 1:
    {
      std::stringstream desc;
      desc << "What to output (once) when pressed.  Defaults to 1 if not connected."
              "  " << get_port_in_connstatus(port);
      return desc.str();
    }
  default:
    return "";
  }
}

bool button_momentary::get_port_in_required(unsigned int port __attribute__((__unused__))) {
  /// Whether an input on this port is necessary for this device to operate
  // all inputs are optional
  return false;
}

unsigned int button_momentary::get_port_out_count() {
  /// How many output ports it has
  return 1;
}

std::string button_momentary::get_port_out_name(unsigned int port __attribute__((__unused__))) {
  /// Name of the output port numbered n
  return "output";
}

std::string button_momentary::get_port_out_description(unsigned int port  __attribute__((__unused__))) {
  /// Description of the output port numbered n
  return "A numerical value of 0 or 1, or a passthrough for whatever input is connected.  "
         "Passthrough values can be any data type, not just numerical.  "
         "Once the output has been read once, it is reset.  "
         "If the button's value is not queried, it remains set indefinitely until queried, making this also usable for arm switches and the like.";
}

double button_momentary::get_port_out_data(unsigned int port __attribute__((__unused__))) {
  /// Query the value data on the specified out port
  if(pushed) {
    pushed = false;
    if(ports_in[1].target) {
      return ports_in[1].target->get_port_out_data(ports_in[1].target_port);
    } else {
      return 1.0;
    }
  } else {
    if(ports_in[0].target) {
      return ports_in[0].target->get_port_out_data(ports_in[0].target_port);
    } else {
      return 0.0;
    }
  }
}

std::string button_momentary::get_port_out_text(unsigned int port __attribute__((__unused__))) {
  /// Query the text data on the specified out port
  if(pushed) {
    pushed = false;
    if(ports_in[1].target) {
      return ports_in[1].target->get_port_out_text(ports_in[1].target_port);
    } else {
      return "on";
    }
  } else {
    if(ports_in[0].target) {
      return ports_in[0].target->get_port_out_text(ports_in[0].target_port);
    } else {
      return "off";
    }
  }
}

GLuint button_momentary::get_port_out_video_analogue(unsigned int port __attribute__((__unused__))) {
  /// Query the analogue video data on the specified out port - returns a texture id
  if(pushed) {
    pushed = false;
    if(ports_in[1].target) {
      return ports_in[1].target->get_port_out_video_analogue(ports_in[1].target_port);
    } else {
      return generate_static_analogue();
    }
  } else {
    if(ports_in[0].target) {
      return ports_in[0].target->get_port_out_video_analogue(ports_in[0].target_port);
    } else {
      return 0;
    }
  }
}

GLuint button_momentary::get_port_out_video_digital(unsigned int port __attribute__((__unused__))) {
  /// Query the digital video data on the specified out port - returns a texture id
  if(pushed) {
    pushed = false;
    if(ports_in[1].target) {
      return ports_in[1].target->get_port_out_video_digital(ports_in[1].target_port);
    } else {
      return generate_static_digital();
    }
  } else {
    if(ports_in[0].target) {
      return ports_in[0].target->get_port_out_video_digital(ports_in[0].target_port);
    } else {
      return 0;
    }
  }
}

void button_momentary::get_port_out_sound(unsigned int port __attribute__((__unused__))) {
  /// Query the audio data on the specified out port
  if(pushed) {
    pushed = false;
    if(ports_in[1].target) {
      return ports_in[1].target->get_port_out_sound(ports_in[1].target_port);
    } else {
      return generate_static_sound();
    }
  } else {
    if(ports_in[0].target) {
      return ports_in[0].target->get_port_out_sound(ports_in[0].target_port);
    } else {
      return;
    }
  }
}

void button_momentary::render() {
  /// Render the switch in its appropriate position
  glPushMatrix();

  glTranslated(position.x,
               position.y,
               position.z);

  Vector3d thissize = get_size();
  if(pushed) {
    // pressing it animation
    thissize.z = 0.002;
  }

  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, Vector4f(0.2, 0.2, 0.2, 1.0));
  glMaterialfv(GL_FRONT, GL_SPECULAR,            Vector4f(0.2, 0.2, 0.2, 1.0));
  glMaterialfv(GL_FRONT, GL_EMISSION,            Vector4f(0.0, 0.0, 0.0, 1.0));
  glMaterialf(GL_FRONT,  GL_SHININESS,           2.0);                          // 0 to 127
  glBegin(GL_QUADS);
  // front
  glNormal3d(0.0, 0.0, 1.0);
  glVertex3d(0.0,        0.0,        thissize.z);
  glVertex3d(thissize.x, 0.0,        thissize.z);
  glVertex3d(thissize.x, thissize.y, thissize.z);
  glVertex3d(0.0,        thissize.y, thissize.z);
  // top
  glNormal3d(0.0, 1.0, 0.0);
  glVertex3d(0.0,        thissize.y, 0.0);
  glVertex3d(0.0,        thissize.y, thissize.z);
  glVertex3d(thissize.x, thissize.y, thissize.z);
  glVertex3d(thissize.x, thissize.y, 0.0);
  // bottom
  glNormal3d(0.0, -1.0, 0.0);
  glVertex3d(0.0,        0.0,        0.0);
  glVertex3d(thissize.x, 0.0,        0.0);
  glVertex3d(thissize.x, 0.0,        thissize.z);
  glVertex3d(0.0,        0.0,        thissize.z);
  // right
  glNormal3d(1.0, 0.0, 0.0);
  glVertex3d(thissize.x, 0.0,        0.0);
  glVertex3d(thissize.x, thissize.y, 0.0);
  glVertex3d(thissize.x, thissize.y, thissize.z);
  glVertex3d(thissize.x, 0.0,        thissize.z);
  // left
  glNormal3d(-1.0, 0.0, 0.0);
  glVertex3d(0.0,        0.0,        0.0);
  glVertex3d(0.0,        0.0,        thissize.z);
  glVertex3d(0.0,        thissize.y, thissize.z);
  glVertex3d(0.0,        thissize.y, 0.0);
  glEnd();

  glPopMatrix();
}

void button_momentary::activate() {
  /// The time has come to... push the button
  //pushed = true;
  pushed = !pushed;                                                             // allow un-pushing unread buttons
}
