#include "switch_onoff.h"

switch_onoff::switch_onoff()
  : setting(0) {
  /// Default constructor
  ports_in.resize(get_port_in_count());                                         // anything with input ports needs this
}

switch_onoff::~switch_onoff() {
  /// Default destructor
}

std::string switch_onoff::get_name() {
  /// Return the name of this device
  return "on/off switch";
}

std::string switch_onoff::get_model() {
  /// Return a model name for this device
  random_reset();
  std::stringstream model;
  model << "SW" << get_random_int(10, 90);
  return model.str();
}

std::string switch_onoff::get_description() {
  /// Return a detailed description of this device
  return "A simple but durable two setting switch.  "
         "By default it switches between outputting values of 0 and 1, but can be configured to output any pair of values.";
}

double switch_onoff::get_mass() {
  /// Return the weight of the device, in kilograms
  return 0.05;
}

Vector3d switch_onoff::get_size() {
  /// Return a size for this object, in metres - hardcoded
  return Vector3d(0.02, 0.02, 0.004);
}

unsigned int switch_onoff::get_port_in_count() {
  /// How many input ports it has
  return 2;
}

std::string switch_onoff::get_port_in_name(unsigned int port) {
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

std::string switch_onoff::get_port_in_description(unsigned int port) {
  /// Description of the input port numbered n
  switch(port) {
  case 0:
    {
      std::stringstream desc;
      desc << "What to output when set to \"off\" position.  Defaults to 0 if not connected."
              "  " << get_port_in_connstatus(port);
      return desc.str();
    }
  case 1:
    {
      std::stringstream desc;
      desc << "What to output when set to \"on\" position.  Defaults to 1 if not connected."
              "  " << get_port_in_connstatus(port);
      return desc.str();
    }
  default:
    return "";
  }
}

bool switch_onoff::get_port_in_required(unsigned int port __attribute__((__unused__))) {
  /// Whether an input on this port is necessary for this device to operate
  // all inputs are optional
  return false;
}

unsigned int switch_onoff::get_port_out_count() {
  /// How many output ports it has
  return 1;
}

std::string switch_onoff::get_port_out_name(unsigned int port __attribute__((__unused__))) {
  /// Name of the output port numbered n
  return "output";
}

std::string switch_onoff::get_port_out_description(unsigned int port  __attribute__((__unused__))) {
  /// Description of the output port numbered n
  return "A numerical value of 0 or 1, or a passthrough for whatever input is connected.  Passthrough values can be any data type, not just numerical.";
}

double switch_onoff::get_port_out_data(unsigned int port __attribute__((__unused__))) {
  /// Query the value data on the specified out port
  if(ports_in[setting].target) {
    return ports_in[setting].target->get_port_out_data(ports_in[setting].target_port);
  } else {
    return static_cast<double>(setting);
  }
}

std::string switch_onoff::get_port_out_text(unsigned int port __attribute__((__unused__))) {
  /// Query the text data on the specified out port
  if(ports_in[setting].target) {
    return ports_in[setting].target->get_port_out_text(ports_in[setting].target_port);
  } else {
    if(setting == 0) {
      return "off";
    } else {
      return "on";
    }
  }
}

GLuint switch_onoff::get_port_out_video_analogue(unsigned int port __attribute__((__unused__))) {
  /// Query the analogue video data on the specified out port - returns a texture id
  if(ports_in[setting].target) {
    return ports_in[setting].target->get_port_out_video_analogue(ports_in[setting].target_port);
  } else {
    if(setting == 0) {
      return 0;
    } else {
      return generate_static_analogue();
    }
  }
}

GLuint switch_onoff::get_port_out_video_digital(unsigned int port __attribute__((__unused__))) {
  /// Query the digital video data on the specified out port - returns a texture id
  if(ports_in[setting].target) {
    return ports_in[setting].target->get_port_out_video_digital(ports_in[setting].target_port);
  } else {
    if(setting == 0) {
      return 0;
    } else {
      return generate_static_digital();
    }
  }
}

void switch_onoff::get_port_out_sound(unsigned int port __attribute__((__unused__))) {
  /// Query the audio data on the specified out port
  if(ports_in[setting].target) {
    return ports_in[setting].target->get_port_out_sound(ports_in[setting].target_port);
  } else {
    if(setting == 0) {
      return;
    } else {
      return generate_static_sound();
    }
  }
}

void switch_onoff::render() {
  /// Render the switch in its appropriate position
  glPushMatrix();

  glTranslated(position.x,
               position.y,
               position.z);

  Vector3d thissize = get_size();
  double switch_tip_y;
  double normal_flip;
  if(setting == 0) {
    switch_tip_y = 0.0;
    //switch_tip_y = -thissize.y;
    normal_flip = -1.0;
  } else {
    switch_tip_y = thissize.y;
    //switch_tip_y = (thissize.y * 2.0);
    normal_flip = 1.0;
  }
  double const switch_tip_z = thissize.z + thissize.y;
  double const switch_arm_inset = 0.005;

  // switch body
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

  // switch arm
  glNormal3d(0.0, -0.70710678118 * normal_flip, -0.70710678118 * normal_flip);  // 45 degrees ~= 1/sqrt(2)
  glVertex3d(             switch_arm_inset, thissize.y / 2.0 - 0.002, thissize.z); // bottom surface
  glVertex3d(thissize.x - switch_arm_inset, thissize.y / 2.0 - 0.002, thissize.z);
  glVertex3d(thissize.x - switch_arm_inset, switch_tip_y            , switch_tip_z);
  glVertex3d(             switch_arm_inset, switch_tip_y            , switch_tip_z);
  glNormal3d(0.0,  0.70710678118 * normal_flip,  0.70710678118 * normal_flip);  // 45 degrees ~= 1/sqrt(2)
  glVertex3d(thissize.x - switch_arm_inset, thissize.y / 2.0 + 0.002, thissize.z); // top surface
  glVertex3d(             switch_arm_inset, thissize.y / 2.0 + 0.002, thissize.z);
  glVertex3d(             switch_arm_inset, switch_tip_y            , switch_tip_z);
  glVertex3d(thissize.x - switch_arm_inset, switch_tip_y            , switch_tip_z);
  glEnd();

  // switch arm sides
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, Vector4f(0.8, 0.8, 0.8, 1.0));
  glMaterialfv(GL_FRONT, GL_SPECULAR,            Vector4f(0.8, 0.8, 0.8, 1.0));
  glMaterialfv(GL_FRONT, GL_EMISSION,            Vector4f(0.0, 0.0, 0.0, 1.0));
  glMaterialf(GL_FRONT,  GL_SHININESS,           2.0);
  glBegin(GL_TRIANGLES);
  glNormal3d(-1.0, 0.0, 0.0);
  glVertex3d(             switch_arm_inset, thissize.y / 2.0 - 0.002, thissize.z); // left edge
  glVertex3d(             switch_arm_inset, switch_tip_y            , switch_tip_z);
  glVertex3d(             switch_arm_inset, thissize.y / 2.0 + 0.002, thissize.z);
  glNormal3d( 1.0, 0.0, 0.0);
  glVertex3d(thissize.x - switch_arm_inset, thissize.y / 2.0 + 0.002, thissize.z); // right edge
  glVertex3d(thissize.x - switch_arm_inset, switch_tip_y            , switch_tip_z);
  glVertex3d(thissize.x - switch_arm_inset, thissize.y / 2.0 - 0.002, thissize.z);
  glEnd();

  glPopMatrix();
}

void switch_onoff::activate() {
  /// Activate the current device, whatever that means for this particular one
  setting = (setting + 1) % get_port_in_count();
  std::cout << "New setting: " << setting << std::endl;
}
