#pragma once
#include "cinder/gl/gl.h"
#include "cinder/app/App.h"
#include "cinder/gl/gl.h"

namespace PacmanGame {

class PowerUp {
 public:
  /**
   * Constructs a power up given a position
   * @param position the position of the power up
   */
  PowerUp(const glm::vec2 &position);
  
  /**
   * Draws a power up based on its location 
   */
  void Draw();
  
  const glm::vec2 &GetPosition() const;

 private:
  glm::vec2 position_;
  
  const static size_t kRadius = 6;
  
};

} // namespace PacmanGame
