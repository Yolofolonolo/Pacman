#pragma once
#include "cinder/gl/gl.h"
#include "cinder/app/App.h"
#include "cinder/gl/gl.h"
#include "map.h"

namespace PacmanGame {

// Used to get the time while running the application 
typedef std::chrono::time_point<std::chrono::system_clock> TimePoint;

class Pacman {
 public:
  
  /**
   * Constructs an instance of Pacman
   */
  Pacman();
  
  /**
   * Draws Pacman
   */
  void Draw();
  
  /**
   * These methods move Pacman
   */
  void MoveUp();
  void MoveDown();
  void MoveLeft();
  void MoveRight();

  void SetPosition(const glm::vec2 &position);
  const glm::vec2 &GetPosition() const;
  double GetRadius() const;
  void SetRadius(double radius);

  enum State {NORMAL, POWERED_UP};

  State GetState() const;
  void SetState(State state);

  constexpr static double kSpeed = 40;
  
 private:
  enum Direction {UP, DOWN, LEFT, RIGHT};
  Direction direction_;
  TimePoint begin_time_;
  glm::vec2 position_;
  double radius_;
  State state_;
  
};

} // namespace Pacman