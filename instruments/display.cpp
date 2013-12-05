#include "display.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

display::display()
  : display_image(0),
    framebuffer(0) {
  /// Default constructor
  size.x = 0.4;
  size.y = 0.4;
  size.z = 0.02;
  screen_pos.x = 0.01;
  screen_pos.y = 0.01;
  screen_size.x = size.x - 0.02;
  screen_size.y = size.y - 0.02;
  ports_in.resize(get_port_in_count());     // anything with input ports needs this

  // create a blank texture
  glGenTextures(1, &display_image);
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, display_image);        // bind the screen texture
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, screen_width, screen_height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
  // texture parameters
  //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);   // nvidia recommended (see https://developer.nvidia.com/sites/default/files/akamai/gamedev/docs/opengl_rendertexture.pdf)
  //glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);  // when texture area is small, bilinear filter the closest mipmap
  //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);                // nearest neighbour filtering
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);                 // when texture area is large, bilinear filter the original
  glGenerateMipmapEXT(GL_TEXTURE_2D);                 // only if we're using mipmaps
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_ADD);     // emissive style glow effect - see http://www.opengl.org/sdk/docs/man2/xhtml/glTexEnv.xml
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glBindTexture(GL_TEXTURE_2D, 0);                    // unbind the texture

  // create a framebuffer
  glGenFramebuffers(1, &framebuffer);
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, framebuffer);
  glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT,       // target
                            GL_COLOR_ATTACHMENT0_EXT, // attachment point - colour, depth, stencil or depth-stencil
                            GL_TEXTURE_2D,            // texture target / cubemap face
                            display_image,            // texture
                            0);                       // level
  // add a depth buffer
  glGenRenderbuffersEXT(1, &depthbuffer);
  glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, depthbuffer);
  glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, screen_width, screen_height);
  glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT,   GL_RENDERBUFFER_EXT, depthbuffer);

  GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
  if(status != GL_FRAMEBUFFER_COMPLETE_EXT) {
    std::cout << "framebuffer fucked: " << status;
  }
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
}

display::~display() {
  /// Default destructor
  glDeleteTextures(1, &display_image);
}

std::string display::get_name() {
  /// Return the name of this device
  return "graphical display";
}

std::string display::get_description() {
  /// Return a detailed description of this device
  random_reset();
  std::stringstream desc;
  desc << get_random_name_corporation() << " model CRT0606-G."
          "  A very cheap and old but reliable analogue CRT monitor."
          "  Capable of withstanding a lot of punishment, both physical and electromagnetic."
          "  Technically capable of displaying any colour, but for some reason tends to tint everything with a sickly greenish hue."
          "  Slightly increases the pilot's overall radiation exposure.";
  return desc.str();
}

unsigned int display::get_port_in_count() {
  /// How many input ports it has
  return 1;
}

std::string display::get_port_in_name(unsigned int port) {
  /// Name of the input port numbered n
  switch(port) {
  case 0:
    return "analogue video";
  default:
    return "";
  }
}

std::string display::get_port_in_description(unsigned int port) {
  /// Description of the input port numbered n
  switch(port) {
  case 0:
    {
      std::stringstream desc;
      desc << "An analogue video or still image signal."
              "  " << get_port_in_connstatus(port);
      return desc.str();
    }
  default:
    return "";
  }
}

bool display::get_port_in_required(unsigned int port __attribute__((__unused__))) {
  /// Whether an input on this port is necessary for this device to operate
  // all ports are needed
  return true;
}

void display::update() {
  if(ports_in[0].target) {
    // cache the old viewport
    Vector4i oldviewport;
    glGetIntegerv(GL_VIEWPORT, oldviewport);
    glViewport(0, 0, screen_width, screen_height);
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, framebuffer);  // bind the framebuffer for the display screen
    // call the target's render function
    ports_in[0].target->get_port_out_video_analogue(ports_in[0].target_port, Vector2d(screen_width, screen_height));
    // release the framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(oldviewport[0], oldviewport[1], oldviewport[2], oldviewport[3]);
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);            // unbind the framebuffer
  } else {
    glBindTexture(GL_TEXTURE_2D, display_image);            // bind the screen texture
    // analogue tv style banded white noise:
    GLubyte temp_buffer[screen_width][screen_height][3];
    for(unsigned int x = 0; x != screen_width; ++x) {
      double const band = get_random_double(0.5, 1.0);
      for(unsigned int y = 0; y != screen_height; ++y) {
        unsigned char const value = get_random_int(63, 255) * band;
        temp_buffer[x][y][0] = value;                             // uniform b&w noise
        temp_buffer[x][y][1] = value;
        temp_buffer[x][y][2] = value;
        //temp_buffer[x][y][0] = value + get_random_int(-25, 25);   // pale colour noise
        //temp_buffer[x][y][1] = value + get_random_int(-25, 25);
        //temp_buffer[x][y][2] = value + get_random_int(-25, 25);
      }
    }
    glTexImage2D(GL_TEXTURE_2D,               // target
                 0,                           // mipmap level
                 GL_RGB,                      // internalFormat
                 screen_width, screen_height, // dimensions
                 0,                           // border
                 GL_RGB,                      // format
                 GL_UNSIGNED_BYTE,            // type of pixel data (GLubyte), see http://www.opengl.org/sdk/docs/man/xhtml/glTexImage2D.xml
                 &temp_buffer);               // buffer or NULL to leave undefined
    glBindTexture(GL_TEXTURE_2D, 0);                              // release the screen texture
  }
}

void display::render() {
  /// Render this display's contents in the right place
  update();

  glPushMatrix();

  glTranslated(position.x,
               position.y,
               position.z);

  glColor4dv(Vector4d(0.6, 0.6, 0.6, 1.0));
  glBegin(GL_QUADS);
  // front
  glVertex3d(0.0,    0.0,    size.z);
  glVertex3d(size.x, 0.0,    size.z);
  glVertex3d(size.x, size.y, size.z);
  glVertex3d(0.0,    size.y, size.z);
  // top
  glVertex3d(0.0,    size.y, 0.0);
  glVertex3d(0.0,    size.y, size.z);
  glVertex3d(size.x, size.y, size.z);
  glVertex3d(size.x, size.y, 0.0);
  // bottom
  glVertex3d(0.0,    0.0,    0.0);
  glVertex3d(size.x, 0.0,    0.0);
  glVertex3d(size.x, 0.0,    size.z);
  glVertex3d(0.0,    0.0,    size.z);
  // right
  glVertex3d(size.x, 0.0,    0.0);
  glVertex3d(size.x, size.y, 0.0);
  glVertex3d(size.x, size.y, size.z);
  glVertex3d(size.x, 0.0,    size.z);
  // left
  glVertex3d(0.0,    0.0,    0.0);
  glVertex3d(0.0,    0.0,    size.z);
  glVertex3d(0.0,    size.y, size.z);
  glVertex3d(0.0,    size.y, 0.0);
  glEnd();

  glColor4dv(Vector4d(0.2, 0.2, 0.2, 1.0));
  glBindTexture(GL_TEXTURE_2D, display_image);        // bind the screen texture
  glBegin(GL_QUADS);
  glTexCoord2d(0.0, 0.0);
  glVertex3d(screen_pos.x,  screen_pos.y,  size.z + 0.001);
  glTexCoord2d(1.0, 0.0);
  glVertex3d(screen_size.x, screen_pos.y,  size.z + 0.001);
  glTexCoord2d(1.0, 1.0);
  glVertex3d(screen_size.x, screen_size.y, size.z + 0.001);
  glTexCoord2d(0.0, 1.0);
  glVertex3d(screen_pos.x,  screen_size.y, size.z + 0.001);
  glEnd();
  glBindTexture(GL_TEXTURE_2D, 0);                    // unbind the texture
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);        // unbind the framebuffer

  glPopMatrix();
}
