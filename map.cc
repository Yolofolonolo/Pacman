#include <map.h>

namespace PacmanGame {

Map::Map() {}

void Map::LoadInMap(const std::string &map) {
  std::ifstream map_loader(map);
  std::string line;
  size_t row = 0;
  size_t scaled_row = 0;

  // Read in the lines from the file 
  while (std::getline(map_loader, line)) {
    
    // Create a vector of chars
    map_tiles_.emplace_back();
    for (size_t column = 0, scaled_column = 0;
        column < line.size(); column++, scaled_column += kLoopingFactor) {
      
      // Push back onto that vector the line of chars
      map_tiles_[row].push_back(line.at(column));
      
      // Find placement of a drawn structure based on what column and row we 
      // are at in the map txt file 
      glm::vec2 top_left = glm::vec2(scaled_column * kScalingFactor, scaled_row *
          kScalingFactor + kScalingFactor);
      glm::vec2 bottom_right = glm::vec2(scaled_column * kScalingFactor +
          1.5 * kScalingFactor, scaled_row * kScalingFactor + 2.5 * 
          kScalingFactor);

      // The reason for these 2s and 3s is because we are taking an 
      // average so making them constants to prevent magic numbers didn't
      // make sense here 
      glm::vec2 pos = glm::vec2((2 * (scaled_column * kScalingFactor) +
          kScalingFactor) / 2, (2 * (scaled_row * kScalingFactor) + 3 *
          kScalingFactor) / 2);

      // Switch case statement for the current char on the line to determine 
      // what to draw 
      // Find placement of where to draw the items based on what column and row
      // we are at in the map txt file and the scaling factor 
      switch (line.at(column)) {
        case kWall: {
          walls_.emplace_back(top_left, bottom_right);
          break;
        }

        case kSnack: {
          snacks_.emplace_back(pos);
          break;
        }

        case kPowerUp: {
          power_ups_.emplace_back(pos);
          break;
        }
          
        case kPacman: {
          pacman_location_ = pos;
          break;
        }

        case kBlinky: {
          ghost_locations_.emplace_back(pos);
          break;
        }
        
        case kInky: {
          ghost_locations_.emplace_back(pos);
          break;
        }
        
        case kClyde: {
          ghost_locations_.emplace_back(pos);
          break;
        }

        case kTempWall: {
          temp_walls_.emplace_back(top_left, bottom_right);
        }
      }
    }
    row++;
    scaled_row += kLoopingFactor; 
  }
}

void Map::DrawMap(const TimePoint& begin_time) {
  for (Wall& wall : walls_) {
    wall.Draw();
  }
  
  for (Snack& snack : snacks_) {
    snack.Draw();
  }
  
  for (PowerUp& power_up : power_ups_) {
    power_up.Draw();
  }

  // Gets the elapsed time of the program to check if we stop drawing the 
  // temporary walls on the map for the ghost chamber 
  auto end_time = std::chrono::system_clock::now();
  std::chrono::duration<double> elapsed_time = end_time - begin_time;
  
  if (elapsed_time.count() < kTempWallDuration) {
    for (Wall& temp_wall : temp_walls_) {
      temp_wall.Draw();
    }
    
  } else {
    temp_walls_.clear();
  }
}

const std::vector<Wall> &Map::GetWalls() const {
  return walls_;
}

std::vector<Snack> &Map::GetSnacks() {
  return snacks_;
}
const glm::vec2 &Map::GetPacmanLocation() const {
  return pacman_location_;
}
const std::vector<glm::vec2> &Map::GetGhostLocations() const {
  return ghost_locations_;
}
std::vector<PowerUp> &Map::GetPowerUps() {
  return power_ups_;
}
const std::vector<Wall> &Map::GetTempWalls() const {
  return temp_walls_;
}
std::vector<std::vector<char>> &Map::GetMapTiles() {
  return map_tiles_;
}

} // namespace PacmanGame
