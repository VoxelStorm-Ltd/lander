#include "camera.h"

Vector2i const camera::windowsize = Vector2i(256, 256);

camera::camera()
  : display_image(0),
    framebuffer(0),
    zoom(1.0),
    rotation_x(0.0),
    rotation_y(0.0) {
  /// Default constructor
  ports_in.resize(get_port_in_count());     // anything with input ports needs this

  // create a blank texture
  glGenTextures(1, &display_image);
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, display_image);        // bind the screen texture
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, windowsize.x, windowsize.y, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
  // texture parameters
  //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
  //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);    // average between two mipmap levels on nearest neighbour texture pixel
  //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);    // when texture area is small, bilinear filter the closest mipmap
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);     // aka trilinear - nvidia recommended (see https://developer.nvidia.com/sites/default/files/akamai/gamedev/docs/opengl_rendertexture.pdf)
  //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);                  // nearest neighbour filtering
  //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);                  // nearest-neighbour on closeup views to show the pixel squares
  //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);                   // when texture area is large, bilinear filter the original
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LOD,   2);                            // maximum mipmap level
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 2);                            // maximum mipmap level
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
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
  glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, windowsize.x, windowsize.y);
  glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT,   GL_RENDERBUFFER_EXT, depthbuffer);

  GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
  if(status != GL_FRAMEBUFFER_COMPLETE_EXT) {
    std::cout << "framebuffer fucked: " << status;
  }
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
}

camera::~camera() {
  /// Default destructor
  glDeleteTextures(1, &display_image);
}

std::string camera::get_name() {
  /// Return the name of this device
  return "digital video camera";
}

std::string camera::get_model() {
  /// Return a model name for this device
  random_reset();
  std::stringstream model;
  model << "CVPT-I" << get_random_int(100, 400) << " Pro 9000";
  return model.str();
}

std::string camera::get_description() {
  /// Return a detailed description of this device
  return "A specialised camera designed to work both in space and in atmospheres."
         "  Despite the fancy casing, the image quality and the subtle rattle "
         "of the internal components both imply that it's really just a cheap "
         "little Chinese webcam in a heavy armoured box.";
}

double camera::get_mass() {
  /// Return the weight of the device, in kilograms
  return 2.5;
}

Vector3d camera::get_size() {
  /// Return a size for this object, in metres - hardcoded
  return Vector3d(0.2, 0.2, 0.2);
}

unsigned int camera::get_port_in_count() {
  /// How many input ports it has
  return 3;
}

std::string camera::get_port_in_name(unsigned int port) {
  /// Name of the input port numbered n
  switch(port) {
  case 0:
    return "zoom";
  case 1:
    return "pan";
  case 2:
    return "tilt";
  default:
    return "";
  }
}

std::string camera::get_port_in_description(unsigned int port) {
  /// Description of the input port numbered n
  switch(port) {
  case 0:
    {
      std::stringstream desc;
      desc << "How much to zoom in from the widest field of view."
              "  " << get_port_in_connstatus(port);
      return desc.str();
    }
  case 1:
    {
      std::stringstream desc;
      desc << "Rotation left and right, either positive or negative, in degrees."
              "  " << get_port_in_connstatus(port);
      return desc.str();
    }
  case 2:
    {
      std::stringstream desc;
      desc << "Rotation up and down, either positive or negative, in degrees."
              "  " << get_port_in_connstatus(port);
      return desc.str();
    }
  default:
    return "";
  }
}

bool camera::get_port_in_required(unsigned int port __attribute__((__unused__))) {
  /// Whether an input on this port is necessary for this device to operate
  return false;
}

unsigned int camera::get_port_out_count() {
  /// How many output ports it has
  return 1;
}

std::string camera::get_port_out_name(unsigned int port __attribute__((__unused__))) {
  /// Name of the output port numbered n
  switch(port) {
  case 0:
    return "digital video out";
  default:
    return "";
  }
}

std::string camera::get_port_out_description(unsigned int port __attribute__((__unused__))) {
  /// Description of the output port numbered n
  switch(port) {
  case 0:
    return "A digital video signal.";
  default:
    return "";
  }
}

GLuint camera::get_port_out_video_digital(unsigned int port __attribute__((__unused__))) {
  /// Render the view from this camera
  update_if_time();
  return display_image;
}


void camera::update() {
  /// Update the output states and respond to changes in input
  // update the zoom
  if(ports_in[0].target) {
    zoom = ports_in[0].target->get_port_out_data(ports_in[0].target_port);
  } else {
    zoom = 1.0;
  }
  // update the rotation
  if(ports_in[1].target) {
    // pan
    rotation_x = ports_in[2].target->get_port_out_data(ports_in[2].target_port);
  }
  if(ports_in[2].target) {
    // tilt
    rotation_y = ports_in[1].target->get_port_out_data(ports_in[1].target_port);
  }
}

void camera::update_if_time() {
  /// Run the update function only if it's time for an update, and reset the update clock
  boost::chrono::time_point<boost::chrono::high_resolution_clock, boost::chrono::duration<double>> const time_now(boost::chrono::high_resolution_clock::now());
  if(time_now >= time_nextupdate) {
    refresh();
    time_nextupdate = time_now + boost::chrono::duration<double>(boost::chrono::milliseconds(45));  // 22fps
  }
}

void camera::refresh() {
  /// Render from this camera's perspective and update the texture
}
