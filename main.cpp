#include <cstdlib>
#include <iostream>
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

//void pollcontrols(        GLFWwindow *thiswindow);
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

  glfwSetWindowTitle(window_main, "Lander alpha");                            // set the title to the main run's title
  std::cout << "Initialisation complete." << std::endl;

  // these must be absolutely last:
  glfwSetTime(0.0);                                                           // reset the timer for the start of the main loop
}

void mainloop() {   /// the main rendering loop
  double dt = 1000.0;
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
    //pollcontrols(window_main);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // translate us to the camera's viewpoint
    //glRotated(freecam->pitch, 1, 0, 0);
    //glRotated(freecam->yaw,   0, 1, 0);
    //glTranslated(-freecam->coords.x,
    //             -freecam->coords.y,
    //             -freecam->coords.z);


    // TESTING ONLY
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, windowwidth, windowheight, 0.0, 0.0, 1.0);
    glMatrixMode (GL_MODELVIEW);
    glDisable(GL_DEPTH_TEST);

    glColor3dv(Vector3d(1.0, 1.0, 1.0));
    for(auto const &it : root.currentsystem->bodies) {
      //double scale = 0.00001;           // earth scale
      double scale = 0.000000002;       // solar system scale
      Vector2d centreoffset = Vector2d(windowwidth / 2.0, windowheight / 2.0);
      Vector2d point = Vector2d(it->position.x, it->position.z);
      Vector2d vel   = Vector2d(it->velocity.x, it->velocity.z);
      if(vel.length() > 0) {
        vel.normalise();
        vel *= 20;
      } else {
        vel *= 0;
      }

      glColor3dv(Vector3d(0.4, 0.6, 0.4));
      glBegin(GL_LINES);
      glVertex2dv(centreoffset);
      glVertex2dv((point * scale) + centreoffset);
      glEnd();

      // velocity vector
      glColor3dv(Vector3d(0.2, 1.0, 0.2));
      glBegin(GL_LINES);
      glVertex2dv((point * scale) + centreoffset);
      glVertex2dv(((point * scale) + centreoffset) -vel);
      glEnd();

      // draw a circle at the radius
      double thisradius = it->get_radius();
      if(thisradius * scale < 2) {
        thisradius = 2 / scale;
      }
      glColor3dv(Vector3d(1.0, 1.0, 1.0));
      glBegin(GL_LINE_LOOP);
      for(double angle = 0.0; angle <= M_PI * 2; angle += M_PI / 16) {
        Vector2d circle_edge = Vector2d(point);
        circle_edge.x += sin(angle) * thisradius;
        circle_edge.y += cos(angle) * thisradius;
        glVertex2dv((circle_edge * scale) + centreoffset);
      }
      glEnd();
    }

    //root.render();

    glfwSwapBuffers(window_main);
  }
}
