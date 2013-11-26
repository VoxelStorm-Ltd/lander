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
int windowwidth = 600;
int windowheight = 400;
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
  double dt = 1;
  //for(;;) {                            // cheap infinite loop
  for(root.time = 0; root.time != 3600000; root.time += dt) {
    // update the orbits for the orbital bodies in the current system
    for(auto const &it : root.currentsystem->bodies) {
      it->update_state(root.time, dt);
    }
    std::cout << "DEBUG: " << root.time << " : " << Vector3d(player->position - player->walking_on->position).length() - player->walking_on->get_radius() << ", " << (player->velocity - player->walking_on->velocity).length() << "m/s" << std::endl;
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
    root.render();

    glfwSwapBuffers(window_main);
  }
}
