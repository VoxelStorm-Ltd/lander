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
universe root;
astronaut *player;

void init_graphics(Vector2i &windowsize);
void init_universe();

void init();
void mainloop();

void pollcontrols(GLFWwindow *thiswindow);

int main() {
  init();
  mainloop();
  return EXIT_SUCCESS;
}

void init() {       /// all the one-time initialisation we need for the engine
  Vector2i windowsize(800, 600);
  init_graphics(windowsize);
  init_universe();
  player->windowsize = windowsize;
  player->update_aspect_ratio();
  std::cout << "Initialisation complete." << std::endl;
  // these must be absolutely last:
  glfwSetTime(0.0);                                                           // reset the timer for the start of the main loop
}

void mainloop() {   /// the main rendering loop
  double deltatime = 5.0;
  //for(;;) {                            // cheap infinite loop
  for(root.time = 0.0; keeprunning; root.time += deltatime) {
    // update the orbits for the orbital bodies in the current system
    for(auto const &it : root.currentsystem->bodies) {
      if(it) {
        it->update_state(root.time, deltatime);
      }
    }

    glfwPollEvents();
    pollcontrols(window_main);

    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    player->render_firstperson();

    glfwSwapBuffers(window_main);
  }
}

