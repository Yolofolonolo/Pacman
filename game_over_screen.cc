#include <pacman_engine.h>
#include "game_over_screen.h"

namespace PacmanGame {

GameOverScreen::GameOverScreen() {
  for (size_t i = 0; i < kGhostNames.size(); i++) {
    ghosts_.emplace_back(glm::vec2(Map::kWindowWidth * .4 + 200 * i , 
                                  Map::kWindowHeight * .5), 
                         kGhostColors[i], kGhostNames[i]);
    ghosts_[i].SetSize(60);
  }
  
  // Enlarges Pacman and the ghosts to be bigger on the game over screen
  pacman_.SetPosition(glm::vec2(Map::kWindowWidth * .25, Map::kWindowHeight * 
  .5));
  pacman_.SetRadius(60);
}

void GameOverScreen::Draw() {
  ci::gl::drawStringCentered(
      message_,
      glm::vec2(Map::kWindowWidth * .5, Map::kWindowHeight * .65),
      ci::Color("white"), ci::Font("Arial", 60));

  ci::gl::drawStringCentered(
      "Press Q to quit!",
      glm::vec2(Map::kWindowWidth * .5, Map::kWindowHeight * .75),
      ci::Color("white"), ci::Font("Arial", 60));
  
  pacman_.Draw();
  for (Ghost& ghost : ghosts_) {
    ghost.Draw();
  }
}
void GameOverScreen::SetState(PacmanEngine::GameState state) {
  state_ = state;

  // Change the message depending on what the game state is after the game 
  if (state_ == PacmanEngine::END) {
    message_ = "Game over!";
  } else {
    message_ = "You won!";
  }
}

} // namespace PacmanGame