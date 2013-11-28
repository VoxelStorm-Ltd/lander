#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "astronaut.h"

extern astronaut *player;

void pollcontrols(GLFWwindow *thiswindow) {
  //double const amount = 10.0;
  double const amount = 1.001;
  double const angle = 0.15;

  if(glfwGetKey(thiswindow, GLFW_KEY_W) == GLFW_PRESS) {
    //double const oldlength = player->target.length();
    //if(oldlength != 0) {
    //  player->target.normalise();
    //}
    //player->target *= oldlength + amount;
    if(player->target.length() == 0) {
      player->target = Vector3d(0, -1, 0);
    }
    player->target *= amount;
    //std::cout << "New target vector = " << player->target.length() << std::endl;
  }
  if(glfwGetKey(thiswindow, GLFW_KEY_S) == GLFW_PRESS) {
    //double const oldlength = player->target.length();
    //if(oldlength != 0) {
    //  player->target.normalise();
    //}
    //player->target *= oldlength - amount;
    player->target /= amount;
    //std::cout << "New target vector = " << player->target.length() << std::endl;
  }
  if(glfwGetKey(thiswindow, GLFW_KEY_A) == GLFW_PRESS) {
    player->target.rotate(0, 0, -angle);
  }
  if(glfwGetKey(thiswindow, GLFW_KEY_D) == GLFW_PRESS) {
    player->target.rotate(0, 0, angle);
  }
  if(glfwGetKey(thiswindow, GLFW_KEY_SPACE) == GLFW_PRESS) {
    player->velocity += player->target;
    std::cout << "Applying delta V " << player->target.length() << "m/s^2" << std::endl;
  }
}
