#include <cstdlib>
#include <iostream>
#include <deque>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "vmath.h"
#include "universe.h"
#include "starsystem.h"
#include "planet.h"
#include "astronaut.h"

// globals
bool keeprunning = true;
GLFWwindow *window_main;      // the main game window
int windowwidth = 800;
int windowheight = 800;
universe root;
astronaut *player;

void init_graphics();
void init_universe();

void init();
void mainloop();

void pollcontrols(        GLFWwindow *thiswindow);
void callback_mousepos(   GLFWwindow *thiswindow, double x, double z);
void callback_key(        GLFWwindow *thiswindow, int key, int scancode __attribute__((unused)), int action, int mods);
void callback_windowclose(GLFWwindow *thiswindow __attribute__((unused)));

int main() {
  init();
  mainloop();
  return EXIT_SUCCESS;
}

void init() {       /// all the one-time initialisation we need for the engine
  init_graphics();
  init_universe();
  std::cout << "Initialisation complete." << std::endl;
  // these must be absolutely last:
  glfwSetTime(0.0);                                                           // reset the timer for the start of the main loop
}

void mainloop() {   /// the main rendering loop
  struct trailtype {
    Vector2d linepoint;
    double fade;
  };
  unsigned int const trailperiod = 60;       // draw a dash in this many frames
  unsigned int trailcounter = trailperiod;

  std::deque<trailtype> trails;

  Vector3d centreoffset = Vector3d(windowwidth / 2.0, windowheight / 2.0, 0.0);
  double scale = 0.00001;           // earth scale
  //double scale = 0.000000002;       // solar system scale

  double dt = 1.0;
  //for(;;) {                            // cheap infinite loop
  for(root.time = 0; root.time != -1; root.time += dt) {
    // update the orbits for the orbital bodies in the current system
    for(auto const &it : root.currentsystem->bodies) {
      it->update_state(root.time, dt);
    }
    //std::cout << "DEBUG: " << root.time << " : " << Vector3d(player->position - player->walking_on->position).length() - player->walking_on->get_radius() << ", " << (player->velocity - player->walking_on->velocity).length() << "m/s" << std::endl;
    //std::cout << "DEBUG1: " << player->velocity << std::endl;
    //std::cout << "DEBUG2: " << player->walking_on->velocity << std::endl;
    //std::cout << "DEBUG: " << root.time << " ";
    //for(auto *it : root.currentsystem->bodies) {
    //  std::cout << it->name << ": " << it->position.x << " ";
    //}
    //std::cout << std::endl;

    glfwPollEvents();
    pollcontrols(window_main);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // TESTING ONLY
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, windowwidth, windowheight, 0.0, 0.0, 1.0);
    glTranslated(centreoffset.x, centreoffset.y, centreoffset.z);
    glScaled(scale, scale, scale);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    // translate us to the camera's viewpoint
    //glRotated(freecam->pitch, 1, 0, 0);
    //glRotated(freecam->yaw,   0, 1, 0);
    //glTranslated(-freecam->coords.x,
    //             -freecam->coords.y,
    //             -freecam->coords.z);
    //glTranslated(-player->position.x,
    //             -player->position.y,
    //             -player->position.z);
    glTranslated(0,
                 400,
                 0);
    // trails
    glBegin(GL_POINTS);
    //for(auto it : trails) {
    for(std::deque<trailtype>::iterator it = trails.begin(); it != trails.end();) {
      glColor4dv(Vector4d((it->fade * 0.5) + 0.1, it->fade, (it->fade * 0.5) + 0.1, 1.0));
      //glColor4dv(Vector4d(0.5, 1.0, 0.5, it->fade));
      glVertex2dv(it->linepoint);
      it->fade *= 0.99995;
      if(it->fade < 0.2) {
        it = trails.erase(it);
      } else {
        ++it;
      }
    }
    glEnd();

    // bodies
    glColor4dv(Vector4d(1.0, 1.0, 1.0, 1.0));
    for(auto const &it : root.currentsystem->bodies) {
      Vector2d point  = Vector2d(it->position.x, it->position.y);
      Vector2d vel    = Vector2d(it->velocity.x, it->velocity.y);
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
      glVertex2dv(point);
      glEnd();

      // velocity vector
      glColor4dv(Vector4d(0.4, 0.6, 0.4, 1.0));
      glBegin(GL_LINES);
      glVertex2dv(point);
      glVertex2dv(point - vel);
      glEnd();

      // target vector
      astronaut *thisastro = dynamic_cast<astronaut*>(it);
      if(thisastro) {
        // if the cast worked, this runs - otherwise nullptr
        Vector2d target = Vector2d(thisastro->target.x, thisastro->target.y) * 1000000;
        glColor4dv(Vector4d(1.0, 0.6, 0.2, 1.0));
        glBegin(GL_LINES);
        glVertex2dv(point);
        glVertex2dv(point + target);
        glEnd();

        // trails
        if(trailcounter == 0) {
          // every period add a trail point
          trailtype trail;
          trail.linepoint.x = point.x;
          trail.linepoint.y = point.y;
          trail.fade = 1.0;
          trails.push_back(trail);
        }
      }

      // draw a circle at the radius
      double thisradius = it->get_radius();
      //double thisradius = it->get_radius() * 10;    // exaggerate the radius
      if(thisradius * scale < 1) {
        thisradius = 1 / scale;
      }
      glColor4dv(Vector4d(1.0, 1.0, 1.0, 1.0));
      glBegin(GL_LINE_LOOP);
      for(double angle = 0.0; angle <= M_PI * 2; angle += M_PI / 16) {
        Vector2d circle_edge = Vector2d(point);
        circle_edge.x += sin(angle) * thisradius;
        circle_edge.y += cos(angle) * thisradius;
        glVertex2dv(circle_edge);
      }
      glEnd();
      glColor4dv(Vector4d(0.2, 0.5, 0.5, 1.0));
      glBegin(GL_LINES);
      for(double angle = 0.0; angle <= M_PI * 2; angle += M_PI / 16) {
        Vector2d circle_edge = Vector2d(point);
        circle_edge.x += sin(angle) * (thisradius + 400000);
        circle_edge.y += cos(angle) * (thisradius + 400000);
        glVertex2dv(circle_edge);
      }
      glEnd();

    }

    // auto scale
    if(Vector2d(player->position.x, player->position.y).length() * scale > windowheight / 2) {
      // zoom out
      std::cout << player->position.length() << "m out, scale " << scale << std::endl;
      scale /= 2;
    } else if(Vector2d(player->position.x, player->position.y).length() * scale < windowheight / 4) {
      // zoom in
      std::cout << player->position.length() << "m out, scale " << scale << std::endl;
      scale *= 2;
    }

    // trails
    if(trailcounter == 0) {
      trailcounter = trailperiod;
    } else {
      --trailcounter;
    }

    //root.render();

    glfwSwapBuffers(window_main);
  }
}
