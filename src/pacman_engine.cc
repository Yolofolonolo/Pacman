#include "pacman_engine.h"

namespace PacmanGame {

PacmanEngine::PacmanEngine() {
  begin_time_ = std::chrono::system_clock::now();
  score_ = 0;
  lives_ = 3;
  pacman_state_ = ALIVE;
  game_state_ = ACTIVE;
}

void PacmanEngine::SetUpGame(const std::string& map) {
  map_.LoadInMap(map);
  pacman_.SetPosition(map_.GetPacmanLocation());
  const std::vector<glm::vec2>& ghost_locations = map_.GetGhostLocations();

  // Create ghosts based on their locations and color
  for (size_t i = 0; i < ghost_locations.size(); i++) {
    ghosts_.emplace_back(ghost_locations[i], kGhostColors[i], kGhostNames[i]);
    ghosts_[i].GetInitialGhostCoordinates(map_.GetMapTiles());
  }
  
  // Get the map tiles and store it
  map_tiles_ = map_.GetMapTiles();
  GetInitialPacmanCoordinates();
}

void PacmanEngine::GetInitialPacmanCoordinates() {
  for (size_t r = 0; r < map_tiles_.size(); r++) {
    for (size_t c = 0; c < map_tiles_[r].size(); c++) {
      if (map_tiles_[r][c] == Map::kPacman) {
        pacman_coordinates_ = initial_pacman_coordinates_ = std::make_pair(r, c);
        break;
      }
    }
  }
}

void PacmanEngine::DrawGame() {
  map_.DrawMap(begin_time_);
  
  for (Ghost& ghost: ghosts_) {
    ghost.Draw();
  }
  
  // Only draw Pacman if he's alive
  if (pacman_state_ == ALIVE) {
    pacman_.Draw();
  }
  DrawLives();
}

void PacmanEngine::DrawLives() const {
  ci::gl::drawStringCentered(
      "Lives: ",
      glm::vec2(Map::kWindowWidth * .88, Map::kWindowHeight * .3),
      ci::Color("white"), ci::Font("Arial", 50));
  
  for (size_t i = 0; i < lives_; i++) {
    
    // Draw's the number of lives as Pacman's to the screen
    ci::gl::color(ci::Color("yellow"));
    glm::vec2 center = glm::vec2(Map::kWindowWidth * .85 + 95 * i,
                                 Map::kWindowHeight * .4);
    ci::gl::drawSolidCircle(center,pacman_.GetRadius() * 1.5);

    ci::gl::color(ci::Color("black"));
    glm::vec2 pt2 = glm::vec2(center.x + pacman_.GetRadius() * 2, center.y +
        pacman_.GetRadius());
    glm::vec2 pt3 = glm::vec2(center.x + pacman_.GetRadius() * 2, center.y -
        pacman_.GetRadius());
    ci::gl::drawSolidTriangle(center, pt2, pt3);
  }
}

void PacmanEngine::RespawnPacman() {
  pacman_state_ = ALIVE;
  // Set Pacman's coordinates back to the initial coordinates
  pacman_.SetPosition(map_.GetPacmanLocation());
}

void PacmanEngine::MovePacman(const std::string& direction) {
  // Makes sure you can't move when you are dead
  if (pacman_state_ == DEAD) {
    return;
  }
  
  // Change the coordinates and moves the position of Pacman
  if (direction == "left") {
    if (IsLeftCollision()) {
      return;
    }
    pacman_coordinates_.second--;
    pacman_.MoveLeft();
    
  } else if (direction == "right") {
    if (IsRightCollision()) {
      return;
    }
    pacman_coordinates_.second++;
    pacman_.MoveRight();
    
  } else if (direction == "up") {
    if (IsUpCollision()) {
      return;
    }
    pacman_coordinates_.first--;
    pacman_.MoveUp();

  } else if (direction == "down") {
    if (IsDownCollision()) {
      return;
    }
    pacman_coordinates_.first++;
    pacman_.MoveDown();
  }

  if (HasEatenSnack()) {
    score_++;
  }
  
  if (HasEatenPowerUp()) {
    pacman_.SetState(Pacman::POWERED_UP);
    for (Ghost& ghost : ghosts_) {
      ghost.SetState(Ghost::FRIGHTEN);
    } 
  }
}

void PacmanEngine::UpdateGame() {

  // First check if the game is over (there are no more snacks) 
  if (map_.GetSnacks().empty()) {
    game_state_ = WON;
    return;
  }
  
  // Next check if Pacman is powered up, if so, check if it's been the power
  // up duration yet
  UpdatePacmanState();
  
  for (Ghost& ghost : ghosts_) {
    ghost.Update(map_.GetMapTiles(), pacman_coordinates_, map_);
    
    // If Pacman isn't powered up, then if pacman is on a ghost, then Pacman 
    // has died. If pacman is powered up, then the ghost has died
    if (HasDied(ghost) && pacman_state_ != DEAD) {
      if (pacman_.GetState() == Pacman::NORMAL) {
        
        // Set Pacman's coordinates back to initial ones
        pacman_coordinates_ = initial_pacman_coordinates_;
        lives_--;

        // If we have no more lives, the game state is over
        if (lives_ == 0) {
          game_state_ = END;
        }
        pacman_state_ = DEAD;
        ScatterGhosts();
        
        // We've eaten a ghost 
      } else {
        score_ += 100;
        ghost.Respawn();
      } 
    }
  }
}

void PacmanEngine::UpdatePacmanState() {
  TimePoint end_time = std::chrono::system_clock::now();
  std::chrono::duration<double> elapsed_time = end_time - begin_time_;
  if (pacman_.GetState() == Pacman::POWERED_UP) {
    
    // If Pacman is powered up, only power up for a set duration 
    if ((int) elapsed_time.count() % kPowerUpDuration == 0) {
      pacman_.SetState(Pacman::NORMAL);
      for (Ghost& ghost : ghosts_) {
        ghost.SetState(Ghost::CHASE);
      }
    }
    
  } else if (pacman_state_ == DEAD) {
    if ((int) elapsed_time.count() % kPowerUpDuration == 0) {
      RespawnPacman();
    }
  }
}

void PacmanEngine::ScatterGhosts() {
  for (Ghost& ghost : ghosts_) {
    ghost.SetState(Ghost::SCATTER);
  }
}

bool PacmanEngine::HasDied(const Ghost& ghost) {
  if (pacman_coordinates_ == ghost.GetGhostCoordinates()) {
    return true;
  }
  return false;
}

bool PacmanEngine::IsUpCollision() {
  
  // Checks if there is a wall in the tile above Pacman 
  Coordinates up = Coordinates(pacman_coordinates_.first - 1,
                               pacman_coordinates_.second);
  if (map_tiles_[up.first][up.second] == Map::kWall) {
    return true;
  }
  return false;
}

bool PacmanEngine::IsDownCollision() {
  Coordinates up = Coordinates(pacman_coordinates_.first + 1, 
                               pacman_coordinates_.second);
  if (map_tiles_[up.first][up.second] == Map::kWall) {
    return true;
  }
  return false;
}

bool PacmanEngine::IsLeftCollision() {
  Coordinates left = Coordinates(pacman_coordinates_.first,
                               pacman_coordinates_.second - 1);
  if (map_tiles_[left.first][left.second] == Map::kWall) {
    return true;
  }
  return false;
}

bool PacmanEngine::IsRightCollision() {
  Coordinates right = Coordinates(pacman_coordinates_.first,
                               pacman_coordinates_.second + 1);
  if (map_tiles_[right.first][right.second] == Map::kWall) {
    return true;
  }
  return false;
}

bool PacmanEngine::HasEatenSnack() {
  std::vector<Snack>& all_snacks = map_.GetSnacks();
  glm::vec2 curr_pos = pacman_.GetPosition();
  
  for (size_t i = 0; i < all_snacks.size(); i++) {
    
    // Checks if the distance between Pacman and any given snack is less than
    // the sum of the two radii. If they are, then Pacman has eaten it and we
    // erase it from the vector of snacks 
    if (glm::distance(all_snacks[i].GetPosition(), curr_pos) <= 
    Snack::kRadius + pacman_.GetRadius()) {
      all_snacks.erase(all_snacks.begin() + i);
      return true;
    }
  }
  return false;
}

bool PacmanEngine::HasEatenPowerUp() {
  std::vector<PowerUp>& all_power_ups = map_.GetPowerUps();
  glm::vec2 curr_pos = pacman_.GetPosition();

  for (size_t i = 0; i < all_power_ups.size(); i++) {
    if (glm::distance(all_power_ups[i].GetPosition(), curr_pos) <=
        Snack::kRadius + pacman_.GetRadius()) {
      all_power_ups.erase(all_power_ups.begin() + i);
      return true;
    }
  }
  return false;
}

size_t PacmanEngine::GetScore() const {
  return score_;
}

const Map &PacmanEngine::GetMap() const {
  return map_;
}
const Pacman &PacmanEngine::GetPacman() const {
  return pacman_;
}
const std::vector<Ghost> &PacmanEngine::GetGhosts() const {
  return ghosts_;
}
const std::pair<size_t, size_t> &PacmanEngine::GetPacmanCoordinates() const {
  return pacman_coordinates_;
}
PacmanEngine::GameState PacmanEngine::GetGameState() const {
  return game_state_;
}
const size_t &PacmanEngine::GetLives() const {
  return lives_;
}
PacmanEngine::PacmanState PacmanEngine::GetPacmanState() const {
  return pacman_state_;
}

} // namespace PacmanGame