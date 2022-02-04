#pragma once
#include "cinder/gl/gl.h"
#include "cinder/app/App.h"
#include "cinder/gl/gl.h"
#include "pacman.h"
#include "map.h"
#include "ghost.h"

namespace PacmanGame {

typedef std::chrono::time_point<std::chrono::system_clock> TimePoint;

class PacmanEngine {
 public:
  
  /**
   * Constructs an instance of the Pacman engine
   */
  PacmanEngine();
  
  /**
   * Draws the game including all the elements
   */
  void DrawGame();
  
  /**
   * Sets up the game by loading in the map
   * @param map the path to the map
   */
  void SetUpGame(const std::string& map);
  
  /**
   * Moves Pacman
   * @param event the keyboard event from Cinder
   */
  void MovePacman(const std::string& direction);
  
  /**
  * Updates the state of the game
  */
  void UpdateGame();

  size_t GetScore() const;
  const Map &GetMap() const;
  const Pacman &GetPacman() const;
  const std::vector<Ghost> &GetGhosts() const;
  const std::pair<size_t, size_t> &GetPacmanCoordinates() const;

  // Change this constant if you want different colored ghosts
  const std::vector<std::string> kGhostColors = {"red", "purple", "orange"};

  // These constants are the first letter of the ghosts' names
  const std::vector<char> kGhostNames = {'B', 'I', 'C'};

  enum GameState {ACTIVE, END, WON};
  enum PacmanState {ALIVE, DEAD};
  
  GameState GetGameState() const;
  const size_t &GetLives() const;
  PacmanState GetPacmanState() const;

 private:
  Pacman pacman_;
  PacmanState pacman_state_;
  Coordinates pacman_coordinates_;
  Coordinates initial_pacman_coordinates_;
  std::vector<std::vector<char>> map_tiles_;
  std::vector<Ghost> ghosts_;
  Map map_;
  TimePoint begin_time_;
  size_t score_;
  size_t lives_;
  GameState game_state_;
  
  // Duration of Pacman's power up
  const static size_t kPowerUpDuration = 5;
  
  // Draws the number of lives you have left 
  void DrawLives() const;
  
  /**
   * Checks if there is a collision with any walls "above" Pacman
   * @return a bool on whether or not there is a collision
   */
  bool IsUpCollision();

  /**
   * Checks if there is a collision with any walls "below" Pacman
   * @return a bool on whether or not there is a collision
   */
  bool IsDownCollision();

  /**
   * Checks if there is a collision with any walls "left" of Pacman
   * @return a bool on whether or not there is a collision
   */
   bool IsLeftCollision();

  /**
   * Checks if there is a collision with any walls "right" of Pacman
   * @return a bool on whether or not there is a collision
   */
  bool IsRightCollision();
  

  /**
   * Checks if Pacman has eaten a snack
   * @return a bool on whether or not Pacman ate a snack while moving 
   */
  bool HasEatenSnack();

  /**
   * Checks if Pacman has eaten a power up
   * @return a bool on whether or not Pacman ate a power up while moving 
   */
  bool HasEatenPowerUp();

  /**
   * Gets the initial coordinates of Pacman
   */
  void GetInitialPacmanCoordinates();
  
  /**
   * Checks if Pacman has died
   * @param ghost the ghost in the map
   * @return a bool on whether Pacman has died
   */
  bool HasDied(const Ghost &ghost);
  
  /**
   * Respawns Pacman back at its initial location
   */
  void RespawnPacman();
  
  /**
   * Scatters the ghosts
   */
  void ScatterGhosts();
  
  /**
   * Updates the DEAD/ALIVE state and NORMAL/POWERED UP state of Pacman
   */
  void UpdatePacmanState();
};

} // namespace PacmanGame