#pragma once
#include "cinder/gl/gl.h"
#include "cinder/app/App.h"
#include "cinder/gl/gl.h"
#include "pacman.h"
#include "pacman_engine.h"
#include "ghost.h"

namespace PacmanGame {

class GameOverScreen {
 public:
  GameOverScreen();
  
  /**
   * Draws the game over screen
   */
  void Draw();

  void SetState(PacmanEngine::GameState state);

 private:
  Pacman pacman_;
  std::vector<Ghost> ghosts_;
  PacmanEngine::GameState state_;
  std::string message_;

  // Change this constant if you want different colored ghosts
  const std::vector<std::string> kGhostColors = {"red", "purple", "orange"};

  // These constants are the first letter of the ghosts' names
  const std::vector<char> kGhostNames = {'B', 'I', 'C'};
  
  
};

} // namespace PacmanGame
