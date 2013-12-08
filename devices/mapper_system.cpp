#include "mapper_system.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "universe.h"
#include "starsystem.h"
#include "spacecraft.h"

extern universe root;

Vector2i const mapper_system::windowsize = Vector2i(256, 256);

mapper_system::mapper_system()
  : display_image(0),
    framebuffer(0),
    //scale(0.00001),           // earth scale
    scale(0.000000002),       // solar system scale
    rotation_x(-90.0),
    rotation_y(0.0),
    trail_ref(nullptr) {
  /// Default constructor
  ports_in.resize(get_port_in_count());     // anything with input ports needs this

  // create a blank texture
  glGenTextures(1, &display_image);
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, display_image);        // bind the screen texture
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, windowsize.x, windowsize.y, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
  // texture parameters
  //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);   // nvidia recommended (see https://developer.nvidia.com/sites/default/files/akamai/gamedev/docs/opengl_rendertexture.pdf)
  //glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);  // when texture area is small, bilinear filter the closest mipmap
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);                // nearest neighbour filtering
  //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  //glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);                 // when texture area is large, bilinear filter the original
  //glGenerateMipmapEXT(GL_TEXTURE_2D);                 // only if we're using mipmaps
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
  glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, windowsize.x, windowsize.y);
  glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT,   GL_RENDERBUFFER_EXT, depthbuffer);

  GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
  if(status != GL_FRAMEBUFFER_COMPLETE_EXT) {
    std::cout << "framebuffer fucked: " << status;
  }
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
}

mapper_system::~mapper_system() {
  /// Default destructor
}

std::string mapper_system::get_name() {
  /// Return the name of this device
  return "star system mapper";
}

std::string mapper_system::get_model() {
  /// Return a model name for this device
  random_reset();
  std::stringstream model;
  model << "MAP-S" << get_random_int(100, 999) << "0a";
  return model.str();
}

std::string mapper_system::get_description() {
  /// Return a detailed description of this device
  return "A mapping computer that keeps track of the orbital positions of all "
         "known bodies within the current star system, including planets, "
         "spacecraft, and all known radar and visual contacts."
         "  The display is orthographic."
         "  Capable of rotation and zoom."
         "  Displays object trails relative to a selectable reference frame.";
}

double mapper_system::get_mass() {
  /// Return the weight of the device, in kilograms
  return 46.0;
}

Vector3d mapper_system::get_size() {
  /// Return a size for this object, in metres - hardcoded
  return Vector3d(0.24, 0.12, 0.04);
}

unsigned int mapper_system::get_port_in_count() {
  /// How many input ports it has
  return 4;
}

std::string mapper_system::get_port_in_name(unsigned int port) {
  /// Name of the input port numbered n
  switch(port) {
  case 0:
    return "scale";
  case 1:
    return "rotation around y";
  case 2:
    return "rotation around x";
  case 3:
    return "trail reference frame";
  default:
    return "";
  }
}

std::string mapper_system::get_port_in_description(unsigned int port) {
  /// Description of the input port numbered n
  switch(port) {
  case 0:
    {
      std::stringstream desc;
      desc << "How much to multiply the scale for display."
              "  " << get_port_in_connstatus(port);
      return desc.str();
    }
  case 1:
    {
      std::stringstream desc;
      desc << "Rotation of the display parallel to the ecliptic, in degrees."
              "  " << get_port_in_connstatus(port);
      return desc.str();
    }
  case 2:
    {
      std::stringstream desc;
      desc << "Tilt of the display relative to the ecliptic, in degrees."
              "  " << get_port_in_connstatus(port);
      return desc.str();
    }
  case 3:
    {
      std::stringstream desc;
      desc << "An integer value for which orbital object's reference frame to display trails relative to."
              "  0 is the centre of mass of the system, 1 is the first body out, etc."
              "  " << get_port_in_connstatus(port);
      return desc.str();
    }
  default:
    return "";
  }
}

bool mapper_system::get_port_in_required(unsigned int port __attribute__((__unused__))) {
  /// Whether an input on this port is necessary for this device to operate
  // all inputs are optional
  return false;
}

unsigned int mapper_system::get_port_out_count() {
  /// How many output ports it has
  return 1;
}

std::string mapper_system::get_port_out_name(unsigned int port) {
  /// Name of the output port numbered n
  switch(port) {
  case 0:
    return "analogue video";
  default:
    return "";
  }
}

std::string mapper_system::get_port_out_description(unsigned int port) {
  /// Description of the output port numbered n
  switch(port) {
  case 0:
    return "An analogue video signal showing a diagram of the star system's layout.";
  default:
    return "";
  }
}

GLuint mapper_system::get_port_out_video_analogue(unsigned int port __attribute__((__unused__))) {
  update_if_time();
  return display_image;
}

void mapper_system::refresh() {
  /// Render the star map on an analogue monitor
  centreoffset = Vector3d(windowsize.x / 2.0, windowsize.y / 2.0, 0.0);

  // cache the old viewport
  Vector4i oldviewport;
  glGetIntegerv(GL_VIEWPORT, oldviewport);
  glViewport(0, 0, windowsize.x, windowsize.y);
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, framebuffer);  // bind the framebuffer for the display screen

  glPushAttrib(GL_LIGHTING_BIT);                          // save state - see http://opengl.czweb.org/ch14/462-465.html
  glDisable(GL_LIGHTING);
  //glClearColor(0.2, 0.3, 0.2, 1.0);
  //glClearColor(0.2, 0.2, 0.2, 1.0);
  glClearColor(0.0, 0.0, 0.0, 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  glOrtho(0.0, windowsize.x, windowsize.y, 0.0, -1406000000000.0 * scale, 1406000000000.0 * scale);   // heliopause ~= 1.406 * 10^13
  glTranslated(centreoffset.x, centreoffset.y, centreoffset.z);   // centre on the screen
  glScaled(scale, scale, scale);                                  // zoom
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();
  // translate and rotate us to the camera's viewpoint
  glRotated(rotation_x, 1.0, 0.0, 0.0);                           // rotate the map in 3D
  glRotated(rotation_y, 0.0, 1.0, 0.0);

  glTranslated(-vessel->position.x,
               -vessel->position.y,
               -vessel->position.z);

  // bodies
  glColor4dv(Vector4d(1.0, 1.0, 1.0, 1.0));
  for(auto const &it : root.currentsystem->bodies) {
    Vector3d point  = it->position;
    Vector3d vel    = it->velocity;
    if(vel.length() > 0.0) {
      vel.normalise();
      vel *= 20.0;
    } else {
      vel *= 0.0;
    }

    // line to centre of system
    glColor4dv(Vector4d(0.2, 0.4, 0.2, 1.0));
    glBegin(GL_LINES);
    glVertex2d(0.0, 0.0);
    glVertex3dv(point);
    glEnd();

    // velocity vector
    glColor4dv(Vector4d(0.4, 0.6, 0.4, 1.0));
    glBegin(GL_LINES);
    glVertex3dv(point);
    glVertex3dv(point - vel);
    glEnd();

    // trails
    if(trailcounter == 0) {
      // every period add a trail point
      trailtype trail;
      if(trail_ref) {
        if(it == trail_ref) {     // don't add trails for the (still) reference body
          continue;
        }
        trail.linepoint = point - trail_ref->position;
      } else {
        trail.linepoint = point;
      }
      trail.fade = 1.0;
      trails.push_back(trail);
    }

    it->render_diagram(scale);
  }

  // trails
  if(trail_ref) {
    // if we have a trail reference point, translate the trails to match that
    glTranslated(trail_ref->position.x,
                 trail_ref->position.y,
                 trail_ref->position.z);
  }
  glBegin(GL_POINTS);
  //glBegin(GL_LINES);
  //for(auto it : trails) {
  for(std::deque<trailtype>::iterator it = trails.begin(); it != trails.end();) {
    glColor4dv(Vector4d((it->fade * (2.0 / 3.0)) + 0.1, it->fade, (it->fade * (2.0 / 3.0)) + 0.1, 1.0));
    //glColor4dv(Vector4d(0.5, 1.0, 0.5, it->fade));
    glVertex3dv(it->linepoint);
    //glVertex3dv(Vector3d(0.0, 1000000.0, 0.0));
    it->fade *= 0.99995;
    if(it->fade < 0.3) {
      it = trails.erase(it);
    } else {
      ++it;
    }
  }
  glEnd();
  // trail counter
  if(trailcounter == 0) {
    trailcounter = trailperiod;
  } else {
    --trailcounter;
  }

  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();
  glPopAttrib();

  // release the framebuffer
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glViewport(oldviewport[0], oldviewport[1], oldviewport[2], oldviewport[3]);
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);            // unbind the framebuffer
}

void mapper_system::update() {
  /// Update the output states and respond to changes in input
  // update the scale
  if(ports_in[0].target) {
    scale = ports_in[0].target->get_port_out_data(ports_in[0].target_port);
  } else {
    scale = 0.00001;           // earth scale
    //scale = 0.000000002;       // solar system scale
  }
  // update the rotation
  if(ports_in[1].target) {
    rotation_y = ports_in[1].target->get_port_out_data(ports_in[1].target_port);
  }
  if(ports_in[2].target) {
    rotation_x = ports_in[2].target->get_port_out_data(ports_in[2].target_port);
  }
  // update the trail reference target
  if(ports_in[3].target) {
    double const ntarget_pre = ports_in[3].target->get_port_out_data(ports_in[3].target_port);
    // sanity checks
    if(ntarget_pre >= 1.0) {
      // iterate through the bodies to try to find the nth
      unsigned int n = 1;
      unsigned int const ntarget = static_cast<unsigned int>(ntarget_pre);
      for(auto const &it : root.currentsystem->bodies) {
        if(ntarget == n) {
          std::cout << get_name() << " trails now locked to " << it->get_name() << " reference frame." << std::endl;
          trail_ref = it;
          break;
        }
        ++n;
      }
    } else {
      trail_ref = nullptr;
    }
  } else {
    trail_ref = nullptr;
  }
}

void mapper_system::update_if_time() {
  /// Run the update function only if it's time for an update, and reset the update clock
  boost::chrono::time_point<boost::chrono::high_resolution_clock, boost::chrono::duration<double>> const time_now(boost::chrono::high_resolution_clock::now());
  if(time_now >= time_nextupdate) {
    refresh();
    time_nextupdate = time_now + boost::chrono::duration<double>(boost::chrono::milliseconds(50));  // 20fps
  }
}
