#include "telltale.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <FTGL/ftgl.h>

extern FTFont *font_title;          // global font definitions
extern FTFont *font_text;

telltale::telltale() {
  /// Default constructor
  ports_in.resize(get_port_in_count());     // anything with input ports needs this
}

telltale::~telltale() {
  /// Default destructor
}

std::string telltale::get_name() {
  /// Return the name of this device
  return "telltale";
}

std::string telltale::get_model() {
  /// Return a model name for this device
  return "TT-G568-10-70D";
}

std::string telltale::get_description() {
  /// Return a detailed description of this device
  return "A green telltale type warning or status indicator.  Outputs light at 10 millicandelas, wavelength 568nm, stable up to 70 degrees C.";
}

double telltale::get_mass() {
  /// Return the weight of the device, in kilograms
  return 0.010;
}

Vector3d telltale::get_size() {
  /// Return a size for this object, in metres - hardcoded
  return Vector3d(0.04, 0.02, 0.002);
}

unsigned int telltale::get_port_in_count() {
  /// How many input ports it has
  return 2;
}

std::string telltale::get_port_in_name(unsigned int port) {
  /// Name of the input port numbered n
  switch(port) {
  case 0:
    return "input value";
  case 1:
    return "display text";
  default:
    return "";
  }
}

std::string telltale::get_port_in_description(unsigned int port) {
  /// Description of the input port numbered n
  switch(port) {
  case 0:
    {
      std::stringstream desc;
      desc << "Input data value.  Will light up if the input is any value greater than 0."
              "  " << get_port_in_connstatus(port);
      return desc.str();
    }
  case 1:
    {
      std::stringstream desc;
      desc << "The text to display on the telltale, or \"ON\" if nothing is connected."
              "  " << get_port_in_connstatus(port);
      return desc.str();
    }
  default:
    return "";
  }
}

bool telltale::get_port_in_required(unsigned int port) {
  /// Whether an input on this port is necessary for this device to operate
  if(port == 0) {
    return true;
  } else {
    return false;
  }
}

void telltale::render() {
  /// Render the switch in its appropriate position
  glPushMatrix();

  glTranslated(position.x,
               position.y,
               position.z);

  Vector3d thissize = get_size();

  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, Vector4f(0.0, 0.2, 0.0, 1.0));
  glMaterialfv(GL_FRONT, GL_SPECULAR,            Vector4f(0.0, 0.2, 0.0, 1.0));
  glMaterialf(GL_FRONT,  GL_SHININESS,           20.0);                           // 0 to 127
  if(ports_in[0].target && ports_in[0].target->get_port_out_data(ports_in[0].target_port) > 0.0) {
    glMaterialfv(GL_FRONT, GL_EMISSION, Vector4f(0.0, 0.8, 0.0, 1.0));
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

  // text label
  std::string thislabel;
  if(ports_in[1].target) {
    thislabel = ports_in[1].target->get_port_out_text(ports_in[1].target_port);
  } else {
    thislabel = "ON";
  }
  if(!thislabel.empty()) {    // don't call the draw routine if there's a blank string
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, Vector4f(0.0, 0.0, 0.0, 1.0));
    glMaterialfv(GL_FRONT, GL_SPECULAR,            Vector4f(0.0, 0.0, 0.0, 1.0));
    glMaterialfv(GL_FRONT, GL_EMISSION,            Vector4f(0.0, 0.0, 0.0, 1.0));
    glMaterialf(GL_FRONT,  GL_SHININESS,           2.0);                           // 0 to 127
    double const labellength = font_text->Advance(thislabel.c_str(), -1);
    double const scale = (thissize.x - 0.002) / labellength;                // automatic scaling to fit in the space
    glPushMatrix();
    glTranslated(0.001, 0.001, thissize.z + 0.001);
    glScaled(scale, scale, scale);
    font_text->Render(thislabel.c_str(), -1, FTPoint(), FTPoint(), FTGL::RENDER_FRONT);
    glPopMatrix();
  }

  glPopMatrix();
}
