#include "device.h"
#include <sstream>
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <FTGL/ftgl.h>
#include "spacecraft.h"
#include "instrumentpanel.h"

extern FTFont *font_title;          // global font definitions
extern FTFont *font_text;

Vector2i const device::screensize_static_analogue(64, 64);
Vector2i const device::screensize_static_digital( 16, 16);
GLuint device::image_static_analogue = 0;
GLuint device::image_static_digital  = 0;
boost::chrono::time_point<boost::chrono::high_resolution_clock, boost::chrono::duration<double>> device::time_next_static_analogue(boost::chrono::high_resolution_clock::now());
boost::chrono::time_point<boost::chrono::high_resolution_clock, boost::chrono::duration<double>> device::time_next_static_digital( boost::chrono::high_resolution_clock::now());
boost::chrono::duration<double> const device::time_interval_static_analogue(boost::chrono::milliseconds(100));
boost::chrono::duration<double> const device::time_interval_static_digital( boost::chrono::milliseconds(500));

device::device()
  : time_nextupdate(boost::chrono::high_resolution_clock::now()),
    status(statustype::UNMOUNTED),
    vessel(nullptr),
    panel(nullptr),
    functional(true) {
  /// Default constructor
  rotation = Quatd(1.0, 0.0, 0.0, 0.0);       // null rotation quaternion
}

device::~device() {
  /// Default destructor
  disconnect_all();
  glDeleteTextures(1, &image_static_analogue);
  glDeleteTextures(1, &image_static_digital);
}

std::string device::get_name() {
  /// Return the name of this device
  // virtual placeholder
  std::cout << "WARNING: virtual function " << __PRETTY_FUNCTION__ << " called without specialisation - this should never happen." << std::endl;
  return "device";
}

std::string device::get_manufacturer() {
  /// Return a manufacturer name for this device
  random_reset();
  return get_random_name_corporation();
}

std::string device::get_model() {
  /// Return a model name for this device
  // virtual placeholder
  std::cout << "WARNING: virtual function " << __PRETTY_FUNCTION__ << " called without specialisation - this should never happen." << std::endl;
  return "DEV0001a";
}

std::string device::get_description() {
  /// Return a detailed description of this device
  // virtual placeholder
  std::cout << "WARNING: virtual function " << __PRETTY_FUNCTION__ << " called without specialisation - this should never happen." << std::endl;
  return "A device of some sort.  It doesn't appear to do anything.";
}

double device::get_mass() {
  /// Return the weight of the device, in kilograms
  // virtual placeholder
  return 1.0;
}

Vector3d device::get_position() {
  /// Return a position for this object in the ship or on the panel
  return position;
}

Vector3d device::get_size() {
  /// Return a size for this object, in metres - hardcoded
  return Vector3d(0.2, 0.2, 0.2);
}

Quatd device::get_rotation() {
  /// Return the rotation quaternion of this object - hardcoded
  return rotation;
}

void device::set_position(Vector3d const &newposition) {
  /// Update the position of this device
  position = newposition;
}

void device::set_position(double x, double y, double z) {
  /// Update the position of this device - component version
  position = Vector3d(x, y, z);
}

void device::set_rotation(Quatd const &newrotation) {
  /// Return the rotation quaternion of this object
  rotation = newrotation;
}

unsigned int device::get_port_in_count() {
  /// How many input ports it has
  return 0;
}

std::string device::get_port_in_name(unsigned int port __attribute__((__unused__))) {
  /// Name of the input port numbered n
  // virtual placeholder
  std::cout << "WARNING: virtual function " << __PRETTY_FUNCTION__ << " called without specialisation - this should never happen." << std::endl;
  return "";
}

std::string device::get_port_in_description(unsigned int port __attribute__((__unused__))) {
  /// Description of the input port numbered n
  // virtual placeholder
  std::cout << "WARNING: virtual function " << __PRETTY_FUNCTION__ << " called without specialisation - this should never happen." << std::endl;
  return "";
}

bool device::get_port_in_required(unsigned int port __attribute__((__unused__))) {
  /// Whether an input on this port is necessary for this device to operate
  return false;
}

std::string device::get_port_in_connstatus(unsigned int port) {
  /// Get a verbal description of the connection status of this input port
  std::stringstream desc;
  if(ports_in[port].target) {
    device *target = ports_in[port].target;
    desc << "Connected to \"" << target->get_port_out_name(ports_in[port].target_port)
         << "\" port of " << target->get_name();
    if(!target->functional) {
      desc << " (not functional)";
    }
    desc << ".";
  } else {
    desc << "Not connected";
    if(get_port_in_required(port)) {
      desc << " (required).";
    } else {
      desc << " (optional).";
    }
  }
  return desc.str();
}

unsigned int device::get_port_out_count() {
  /// How many output ports it has
  return 0;
}

std::string device::get_port_out_name(unsigned int port __attribute__((__unused__))) {
  /// Name of the output port numbered n
  // virtual placeholder
  std::cout << "WARNING: virtual function " << __PRETTY_FUNCTION__ << " called without specialisation - this should never happen." << std::endl;
  return "";
}

std::string device::get_port_out_description(unsigned int port __attribute__((__unused__))) {
  /// Description of the output port numbered n
  // virtual placeholder
  std::cout << "WARNING: virtual function " << __PRETTY_FUNCTION__ << " called without specialisation - this should never happen." << std::endl;
  return "";
}

double device::get_port_out_data(unsigned int port __attribute__((__unused__))) {
  /// Query the value data on the specified out port
  // virtual placeholder - this may get called if nothing specialises it
  return 0.0;
}

std::string device::get_port_out_text(unsigned int port __attribute__((__unused__))) {
  /// Query the text data on the specified out port
  // generate a string of random junk
  std::stringstream ss;
  for(unsigned int i = 0; i != 8; ++i) {
    ss << static_cast<char>(get_random_uint(33, 126));
  }
  return ss.str();
}

GLuint device::get_port_out_video_analogue(unsigned int port __attribute__((__unused__))) {
  /// Query the analogue video data on the specified out port - returns a texture id
  // virtual placeholder - this may get called if nothing specialises it
  return generate_static_analogue();
}

GLuint device::get_port_out_video_digital(unsigned int port __attribute__((__unused__))) {
  /// Query the digital video data on the specified out port - returns a texture id
  // virtual placeholder - this may get called if nothing specialises it
  return generate_static_digital();
}

void device::get_port_out_sound(unsigned int port __attribute__((__unused__))) {
  /// Query the audio data on the specified out port
  // virtual placeholder - this may get called if nothing specialises it
  // this gets called if someone connects a audio speaker to a non-audio output
  // TODO: output hissing / buzzing / glitch noises
}

GLuint device::generate_static_analogue() {
  /// Generate and return a static image
  // only redraw if it's time to do so
  boost::chrono::time_point<boost::chrono::high_resolution_clock, boost::chrono::duration<double>> const time_now(boost::chrono::high_resolution_clock::now());
  if(time_now >= time_next_static_analogue) {
    // TODO: render n frames of static once only to 3D texture, and cycle through the layers
    time_next_static_analogue = time_now + time_interval_static_analogue;
    if(image_static_analogue == 0) {
      // we haven't allocated a texture, so do so now
      glGenTextures(1, &image_static_analogue);
      glEnable(GL_TEXTURE_2D);
      glBindTexture(GL_TEXTURE_2D, image_static_analogue);        // bind the texture
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, screensize_static_analogue.x, screensize_static_analogue.y, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glBindTexture(GL_TEXTURE_2D, 0);                    // release the texture
    }

    // this gets called if someone connects an analogue video display to a non-video output
    glBindTexture(GL_TEXTURE_2D, image_static_analogue);            // bind the screen texture
    // analogue tv style banded white noise:
    GLubyte temp_buffer[screensize_static_analogue.x][screensize_static_analogue.y][3];
    for(int x = 0; x != screensize_static_analogue.x; ++x) {
      double const band = get_random_double(0.5, 1.0);
      for(int y = 0; y != screensize_static_analogue.y; ++y) {
        unsigned char const value = get_random_int(63, 255) * band;
        temp_buffer[x][y][0] = value;                             // uniform b&w noise
        temp_buffer[x][y][1] = value;
        temp_buffer[x][y][2] = value;
        //temp_buffer[x][y][0] = value + get_random_int(-25, 25);   // pale colour noise
        //temp_buffer[x][y][1] = value + get_random_int(-25, 25);
        //temp_buffer[x][y][2] = value + get_random_int(-25, 25);
      }
    }
    glTexImage2D(GL_TEXTURE_2D,                             // target
                 0,                                         // mipmap level
                 GL_RGB,                                    // internalFormat
                 screensize_static_analogue.x,              // dimensions
                 screensize_static_analogue.y,
                 0,                                         // border
                 GL_RGB,                                    // format
                 GL_UNSIGNED_BYTE,                          // type of pixel data (GLubyte), see http://www.opengl.org/sdk/docs/man/xhtml/glTexImage2D.xml
                 &temp_buffer);                             // buffer or NULL to leave undefined
    glBindTexture(GL_TEXTURE_2D, 0);                        // release the screen texture
  }
  return image_static_analogue;
}

GLuint device::generate_static_digital() {
  /// Generate and return a digital static image
  // this gets called if someone connects a digital video display to a non-video output
  // only redraw if it's time to do so
  boost::chrono::time_point<boost::chrono::high_resolution_clock, boost::chrono::duration<double>> const time_now(boost::chrono::high_resolution_clock::now());
  if(time_now >= time_next_static_digital) {
    time_next_static_digital = time_now + time_interval_static_digital;

    if(image_static_digital == 0) {
      // we haven't allocated a texture, so do so now
      glGenTextures(1, &image_static_digital);
      glEnable(GL_TEXTURE_2D);
      glBindTexture(GL_TEXTURE_2D, image_static_digital);        // bind the texture
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, screensize_static_digital.x, screensize_static_digital.y, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);                // nearest neighbour filtering for square pixel effect
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      glBindTexture(GL_TEXTURE_2D, 0);                    // release the texture
    }

    glBindTexture(GL_TEXTURE_2D, image_static_digital);            // bind the screen texture
    // digital tv style strips of junk
    GLubyte temp_buffer[screensize_static_digital.x * screensize_static_digital.y][3];
    Vector3d newcolour(get_random_int(0, 255),
                       get_random_int(0, 255),
                       get_random_int(0, 255));
    for(int i = 0; i != screensize_static_digital.x * screensize_static_digital.y; ++i) {
      // periodically re-randomise
      if(get_random_bool(1 / (screensize_static_digital.x * 1.5))) {
        newcolour.r = get_random_int(0, 255);
        newcolour.g = get_random_int(0, 255);
        newcolour.b = get_random_int(0, 255);
      }
      temp_buffer[i][0] = newcolour.r + get_random_int(-10, 10);
      temp_buffer[i][1] = newcolour.g + get_random_int(-10, 10);
      temp_buffer[i][2] = newcolour.b + get_random_int(-10, 10);
    }
    glTexImage2D(GL_TEXTURE_2D,                             // target
                 0,                                         // mipmap level
                 GL_RGB,                                    // internalFormat
                 screensize_static_digital.x,               // dimensions
                 screensize_static_digital.y,
                 0,                                         // border
                 GL_RGB,                                    // format
                 GL_UNSIGNED_BYTE,                          // type of pixel data (GLubyte), see http://www.opengl.org/sdk/docs/man/xhtml/glTexImage2D.xml
                 &temp_buffer);                             // buffer or NULL to leave undefined
    glBindTexture(GL_TEXTURE_2D, 0);                        // release the screen texture
  }
  return image_static_digital;
}

void device::generate_static_sound() {
  /// Generate hissing or buzzing or whistling noises that sound like untuned radio transmissions
  // TODO
}

void device::attach(spacecraft *to_vessel) {
  /// Attach this device to the specified ship
  if(!to_vessel) {
    std::cout << "ERROR: tried to attach " << get_name() << " to null vessel." << std::endl;
    return;
  }
  vessel = to_vessel;
  vessel->devices.push_back(this);
  // don't attach it to any panel by default
}

bool device::attach_panel(instrumentpanel *to_panel) {
  /// Attempt to attach this device to the specified panel, return success status
  if(!to_panel) {
    std::cout << "ERROR: tried to attach device " << get_name() << " to null panel." << std::endl;
    return false;
  }
  panel = to_panel;
  panel->devices.push_back(this);
  // TODO: find an available position for it on the panel
  set_rotation(Quatd(1.0, 0.0, 0.0, 0.0));       // null rotation quaternion
  status = statustype::ON_PANEL;
  return true;
}

bool device::attach_hull() {
  /// Attempt to attach this device to the hull, return success status
  if(panel) {
    panel->devices.remove(this);
  }
  vessel->devices_cabin.remove(this);
  vessel->devices_hull.push_back(this);
  status = statustype::ON_HULL;
  return true;
}

bool device::attach_cabin() {
  /// Attempt to attach this device to a wall of the cabin, return success status
  if(panel) {
    panel->devices.remove(this);
  }
  vessel->devices_hull.remove(this);
  vessel->devices_cabin.push_back(this);
  status = statustype::IN_CABIN;
  return true;
}

void device::remove() {
  /// Remove this device from whatever ship it's attached to
  /// Note: not safe to be called in an iteration of instruments or devices!
  if(!vessel) {
    std::cout << "ERROR: tried to remove " << get_name() << " which is already not attached to anything." << std::endl;
    return;
  }
  // detach it from whatever part of the ship it's connected to
  remove_panel();
  remove_hull();
  remove_cabin();
  // remove it from the list of the vessel's devices
  vessel->devices.remove(this);
  disconnect_all();
  vessel = nullptr;     // this must obviously come last
}

void device::remove_panel() {
  /// Remove this instrument from its panel only (leave it attached to any ship)
  /// Note: not safe to be called in an iteration of instruments!
  if(!vessel) {
    std::cout << "ERROR: tried to remove instrument " << get_name() << " which is already not attached to a vessel." << std::endl;
    return;
  }
  if(!panel) {
    // not every device is connected to a panel
    //std::cout << "ERROR: tried to remove instrument " << get_name() << " which is already not attached to a panel." << std::endl;
    return;
  }
  panel->devices.remove(this);
  panel = nullptr;     // this must obviously come last
  status = statustype::UNMOUNTED;
}

void device::remove_hull() {
  /// Remove this device from the hull only (leave it attached to any ship)
  /// Note: not safe to be called in an iteration of hull devices!
  if(!vessel) {
    std::cout << "ERROR: tried to remove hull device " << get_name() << " which is already not attached to a vessel." << std::endl;
    return;
  }
  vessel->devices_hull.remove(this);
  status = statustype::UNMOUNTED;
}

void device::remove_cabin() {
  /// Remove this device from the cabin only (leave it attached to any ship)
  /// Note: not safe to be called in an iteration of cabin devices!
  if(!vessel) {
    std::cout << "ERROR: tried to remove cabin device " << get_name() << " which is already not attached to a vessel." << std::endl;
    return;
  }
  vessel->devices_cabin.remove(this);
  status = statustype::UNMOUNTED;
}

void device::connect(unsigned int port_in, device *target, unsigned int target_port_out) {
  /// Connect an input port on this device to an output port elsewhere
  if(port_in >= get_port_in_count()) {
    std::cout << "ERROR: tried to connect to input port " << port_in << " on device " << get_name() << " which only has " << get_port_in_count() << " ports." << std::endl;
    return;
  }
  if(!target) {
    std::cout << "ERROR: tried to connect to port on null target." << std::endl;
    return;
  }
  if(target_port_out >= target->get_port_out_count()) {
    std::cout << "ERROR: tried to connect " << get_name() << " to output port " << target_port_out << " on device " << target->get_name() << " which only has " << target->get_port_out_count() << " ports." << std::endl;
    return;
  }
  //if(ports_in[port_in].target) {
  //  // it's already plugged into something - disconnect it first
  //  disconnect(port_in);
  //}
  ports_in[port_in].target = target;
  ports_in[port_in].target_port = target_port_out;
  update();
  target->update();
}

void device::disconnect(unsigned int port_in) {
  /// Disconnect an input port on this device
  if(port_in >= get_port_in_count()) {
    std::cout << "ERROR: tried to disconnect input port " << port_in << " on device " << get_name() << " which only has " << get_port_in_count() << " ports." << std::endl;
    return;
  }
  device *cachedtarget = ports_in[port_in].target;
  ports_in[port_in].target = nullptr;
  ports_in[port_in].target_port = 0;
  update();
  cachedtarget->update();
}

void device::disconnect_all() {
  // make sure there are no dangling connections from other devices to this
  for(auto const &it : vessel->devices) {
    if(!it || it == this) {
      continue;         // don't check for connection to ourself
    }
    unsigned int numports = it->get_port_in_count();
    for(unsigned int i = 0; i != numports; ++i) {
      if(it->ports_in[i].target == this) {
        //std::cout << it->get_name() << " was disconnected from " << get_name() << std::endl;
        it->disconnect(i);
      }
    }
  }
}

void device::update() {
  /// Update the output states and respond to changes in input
  // virtual placeholder - may be called by classes that don't specialise this
}

void device::update_if_time() {
  /// Run the update function only if it's time for an update, and reset the update clock
  boost::chrono::time_point<boost::chrono::high_resolution_clock, boost::chrono::duration<double>> const time_now(boost::chrono::high_resolution_clock::now());
  if(time_now >= time_nextupdate) {
    update();
    time_nextupdate = time_now + boost::chrono::duration<double>(boost::chrono::milliseconds(100));
  }
}

void device::render() {
  /// Show this device on the instrument panel or in the cockpit or on the hull of the ship
  // render a basic placeholder for unspecified devices
  glPushMatrix();

  glTranslated(position.x,
               position.y,
               position.z);

  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, Vector4f(0.2, 0.2, 0.2, 1.0));
  glMaterialfv(GL_FRONT, GL_SPECULAR,            Vector4f(0.2, 0.2, 0.2, 1.0));
  glMaterialfv(GL_FRONT, GL_EMISSION,            Vector4f(0.0, 0.0, 0.0, 1.0));
  glMaterialf(GL_FRONT,  GL_SHININESS,           2.0);                           // 0 to 127
  //glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, Vector4f(1.6, 1.1, 0.2, 1.0));
  //glMaterialfv(GL_FRONT, GL_SPECULAR,            Vector4f(2.0, 1.9, 1.7, 1.0));
  //glMaterialfv(GL_FRONT, GL_EMISSION,            Vector4f(0.0, 0.0, 0.0, 1.0));
  //glMaterialf(GL_FRONT,  GL_SHININESS,           27.89743616);                           // 0 to 127

  glBegin(GL_QUADS);
  // front
  glNormal3d(0.0, 0.0, 1.0);
  glVertex3d(0.0,          0.0,          get_size().z);
  glVertex3d(get_size().x, 0.0,          get_size().z);
  glVertex3d(get_size().x, get_size().y, get_size().z);
  glVertex3d(0.0,          get_size().y, get_size().z);
  // top
  glNormal3d(0.0, 1.0, 0.0);
  glVertex3d(0.0,          get_size().y, 0.0);
  glVertex3d(0.0,          get_size().y, get_size().z);
  glVertex3d(get_size().x, get_size().y, get_size().z);
  glVertex3d(get_size().x, get_size().y, 0.0);
  // bottom
  glNormal3d(0.0, -1.0, 0.0);
  glVertex3d(0.0,          0.0,          0.0);
  glVertex3d(get_size().x, 0.0,          0.0);
  glVertex3d(get_size().x, 0.0,          get_size().z);
  glVertex3d(0.0,          0.0,          get_size().z);
  // right
  glNormal3d(1.0, 0.0, 0.0);
  glVertex3d(get_size().x, 0.0,          0.0);
  glVertex3d(get_size().x, get_size().y, 0.0);
  glVertex3d(get_size().x, get_size().y, get_size().z);
  glVertex3d(get_size().x, 0.0,          get_size().z);
  // left
  glNormal3d(-1.0, 0.0, 0.0);
  glVertex3d(0.0,          0.0,          0.0);
  glVertex3d(0.0,          0.0,          get_size().z);
  glVertex3d(0.0,          get_size().y, get_size().z);
  glVertex3d(0.0,          get_size().y, 0.0);
  glEnd();

  // manufacturer / model label
  Vector3d const thissize = get_size();
  double const scale = 0.00035277777;       // 1m / (72dpi * 39.3700787in) = 0.00035277777
  //glEnable(GL_NORMALIZE);                   // to allow correct lighting
  glEnable(GL_RESCALE_NORMAL);              // to allow correct lighting (faster than GL_NORMALIZE)
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, Vector4f(0.8, 0.8, 0.8, 1.0));
  glMaterialfv(GL_FRONT, GL_SPECULAR,            Vector4f(0.8, 0.8, 0.8, 1.0));
  glMaterialfv(GL_FRONT, GL_EMISSION,            Vector4f(0.0, 0.0, 0.0, 1.0));
  glMaterialf(GL_FRONT,  GL_SHININESS,           2.0);                           // 0 to 127
  double const modellength = font_title->Advance(get_model().c_str(), -1);
  if(modellength * scale <= thissize.x + 0.004) {               // only insert if there's room to do so
    glPushMatrix();
    glTranslated(thissize.x - 0.002, 0.002, thissize.z + 0.001);
    glScaled(scale, scale, scale);
    glTranslated(-modellength, 0.0, 0.0);    // slide it back for right-align
    font_title->Render(get_model().c_str(), -1, FTPoint(), FTPoint(), FTGL::RENDER_FRONT);
    glPopMatrix();
    double const manufacturerlength = font_title->Advance(get_manufacturer().c_str(), -1);
    if((modellength + manufacturerlength) * scale <= thissize.x - 0.006) {
      glPushMatrix();
      glTranslated(0.002, 0.002, thissize.z + 0.001);
      glScaled(scale, scale, scale);
      font_title->Render(get_manufacturer().c_str(), -1, FTPoint(), FTPoint(), FTGL::RENDER_FRONT);
      glPopMatrix();
    }
  } else {                                                     // otherwise scale model down to fit
    glPushMatrix();
    glTranslated(0.001, 0.001, thissize.z + 0.001);
    double const newscale = (thissize.x - 0.002) / modellength;
    glScaled(newscale, newscale, newscale);
    font_title->Render(get_model().c_str(), -1, FTPoint(), FTPoint(), FTGL::RENDER_FRONT);
    glPopMatrix();
  }
  //glDisable(GL_NORMALIZE);                  // enable needed to allow correct lighting, disable for speed
  glDisable(GL_RESCALE_NORMAL);             // enable needed to allow correct lighting, disable for speed

  glPopMatrix();
}

bool device::pick(Vector2d pickpos) {
  /// Return true if we're picking this by panel coords - 2D version
  Vector3d const thissize = get_size();
  if(pickpos.x >= position.x &&
     pickpos.y >= position.y &&
     pickpos.x <= position.x + thissize.x &&
     pickpos.y <= position.y + thissize.y) {
    return true;
  } else {
    return false;
  }
}

bool device::pick(Vector3d pickpos) {
  /// Return true if we're picking this by panel coords - 3D version
  Vector3d const thissize = get_size();
  if(pickpos.x >= position.x &&
     pickpos.y >= position.y &&
     pickpos.z >= position.z &&
     pickpos.x <= position.x + thissize.x &&
     pickpos.y <= position.y + thissize.y &&
     pickpos.y <= position.z + thissize.z) {
    return true;
  } else {
    return false;
  }
}

void device::destroy() {
  /// Put this device out of commission
  std::cout << get_name() << " is no longer operative." << std::endl;
  functional = false;
}

void device::describe_to_console() {
  /// Dump a vebrose description of this device and all its connections to the console
  std::cout << "*** " << get_manufacturer() << " model " << get_model() << ": " << get_name() << " ***" << std::endl;
  std::cout << get_description() << std::endl;
  std::cout << "Status: ";
  switch(status) {
  case statustype::UNMOUNTED:
    std::cout << "In the spare parts bin.";
    break;
  case statustype::ON_HULL:
    std::cout << "Mounted outside on the hull.";
    break;
  case statustype::IN_CABIN:
    std::cout << "Mounted on a wall of the cabin.";
    break;
  case statustype::ON_PANEL:
    std::cout << "Mounted on an instrument panel.";
    break;
  }
  std::cout << std::endl;
  std::cout << "Input ports: " << get_port_in_count() << std::endl;
  for(unsigned int i = 0; i != get_port_in_count(); ++i) {
    std::cout << "  " << i + 1 << ": " << get_port_in_name(i) << std::endl;
    std::cout << "    " << get_port_in_description(i) << std::endl;
  }
  std::cout << "Output ports: " << get_port_out_count() << std::endl;
  for(unsigned int i = 0; i != get_port_out_count(); ++i) {
    std::cout << "  " << i + 1 << ": " << get_port_out_name(i) << std::endl;
    std::cout << "    " << get_port_out_description(i) << std::endl;
  }
  std::cout << "Dimensions: ";
  if(get_size().x < 1 || get_size().y < 1 || get_size().z < 1) {
    std::cout << get_size() * 1000 << "mm";
  } else {
    std::cout << get_size() << "m";
  }
  std::cout << ", Mass: ";
  if(get_mass() < 1) {
    std::cout << get_mass() * 1000 << "g";
  } else {
    std::cout << get_mass() << "Kg";
  }
  std::cout << std::endl;
}

void device::activate() {
  /// Activate the current device, whatever that means for this particular one
  // virtual placeholder
  std::cout << "WARNING: virtual function " << __PRETTY_FUNCTION__ << " called without specialisation - this should never happen." << std::endl;
}
