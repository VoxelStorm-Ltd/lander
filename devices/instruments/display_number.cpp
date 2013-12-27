#include "display_number.h"
#include <sstream>
#include "boost/format.hpp"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <FTGL/ftgl.h>

extern FTFont *font_7segment3d;

display_number::display_number() {
  /// Default constructor
  ports_in.resize(get_port_in_count());     // anything with input ports needs this
}

display_number::~display_number() {
  /// Default destructor
}

std::string display_number::get_name() {
  /// Return the name of this device
  return "7-segment numerical display";
}

std::string display_number::get_model() {
  /// Return a model name for this device
  return "ARK SR420561K";
}

std::string display_number::get_description() {
  /// Return a detailed description of this device
  return "An 8-character seven-segment numerical display with separate in-line decimal point.  "
         "Capable of displaying floating point numbers.  "
         "The segments are lit by green LEDs glowing at around 4 millicandelas with a wavelength of 568nm.";
}

double display_number::get_mass() {
  /// Return the weight of the device, in kilograms
  return 0.02;
}

Vector3d display_number::get_size() {
  /// Return a size for this object, in metres - hardcoded
  return Vector3d(sizex, 0.025, 0.002);
}

unsigned int display_number::get_port_in_count() {
  /// How many input ports it has
  return 1;
}

std::string display_number::get_port_in_name(unsigned int port __attribute__((__unused__))) {
  /// Name of the input port numbered n
  return "input value";
}

std::string display_number::get_port_in_description(unsigned int port) {
  /// Description of the input port numbered n
  std::stringstream desc;
  desc << "Input data value to display."
          "  " << get_port_in_connstatus(port);
  return desc.str();
}

bool display_number::get_port_in_required(unsigned int port __attribute__((__unused__))) {
  /// Whether an input on this port is necessary for this device to operate
  // all inputs are mandatory
  return true;
}

void display_number::update() {
  /// Update the number on this display
  if(ports_in[0].target) {
    displaycontent = trimstring(ports_in[0].target->get_port_out_data(ports_in[0].target_port));
  } else {
    displaycontent = "NO INPUT";
  }
}

void display_number::update_if_time() {
  /// Run the update function only if it's time for an update, and reset the update clock
  boost::chrono::time_point<boost::chrono::high_resolution_clock, boost::chrono::duration<double>> const time_now(boost::chrono::high_resolution_clock::now());
  if(time_now >= time_nextupdate) {
    update();
    time_nextupdate = time_now + boost::chrono::duration<double>(boost::chrono::milliseconds(250));   // 4Hz
  }
}

void display_number::render() {
  /// Render this display's contents in the right place
  update_if_time();

  glPushMatrix();

  glTranslated(position.x,
               position.y,
               position.z);

  Vector3d thissize = get_size();

  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, Vector4f(0.2, 0.2, 0.2, 1.0));
  glMaterialfv(GL_FRONT, GL_SPECULAR,            Vector4f(0.2, 0.2, 0.2, 1.0));
  glMaterialfv(GL_FRONT, GL_EMISSION,            Vector4f(0.0, 0.0, 0.0, 1.0));
  glMaterialf(GL_FRONT,  GL_SHININESS,           2.0);                           // 0 to 127

  //glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, Vector4f(0.2, 0.2, 0.2, 1.0));
  //glMaterialfv(GL_FRONT, GL_SPECULAR,            Vector4f(1.0, 1.0, 1.0, 1.0));
  //glMaterialfv(GL_FRONT, GL_EMISSION,            Vector4f(0.0, 0.0, 0.0, 1.0));
  //glMaterialf(GL_FRONT,  GL_SHININESS,           127.0);                           // 0 to 127
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

  // text display
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, Vector4f(0.0, 0.0, 0.0, 1.0));
  glMaterialfv(GL_FRONT, GL_SPECULAR,            Vector4f(0.0, 0.0, 0.0, 1.0));
  glMaterialfv(GL_FRONT, GL_EMISSION,            Vector4f(0.0, 0.8, 0.0, 1.0));
  glMaterialf(GL_FRONT,  GL_SHININESS,           127.0);                           // 0 to 127
  glPushMatrix();
  glTranslated(0.001, 0.003, thissize.z + 0.0001);
  glScaled(displayscale, displayscale, displayscale);
  glMaterialfv(GL_FRONT, GL_EMISSION,            Vector4f(0.0, 0.0, 0.0, 1.0));
  //font_7segment3d->Render("8.8.8.8.8.8.8.8.", 16, FTPoint(), FTPoint(), FTGL::RENDER_FRONT);   // draw the background fill
  font_7segment3d->Render("8,8,8,8,8,8,8,8,", 16, FTPoint(), FTPoint(), FTGL::RENDER_FRONT);   // draw the background fill
  glTranslated(0.0, 0.0, 0.1);
  glMaterialfv(GL_FRONT, GL_EMISSION,            Vector4f(0.0, 0.8, 0.0, 1.0));
  font_7segment3d->Render(displaycontent.c_str(), -1, FTPoint(), FTPoint(), FTGL::RENDER_FRONT);
  glPopMatrix();

  glPopMatrix();
}

std::string display_number::trimstring(std::string const &oldstring) {
  /// Trim, sanitise and clean up input and return a string precisely the right length
  // permissible chars for 7-Segment-Display-Extended.ttf are A-Za-z0-9-_ ,. and the latter two do not advance the cursor
  std::stringstream ss;
  // iterate through every char and deal appropriately, C style for speed
  unsigned int outchars = 0;
  for(char const &c : oldstring) {            // <3 <3 <3 C++11
    if(c == ' ' ||
       c == '-' ||
       c == '_' ||
       (c >= '0' && c <= '9') ||
       (c >= 'A' && c <= 'Z') ||
       (c >= 'a' && c <= 'z')) {
      ss << c;
      ++outchars;
    } else if(c == ',' ||
              c == '.') {
      ss << c;
    } else {
      // skip all non-printables
      continue;
    }
    if(outchars == 8) {
      break;
    }
  }
  //std::string outstring(8 - outchars, ' ');   // front filler
  //outstring += ss.str();
  //return outstring;
  ss << std::string(8 - outchars, ' ');     // back filler
  return ss.str();
}

std::string display_number::trimstring(double input) {
  /// Process a number for display or show error on overflow
  std::stringstream numss;
  // let integers be displayed without decimal cruft
  if(fabs(input - static_cast<int>(input)) > 0.00000001) {
    numss.width(8);
    numss.precision(8);
    numss << std::fixed << input;
    numss << boost::format("%-8f") % input;
  } else {
    numss << static_cast<int>(input);
  }
  // grab the first 9 characters only
  std::cout << numss.str() << std::endl;
  std::cout << "0.0000000 [reference]" << std::endl;
  std::stringstream ss;
  unsigned int outchars = 0;
  bool haddecimal = false;
  for(char const &c : numss.str()) {            // <3 <3 <3 C++11
    if(c == '.') {
      haddecimal = true;
    } else {
      ++outchars;
    }
    ss << c;
    if(haddecimal) {
      // come out of the loop when we fill the display
      if(outchars == 8) {
        break;
      }
    } else {
      // if none is a decimal point and we've filled more than 8, we have an overflow
      if(outchars == 9) {
        return "ERROR O.F.";
      }
    }
  }
  //std::string outstring((outchars < 8) ? 8 - outchars : 0, ' ');   // front filler
  std::string outstring(8 - outchars, ' ');   // front filler - less safe version but should be ok as long as above stays the same
  outstring += ss.str();
  return outstring;
  //ss << std::string(8 - outchars, ' ');     // back filler
  //return ss.str();
}
