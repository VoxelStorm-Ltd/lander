#include "terminal.h"
//#include <GL/glew.h>
//#include <GLFW/glfw3.h>
#include <FTGL/ftgl.h>
#include <iostream>

// TODO: load a monospace computery font
extern FTFont *font_monospace_small;

vector2<unsigned int> const terminal::windowsize      = vector2i(512, 512);
vector2<unsigned int> const terminal::windowsize_text = vector2i(80,  48);

terminal::terminal() {
  /// Default constructor
  ports_in.resize(get_port_in_count());                                         // anything with input ports needs this
  update_vbo();                                                                 // every device with a custom size needs this

  // create a blank texture
  glGenTextures(1, &display_image);
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, display_image);                                  // bind the screen texture
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, windowsize.x, windowsize.y, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
  // texture parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // aka trilinear - nvidia recommended (see https://developer.nvidia.com/sites/default/files/akamai/gamedev/docs/opengl_rendertexture.pdf)
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);            // nearest-neighbour on closeup views to show the pixel squares
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LOD,   4);                      // maximum mipmap level
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 4);                      // maximum mipmap level
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glBindTexture(GL_TEXTURE_2D, 0);                                              // unbind the texture

  // create a framebuffer
  glGenFramebuffersEXT(1, &framebuffer);
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, framebuffer);
  glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT,                                 // target
                            GL_COLOR_ATTACHMENT0_EXT,                           // attachment point - colour, depth, stencil or depth-stencil
                            GL_TEXTURE_2D,                                      // texture target / cubemap face
                            display_image,                                      // texture
                            0);                                                 // level

  GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
  if(status != GL_FRAMEBUFFER_COMPLETE_EXT) {
    std::cout << "ERROR: framebuffer fucked: " << status;
  }
  if(!display_image) {
    std::cout << "ERROR: display_image got nothing from OpenGL" << std::endl;
  }
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
}

terminal::~terminal() {
  /// Default destructor
  glDeleteTextures(       1, &display_image);
  glDeleteFramebuffersEXT(1, &framebuffer);
}

std::string terminal::get_name() {
  /// Return the name of this device
  return "terminal";
}

std::string terminal::get_model() {
  /// Return a model name for this device
  random_reset();
  std::stringstream model;
  model << "VT-TTY" << get_random_int(100, 999);
  return model.str();
}

std::string terminal::get_description() {
  /// Return a detailed description of this device
  return "A computer terminal collects and buffers text input from a device, "
         "and generates a video signal displaying the buffer."
         "  It can also accept and buffer input character by character, for "
         "instance rom a keyboard, and send it on to another device when the "
         "line is complete (a carriage return is received).";
}

double terminal::get_mass() {
  /// Return the weight of the device, in kilograms
  return 1.0;
}

vector3d terminal::get_size() {
  /// Return a size for this object, in metres - hardcoded
  return vector3d(0.10, 0.10, 0.06);
}

unsigned int terminal::get_port_in_count() {
  /// How many input ports it has
  return 2;
}

std::string terminal::get_port_in_name(unsigned int port) {
  /// Name of the input port numbered n
  switch(port) {
  case 0:
    return "display text";
  case 1:
    return "entry text";
  default:
    return "";
  }
}

std::string terminal::get_port_in_description(unsigned int port) {
  /// Description of the input port numbered n
  switch(port) {
  case 0:
    {
      std::stringstream desc;
      desc << "A source for text to display on screen.  Returned text will be displayed continuously as it's read."
              "  " << get_port_in_connstatus(port);
      return desc.str();
    }
  case 1:
    {
      std::stringstream desc;
      desc << "Text data to accumulate and send.  Keeps a buffer of text and sends once a complete line has been entered and a newline (carriage return) is received."
              "  " << get_port_in_connstatus(port);
      return desc.str();
    }
  default:
    return "";
  }
}

bool terminal::get_port_in_required(unsigned int port) {
  /// Whether an input on this port is necessary for this device to operate
  if(port == 0) {
    return true;
  } else {
    return false;
  }
}

unsigned int terminal::get_port_out_count() {
  /// How many output ports it has
  return 2;
}

std::string terminal::get_port_out_name(unsigned int port) {
  /// Name of the output port numbered n
  switch(port) {
  case 0:
    return "analogue video";
  case 1:
    return "entered text";
  default:
    return "";
  }
}

std::string terminal::get_port_out_description(unsigned int port) {
  /// Description of the output port numbered n
  switch(port) {
  case 0:
    return "An analogue video signal showing the contents of the display buffer and the state of the input buffer.";
  case 1:
    return "A text output sending on the last entered complete line of text input.";
  default:
    return "";
  }
}

GLuint terminal::get_port_out_video_analogue(unsigned int port) {
  if(port == 0) {
    update_if_time();
    return display_image;
  } else {
    return 0;
  }
}

std::string terminal::get_port_out_text(unsigned int port) {
  /// Query the text data on the specified out port
  if(port == 1) {
    // TODO
    return "";
  } else {
    return "";
  }
}

void terminal::update() {
  /// Update the output states and respond to changes in input
  if(!ports_in[0].target) {
    return;
  }
  std::string s = ports_in[0].target->get_port_out_text(ports_in[0].target_port);
  if(s.empty()) {
    return;                                                                     // we ignore blank strings
  }
  // break lines
  do {
    buffer.push_back(s.substr(0, windowsize_text.x));
    if(buffer.size() > windowsize_text.y) {
      buffer.pop_front();
    }
  } while(s.length() > windowsize_text.x && (s = s.substr(windowsize_text.x, std::string::npos), true));
}

void terminal::update_if_time() {
  /// Run the update function only if it's time for an update, and reset the update clock
  boost::chrono::time_point<boost::chrono::high_resolution_clock, boost::chrono::duration<double>> const time_now(boost::chrono::high_resolution_clock::now());
  if(time_now >= time_nextupdate) {
    update();
    refresh();
    time_nextupdate = time_now + boost::chrono::duration<double>(boost::chrono::milliseconds(200)); // 5Hz
  }
}

void terminal::refresh() {
  /// Re-draw the texture
  // cache the old viewport
  vector4i oldviewport;
  glGetIntegerv(GL_VIEWPORT, oldviewport);
  glViewport(0, 0, windowsize.x, windowsize.y);
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, framebuffer);                        // bind the framebuffer for the display screen

  glPushAttrib(GL_ALL_ATTRIB_BITS);                                             // save state - see http://opengl.czweb.org/ch14/462-465.html
  glDisable(GL_LIGHTING);
  glDisable(GL_DEPTH_TEST);
  glClearColor(0.0, 0.0, 0.0, 1.0);
  glClear(GL_COLOR_BUFFER_BIT);

  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  //glOrtho(0.0, windowsize.x, windowsize.y, 0.0, 0.0, 1.0);
  glOrtho(0.0, windowsize.x, 0.0, windowsize.y, 0.0, 1.0);
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();


  glColor4d(0.2, 1.0, 0.2, 1.0);
  double linepos = windowsize.y - margin_bottom - lineheight;
  for(auto const &line : buffer) {
    font_monospace_small->Render(line.c_str(), line.length(), FTPoint(margin_left, linepos), FTPoint(), FTGL::RENDER_FRONT);
    linepos -= lineheight;
  }

  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();
  glPopAttrib();

  // release the framebuffer
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);                                  // unbind the framebuffer
  glViewport(oldviewport[0], oldviewport[1], oldviewport[2], oldviewport[3]);   // restore the viewport

  // generate mipmaps - only use this if we're actually using a mipmap
  glBindTexture(GL_TEXTURE_2D, display_image);
  glGenerateMipmapEXT(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, 0);
}
