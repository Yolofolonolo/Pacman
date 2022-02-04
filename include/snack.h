#pragma once
#include "cinder/gl/gl.h"
#include "cinder/app/App.h"
#include "cinder/gl/gl.h"

namespace PacmanGame {

class Snack {
 public:
  explicit Snack(const glm::vec2 &position);
  const glm::vec2 &GetPosition() const;

  /**
   * Draws a snack based on its position 
   */
  void Draw();
  
  const static size_t kRadius = 3;
 private:
  glm::vec2 position_;
  
  
};

} // namespace PacmanGame
