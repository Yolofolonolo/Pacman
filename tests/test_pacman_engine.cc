#include <catch2/catch.hpp>
#include "pacman_engine.h"

using namespace PacmanGame;

TEST_CASE("Map is loaded in correctly") {
  PacmanEngine pacman_engine;
  pacman_engine.SetUpGame("tests/test_map.txt");
  Map map = pacman_engine.GetMap();
  
  SECTION("Walls are loaded in correctly") {
    REQUIRE(map.GetWalls().size() == 38);
  }
  
  SECTION("Snacks are loaded in correctly") {
    REQUIRE(map.GetSnacks().size() == 3);
  }
  
  SECTION("Power ups are loaded in correctly") {
    REQUIRE(map.GetPowerUps().size() == 1);
  }
  
  SECTION("Ghosts are loaded in correctly") {
    REQUIRE(map.GetGhostLocations().size() == 2);
  }
}

TEST_CASE("Wall collision is functional") {
  PacmanEngine pacman_engine;
  pacman_engine.SetUpGame("tests/test_map.txt");
  
  SECTION("Left wall collision is functional") {
    pacman_engine.MovePacman("left");
    glm::vec2 curr_pos = pacman_engine.GetPacman().GetPosition();

    pacman_engine.MovePacman("left");
    glm::vec2 updated_pos = pacman_engine.GetPacman().GetPosition();
    REQUIRE(curr_pos == updated_pos);
  }

  SECTION("Upper wall collision is functional") {
    pacman_engine.MovePacman("up");
    glm::vec2 curr_pos = pacman_engine.GetPacman().GetPosition();

    pacman_engine.MovePacman("up");
    glm::vec2 updated_pos = pacman_engine.GetPacman().GetPosition();
    REQUIRE(curr_pos == updated_pos);
  }

  SECTION("Lower wall collision is functional") {
    pacman_engine.MovePacman("down");
    glm::vec2 curr_pos = pacman_engine.GetPacman().GetPosition();

    pacman_engine.MovePacman("down");
    glm::vec2 updated_pos = pacman_engine.GetPacman().GetPosition();
    REQUIRE(curr_pos == updated_pos);
  }

  SECTION("Right wall collision is functional") {
    
    // Moves Pacman to the right wall 
    pacman_engine.MovePacman("right");
    pacman_engine.MovePacman("right");
    pacman_engine.MovePacman("right");
    pacman_engine.MovePacman("right");

    glm::vec2 curr_pos = pacman_engine.GetPacman().GetPosition();
    
    pacman_engine.MovePacman("right");
    glm::vec2 updated_pos = pacman_engine.GetPacman().GetPosition();
    
    REQUIRE(curr_pos == updated_pos);
  }
}

TEST_CASE("Snacks and power ups are successfully eaten. Score is successfully"
          " updated") {
  PacmanEngine pacman_engine;
  pacman_engine.SetUpGame("tests/test_map.txt");
  Map map = pacman_engine.GetMap();
  std::vector<Snack> snacks = map.GetSnacks();
  std::vector<PowerUp> power_ups = map.GetPowerUps();
  
  SECTION("There should be 3 snacks and 1 power up to start out with") {
    REQUIRE(snacks.size() == 3);
    REQUIRE(power_ups.size() == 1);
  }
  
  SECTION("Moving right will eat a snack and reduce snack size and increase "
          "score by 1") {
    pacman_engine.MovePacman("right");
    Map updated_map = pacman_engine.GetMap();
    REQUIRE(updated_map.GetSnacks().size() == 2);
    REQUIRE(pacman_engine.GetScore() == 1);

    SECTION("Moving right again will eat a power up, reduce power up size "
            "and power up pacman") {
      pacman_engine.MovePacman("right");
      pacman_engine.MovePacman("right");
      Map new_updated_map = pacman_engine.GetMap();
      
      REQUIRE(new_updated_map.GetPowerUps().empty());
      
      SECTION("Pacman should be in powered up state") {
        REQUIRE(pacman_engine.GetPacman().GetState() == Pacman::POWERED_UP);
        
        SECTION("Ghosts should be frightened") {
          std::vector<Ghost> ghosts = pacman_engine.GetGhosts();
          bool all_ghosts_frightened = true;
          
          for (const Ghost& ghost : ghosts) {
            if (ghost.GetState() != Ghost::FRIGHTEN) {
              all_ghosts_frightened = false;
            }
          }
          REQUIRE(all_ghosts_frightened);
        }
      }
    }
  }
}

TEST_CASE("Pacman is able to eat the ghosts") {
  PacmanEngine pacman_engine;
  pacman_engine.SetUpGame("tests/test_map.txt");
  pacman_engine.MovePacman("right");
  pacman_engine.MovePacman("right");
  pacman_engine.MovePacman("right");
  
  for (size_t i = 0; i < 5; i++) {
    pacman_engine.MovePacman("down");
  }
  pacman_engine.UpdateGame();
  size_t score = pacman_engine.GetScore();
  REQUIRE(score == 103);
}


TEST_CASE("Pacman is able to die and respawn") {
  PacmanEngine pacman_engine;
  pacman_engine.SetUpGame("tests/test_map.txt");
  pacman_engine.MovePacman("right");
  
  SECTION("Ghosts are able to kill Pacman. Lives decrease by 1") {
    // We do 80 loops because the ghosts only move every 15 updates
    for (size_t i = 0; i < 75; i++) {
      pacman_engine.UpdateGame();
    }
    REQUIRE(pacman_engine.GetLives() == 2);
    REQUIRE(pacman_engine.GetPacmanState() == PacmanEngine::DEAD);
    
    SECTION("Pacman is able to respawn") {
      pacman_engine.UpdateGame();
      REQUIRE(pacman_engine.GetPacmanState() == PacmanEngine::ALIVE);
    }
  }
}

TEST_CASE("Game state is WON after you eat all the snacks") {
  PacmanEngine pacman_engine;
  pacman_engine.SetUpGame("tests/test_map.txt");
  pacman_engine.MovePacman("left");
  pacman_engine.MovePacman("right");
  pacman_engine.MovePacman("right");
  pacman_engine.MovePacman("right");
  pacman_engine.UpdateGame();
  REQUIRE(pacman_engine.GetGameState() == PacmanEngine::WON);
}

TEST_CASE("Game state is END after you lose all of your lives") {
  PacmanEngine pacman_engine;
  pacman_engine.SetUpGame("tests/test_map.txt");
  
  // We do 200 updates as that will reasonably let Pacman die 3 times
  for (size_t i = 0; i < 200; i++) {
    pacman_engine.UpdateGame();
  }
  REQUIRE(pacman_engine.GetGameState() == PacmanEngine::END);
}