#include "power_up.h"

namespace PacmanGame {

PowerUp::PowerUp(const glm::vec2 &position) {
  position_ = position;
}

const glm::vec2& PowerUp::GetPosition() const {
  return position_;
}

void PowerUp::Draw() {
  ci::gl::color(ci::Color("yellow"));
  ci::gl::drawSolidCircle(position_, kRadius);
}

} // namespace PacmanGame