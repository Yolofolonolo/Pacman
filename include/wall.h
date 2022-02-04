#pragma once
#include "cinder/gl/gl.h"
#include "cinder/app/App.h"
#include "cinder/gl/gl.h"


namespace PacmanGame {

class Wall {
 public:
  
  /**
   * Constructs a wall given the top left and bottom right corner positions
   * @param top_left the top left corner position
   * @param bottom_right the bottom right corner position
   */
  Wall(const glm::vec2 &top_left, const glm::vec2 &bottom_right);
  
  /**
   * Draws a wall based on its location 
   */
  void Draw();

  const glm::vec2 &GetTopLeft() const;
  const glm::vec2 &GetBottomRight() const;
 private:
  glm::vec2 top_left_;
  glm::vec2 bottom_right_;
};

} // namespace PacmanGame
