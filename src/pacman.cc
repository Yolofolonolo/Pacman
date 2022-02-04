#include <pacman.h>

namespace PacmanGame {

Pacman::Pacman() {
  begin_time_ = std::chrono::system_clock::now();
  direction_ = LEFT;
  radius_ = Map::kWindowHeight / 60;
  state_ = NORMAL;
}

void Pacman::Draw() {
  ci::gl::color(ci::Color("yellow"));
  ci::gl::drawSolidCircle(position_, radius_);
  ci::gl::color(ci::Color("black"));
  
  // Gets the elapsed time of the program to draw the biting animation for 
  // Pacman
  auto end_time = std::chrono::system_clock::now();
  std::chrono::duration<double> duration = end_time - begin_time_;
  size_t elapsed_time = duration.count() * 10;
  size_t scaling_factor = elapsed_time % ((int) radius_ / 2);

  // Gets the direction of Pacman and draws a black triangle tha represents 
  // the mouth of Pacman
  switch (direction_) {
    case UP: {
      glm::vec2 pt2 = glm::vec2(position_.x - scaling_factor, position_.y - radius_);
      glm::vec2 pt3 = glm::vec2(position_.x + scaling_factor, position_.y - radius_);
      ci::gl::drawSolidTriangle(position_, pt2, pt3);
      break;
    }
    
    case DOWN: {
      glm::vec2 pt2 = glm::vec2(position_.x - scaling_factor, position_.y + radius_);
      glm::vec2 pt3 = glm::vec2(position_.x + scaling_factor, position_.y + radius_);
      ci::gl::drawSolidTriangle(position_, pt2, pt3);
      break;
    }
    
    case LEFT: {
      glm::vec2 pt2 = glm::vec2(position_.x - radius_, position_.y + scaling_factor);
      glm::vec2 pt3 = glm::vec2(position_.x - radius_, position_.y - scaling_factor);
      ci::gl::drawSolidTriangle(position_, pt2, pt3);
      break;
    }
    
    case RIGHT: {
      glm::vec2 pt2 = glm::vec2(position_.x + radius_, position_.y + scaling_factor);
      glm::vec2 pt3 = glm::vec2(position_.x + radius_, position_.y - scaling_factor);
      ci::gl::drawSolidTriangle(position_, pt2, pt3);
      break;
    }
  }
}

void Pacman::MoveUp() {
  position_.y -= kSpeed;
  direction_ = UP;
}

void Pacman::MoveDown() {
  position_.y += kSpeed;
  direction_ = DOWN;
}

void Pacman::MoveLeft() {
  position_.x -= kSpeed;
  direction_ = LEFT;
}

void Pacman::MoveRight() {
  position_.x += kSpeed;
  direction_ = RIGHT;
}

void Pacman::SetPosition(const glm::vec2 &position) {
  position_ = position;
}

const glm::vec2 &Pacman::GetPosition() const {
  return position_;
}
void Pacman::SetRadius(double radius) {
  radius_ = radius;
}
double Pacman::GetRadius() const {
  return radius_;
}
void Pacman::SetState(Pacman::State state) {
  state_ = state;
}
Pacman::State Pacman::GetState() const {
  return state_;
}

} // namespace PacmanGame

