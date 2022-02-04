#include "wall.h"

namespace PacmanGame {

Wall::Wall(const glm::vec2 &top_left, const glm::vec2 &bottom_right) {
  top_left_ = top_left;
  bottom_right_ = bottom_right;
}

void Wall::Draw() {
  ci::gl::color(ci::Color("blue"));
  ci::Rectf wall(top_left_, bottom_right_);
  ci::gl::drawSolidRect(wall);
}

const glm::vec2 &Wall::GetTopLeft() const {
  return top_left_;
}
const glm::vec2 &Wall::GetBottomRight() const {
  return bottom_right_;
}

} // namespace PacmanGame