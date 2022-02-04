#include "snack.h"

namespace PacmanGame {

Snack::Snack(const glm::vec2 &position) {
  position_ = position;
}

const glm::vec2& Snack::GetPosition() const {
  return position_;
}

void Snack::Draw() {
  ci::gl::color(ci::Color("white"));
  ci::gl::drawSolidCircle(position_, kRadius);
}

} // namespace PacmanGame