#include <pacman_app.h>

namespace PacmanGame {

PacmanApp::PacmanApp() {
  ci::app::setWindowSize(Map::kWindowWidth, Map::kWindowHeight);
}

void PacmanApp::draw() {
  ci::Color8u background_color(0, 0, 0);  // black
  ci::gl::clear(background_color);
  
  // If the game state is at its END, then we draw the game over screen. 
  // Else, we continue drawing the game
  if (pacman_engine_.GetGameState() == PacmanEngine::GameState::ACTIVE) {
    pacman_engine_.DrawGame();
    ci::gl::drawStringCentered(
        "Score: " + std::to_string(pacman_engine_.GetScore()),
        glm::vec2(Map::kWindowWidth * .9, Map::kWindowHeight * .1),
        ci::Color("white"), ci::Font("Arial", 50));
    
  } else {
    
    // Set what the game over screen is going to be depending on if you lost 
    // or won
    game_over_screen_.SetState(pacman_engine_.GetGameState());
    game_over_screen_.Draw();
  }
}
void PacmanApp::update() {
  // If the game is over, we stop updating the game
  if (pacman_engine_.GetGameState() == PacmanEngine::GameState::ACTIVE) {
    pacman_engine_.UpdateGame();
  } else {
    return;
  }
}

void PacmanApp::keyDown(ci::app::KeyEvent event) {
  switch (event.getCode()) {
    case ci::app::KeyEvent::KEY_LEFT:
      pacman_engine_.MovePacman("left");
      break;

    case ci::app::KeyEvent::KEY_RIGHT:
      pacman_engine_.MovePacman("right");
      break;

    case ci::app::KeyEvent::KEY_UP:
      pacman_engine_.MovePacman("up");
      break;

    case ci::app::KeyEvent::KEY_DOWN:
      pacman_engine_.MovePacman("down");
      break;
      
    case ci::app::KeyEvent::KEY_q:
      std::exit(0);
  }
}
void PacmanApp::setup() {
  pacman_engine_.SetUpGame(kMapPath);
}

} // namespace PacmanGame
