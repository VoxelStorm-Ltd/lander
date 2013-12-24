#include "led_green.h"

led_green::led_green() {
  /// Default constructor
  ports_in.resize(get_port_in_count());     // anything with input ports needs this
}

led_green::~led_green() {
  /// Default destructor
}

std::string led_green::get_name() {
  /// Return the name of this device
  return "green LED";
}

std::string led_green::get_model() {
  /// Return a model name for this device
  return "LED-G568-10-70D";
}

std::string led_green::get_description() {
  /// Return a detailed description of this device
  // based loosely on http://www.mouser.com/ds/2/216/WP4060GD-67585.pdf
  return "A simple green light-emitting diode.  Outputs light at 10 millicandelas, wavelength 568nm, stable up to 70 degrees C.";
}

double led_green::get_mass() {
  /// Return the weight of the device, in kilograms
  return 0.0012;
}

Vector3d led_green::get_size() {
  /// Return a size for this object, in metres - hardcoded
  return Vector3d(0.01, 0.01, 0.002);
}

unsigned int led_green::get_port_in_count() {
  /// How many input ports it has
  return 1;
}

std::string led_green::get_port_in_name(unsigned int port __attribute__((__unused__))) {
  /// Name of the input port numbered n
  return "input value";
}

std::string led_green::get_port_in_description(unsigned int port __attribute__((__unused__))) {
  /// Description of the input port numbered n
  std::stringstream desc;
  desc << "Input data value.  Will light up if the input is any value greater than 0."
          "  " << get_port_in_connstatus(port);
  return desc.str();
}

bool led_green::get_port_in_required(unsigned int port __attribute__((__unused__))) {
  /// Whether an input on this port is necessary for this device to operate
  // all inputs are mandatory
  return true;
}

void led_green::render() {
  /// Render the switch in its appropriate position
  glPushMatrix();

  glTranslated(position.x,
               position.y,
               position.z);

  Vector3d thissize = get_size();

  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, Vector4f(0.0, 0.2, 0.0, 1.0));
  glMaterialfv(GL_FRONT, GL_SPECULAR,            Vector4f(0.0, 0.2, 0.0, 1.0));
  glMaterialf(GL_FRONT,  GL_SHININESS,           2.0);                           // 0 to 127
  if(ports_in[0].target && ports_in[0].target->get_port_out_data(ports_in[0].target_port) > 0.0) {
    glMaterialfv(GL_FRONT, GL_EMISSION, Vector4f(0.0, 1.0, 0.0, 1.0));
  } else {
    glMaterialfv(GL_FRONT, GL_EMISSION, Vector4f(0.0, 0.0, 0.0, 1.0));
  }
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
