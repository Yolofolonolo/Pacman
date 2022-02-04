#include <catch2/catch.hpp>
#include <pacman_engine.h>
#include "ghost.h"

using namespace PacmanGame;

TEST_CASE("Update successfully updates the position of the ghosts") {
  PacmanEngine pacman_engine;
  pacman_engine.SetUpGame("tests/test_map.txt");
  Map map = pacman_engine.GetMap();
  
  std::vector<Ghost> ghosts = pacman_engine.GetGhosts();
  
  std::vector<Coordinates> orig_positions;
  
  for (const Ghost& ghost : ghosts) {
    orig_positions.push_back(ghost.GetGhostCoordinates());
  }
  
  for (size_t i = 0; i < 16; i++) {
    pacman_engine.UpdateGame();
  }

  std::vector<Ghost> updated_ghosts = pacman_engine.GetGhosts();
  
  SECTION("Position is updated for all the ghosts") {
    bool is_successful = true;
    
    for (size_t i = 0; i < ghosts.size(); i++) {
      if (updated_ghosts[i].GetGhostCoordinates() == orig_positions[i]) {
        is_successful = false;
      }
    }
    REQUIRE(is_successful);
  }
  
  SECTION("The ghosts should move upward initially to chase Pacman") {
    bool is_successful = true;
    
    for (size_t i = 0; i < ghosts.size(); i++) {
      if (updated_ghosts[i].GetGhostCoordinates().first != orig_positions[i].first - 1) {
        is_successful = false;
      }
    }
    REQUIRE(is_successful);
    
    SECTION("The ghosts should move upward again to chase Pacman (they are "
            "correctly path finding") {
      bool new_is_successful = true;

      for (size_t i = 0; i < 16; i++) {
        pacman_engine.UpdateGame();
      }

      std::vector<Ghost> new_updated_ghosts = pacman_engine.GetGhosts();
      
      for (size_t i = 0; i < ghosts.size(); i++) {
        if (new_updated_ghosts[i].GetGhostCoordinates().first != 
        orig_positions[i].first - 2) {
          new_is_successful = false;
        }
      }
      REQUIRE(new_is_successful);
    }
  }
}