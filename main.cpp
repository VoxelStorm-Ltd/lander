#include <cstdlib>
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "vmath.h"
#include "universe.h"
#include "starsystem.h"
#include "body.h"
#include "astronaut.h"
#include "spacecraft.h"

// globals
bool keeprunning = true;
GLFWwindow *window_main;      // the main game window
int const extern windowwidth  = 800.0;
int const extern windowheight = 800.0;
//double const aspect_ratio = 1.0;
double const aspect_ratio = static_cast<double>(windowheight) / static_cast<double>(windowwidth);
//double const aspect_ratio = static_cast<double>(windowwidth) / static_cast<double>(windowheight);
double const fov = 90.0;
//double const fov_ratio = 1.0;
double const fov_ratio = tan(fov / 360.0 * M_PI);
universe root;
astronaut *player;

Vector3d cameracentre;

void init_graphics();
void init_universe();

void init();
void mainloop();

void pollcontrols(        GLFWwindow *thiswindow);
void callback_mousepos(   GLFWwindow *thiswindow, double x, double z);
void callback_key(        GLFWwindow *thiswindow, int key, int scancode __attribute__((unused)), int action, int mods);
void callback_windowclose(GLFWwindow *thiswindow);
void callback_scroll(     GLFWwindow *thiswindow, double xoffset, double yoffset);

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
  double deltatime = 1.0;
  //for(;;) {                            // cheap infinite loop
  for(root.time = 0.0; keeprunning; root.time += deltatime) {
    // update the orbits for the orbital bodies in the current system
    for(auto const &it : root.currentsystem->bodies) {
      if(it) {
        it->update_state(root.time, deltatime);
      }
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

    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    double const nearplane = 0.1;
    double const farplane  = 20.0;
    glFrustum(nearplane * -fov_ratio,
              nearplane *  fov_ratio,
              nearplane * -fov_ratio * aspect_ratio,
              nearplane *  fov_ratio * aspect_ratio,
              nearplane, farplane);
    // TODO: cache this matrix at init, and load it here

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    if(player->state == astronaut::statetype::IN_VESSEL) {
      // translate and rotate to player's view in the cockpit
      glTranslated(-player->position.x,
                   -player->position.y,
                   -player->position.z);
      // draw cockpit view of the current ship
      player->vessel_in->render_cockpit();
    } else if(player->state == astronaut::statetype::EVA) {
      // translate and rotate to player's view in the universe
      glTranslated(-player->position.x,
                   -player->position.y,
                   -player->position.z);
      // draw first person view from outside
      // TODO
    }
    //root.render();

    glfwSwapBuffers(window_main);

    // DEBUG ONLY:
    //while(true);
  }
}
