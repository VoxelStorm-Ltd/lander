#include "mapper_system.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "universe.h"
#include "starsystem.h"
#include "spacecraft.h"

extern universe root;

mapper_system::mapper_system()
  //: scale(0.00001),           // earth scale
  : scale(0.000000002),       // solar system scale
    trail_ref(nullptr) {
  /// Default constructor
  ports_in.resize(get_port_in_count());     // anything with input ports needs this
}

mapper_system::~mapper_system() {
  /// Default destructor
}

std::string mapper_system::get_name() {
  /// Return the name of this device
  return "star system mapper";
}

std::string mapper_system::get_description() {
  /// Return a detailed description of this device
  random_reset();
  std::stringstream desc;
  desc << get_random_name_corporation() << " model MAP-S" << get_random_int(100, 999) << "0a."
       << "  A mapping computer that keeps track of the orbital positions of all "
          "known bodies within the current star system, including planets, "
          "spacecraft, and all known radar and visual contacts."
          "  The display is orthographic."
          "  Capable of rotation and zoom."
          "  Displays object trails relative to a selectable reference frame.";
  return desc.str();
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

void mapper_system::get_port_out_video_analogue(unsigned int port __attribute__((__unused__)),
                                                GLuint framebuffer,
                                                Vector2i windowsize) {
  /// Render the star map on an analogue monitor
  centreoffset = Vector3d(windowsize.x / 2.0, windowsize.y / 2.0, 0.0);

  glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
  glViewport(0, 0, windowsize.x, windowsize.y);

  glClearColor(0.2, 0.3, 0.2, 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  glOrtho(0.0, windowsize.x, windowsize.y, 0.0, -1406000000000.0 * scale, 1406000000000.0 * scale);   // heliopause ~= 1.406 * 10^13
  glTranslated(centreoffset.x, centreoffset.y, centreoffset.z);   // centre on the screen
  glScaled(scale, scale, scale);                                  // zoom
  //glRotated(-45.0, 1.0, 0.0, 0.0);                                // tilt projection plane
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();
  // translate and rotate us to the camera's viewpoint
  glRotated(-45, 1.0, 0.0, 0.0);
  //glRotated(45.0, 0.0, 1.0, 0.0);

  glTranslated(-vessel->position.x,
               -vessel->position.y,
               -vessel->position.z);

  // trails
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
      trail.linepoint = point;
      trail.fade = 1.0;
      trails.push_back(trail);
    }

    it->render_diagram(scale);
  }

  // trails
  if(trailcounter == 0) {
    trailcounter = trailperiod;
  } else {
    --trailcounter;
  }

  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void mapper_system::update() {
  /// Update the output states and respond to changes in input
  // update the scale
  if(ports_in[0].target) {
    scale = ports_in[0].target->get_port_out_data(ports_in[0].target_port);
  } else {
    //scale = 0.00001;           // earth scale
    scale = 0.000000002;       // solar system scale
  }
  // update the rotation
  double rotation_y = 0.0;
  double rotation_x = 0.0;
  if(ports_in[1].target) {
    rotation_y = ports_in[2].target->get_port_out_data(ports_in[2].target_port);
  }
  if(ports_in[2].target) {
    rotation_x = ports_in[1].target->get_port_out_data(ports_in[1].target_port);
  }
  rotation = Matrix4f::createRotationAroundAxis(rotation_x, rotation_y, 0.0);
  // update the trail reference target
  if(ports_in[3].target) {
    double const ntarget_pre = ports_in[2].target->get_port_out_data(ports_in[2].target_port);
    // sanity checks
    if(ntarget_pre >= 1.0) {
      // iterate through the bodies to try to find the nth
      unsigned int n = 1;
      unsigned int const ntarget = static_cast<unsigned int>(ntarget_pre);
      for(auto const &it : root.currentsystem->bodies) {
        if(ntarget == n) {
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
