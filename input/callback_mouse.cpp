#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "vmath.h"
#include "astronaut.h"

extern astronaut *player;

void callback_mousepos(GLFWwindow *thiswindow __attribute__((unused)), double x, double z) {
  /// React to mouse position updates
  // Quake mouselook code is here for reference: https://github.com/id-Software/Quake/blob/bf4ac424ce754894ac8f1dae6a3981954bc9852d/WinQuake/in_win.c
  Vector2d const mouse_new(x, z);
  Vector2d mouse_diff = mouse_new - player->mouse_last;
  player->mouse_last = mouse_new;

  // apply sensitivity
  //mouse_diff *= player->mouse_sensitivity;
  mouse_diff *= player->mouse_sensitivity * player->fov_angle;

  // generate a rotation quaternion
  //Quatd temp = Quatd::fromEulerAngles(mouse_diff.y, mouse_diff.x, 0.0);
  //Quatd temp = Quatd::fromAxisRot(Vector3d(1, 0, 0), mouse_diff.y) *
  //             Quatd::fromAxisRot(Vector3d(0, 1, 0), mouse_diff.x);
  //player->rotation_head = temp * player->rotation_head;
  //player->rotation_head.normalise();

  player->rotation_head_yaw   += mouse_diff.x;
  player->rotation_head_pitch += mouse_diff.y;
  if(player->rotation_head_pitch > 80) {
    player->rotation_head_pitch = 80;
  } else if(player->rotation_head_pitch < -80) {
    player->rotation_head_pitch = -80;
  }

}
